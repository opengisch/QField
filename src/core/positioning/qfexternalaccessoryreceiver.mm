/***************************************************************************
 qfexternalaccessoryreceiver.mm - QfExternalAccessoryReceiver

 ---------------------
 begin                : 26.09.2026
 copyright            : (C) 2026 by James Curtin
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfexternalaccessoryreceiver.h"
#include "qfield.h"

#include <QDebug>
#include <QPointer>
#include <QThread>

#include <functional>

#import <ExternalAccessory/ExternalAccessory.h>
#import <Foundation/Foundation.h>

QLatin1String QfExternalAccessoryReceiver::identifier =
    QLatin1String("eaccessory");

namespace {
//! Returns an NMEA-style sentence with its checksum and line ending
QByteArray nmeaSentence(const QByteArray &body) {
  char checksum = 0;
  for (const char c : body) {
    checksum ^= c;
  }
  return QByteArray("$") + body + QByteArray("*") +
         QByteArray::number(static_cast<uchar>(checksum), 16)
             .rightJustified(2, '0')
             .toUpper() +
         QByteArray("\r\n");
}

//! Returns the session message which starts the NMEA stream of the
//! com.bad-elf.gnss protocol, see
//! https://github.com/BadElf/gps-sdk/wiki/Integration-Guide:-iOS-&-iPadOS
QByteArray pbejsSessionMessage() {
  const QString bundleId =
      QString::fromNSString([[NSBundle mainBundle] bundleIdentifier]);
  return nmeaSentence(
      QStringLiteral(
          "PBEJS,{\"method\":\"session\",\"params\":{\"appName\":\"%1\","
          "\"appId\":\"%2\",\"appVersion\":\"%3\",\"msgs\":\"NMEA\"}}")
          .arg(Qfield::appName, bundleId, Qfield::appVersion)
          .toUtf8());
}

//! An external accessory protocol streaming NMEA sentences
struct AccessoryProtocol {
  //! Protocol string, also listed in Info.plist's
  //! UISupportedExternalAccessoryProtocols
  const char *name;
  //! Optional message to send once the session is open to start the stream
  QByteArray (*startMessage)();
};

const AccessoryProtocol sProtocols[] = {
    {"com.bad-elf.gnss", pbejsSessionMessage},
};
} // namespace

@interface QfExternalAccessoryStreamDelegate : NSObject <NSStreamDelegate>
- (instancetype)initWithHandler:
    (std::function<void(NSStream *, NSStreamEvent)>)handler;
@end

@implementation QfExternalAccessoryStreamDelegate {
  std::function<void(NSStream *, NSStreamEvent)> mHandler;
}

- (instancetype)initWithHandler:
    (std::function<void(NSStream *, NSStreamEvent)>)handler {
  self = [super init];
  if (self) {
    mHandler = handler;
  }
  return self;
}

- (void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode {
  if (mHandler) {
    mHandler(stream, eventCode);
  }
}
@end

class QfExternalAccessoryReceiver::SessionContainer {
public:
  explicit SessionContainer(QfExternalAccessoryReceiver *receiver)
      : mReceiver(receiver) {
    [[EAAccessoryManager sharedAccessoryManager] registerForLocalNotifications];

    // The blocks may run after this container is gone, only reach the
    // receiver through a guarded pointer
    QPointer<QfExternalAccessoryReceiver> guardedReceiver(receiver);
    NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
    mConnectObserver =
        [center addObserverForName:EAAccessoryDidConnectNotification
                            object:nil
                             queue:[NSOperationQueue mainQueue]
                        usingBlock:^(NSNotification *) {
                            if (guardedReceiver) {
                              guardedReceiver->openSession();
                            }
                        }];
    mDisconnectObserver =
        [center addObserverForName:EAAccessoryDidDisconnectNotification
                            object:nil
                             queue:[NSOperationQueue mainQueue]
                        usingBlock:^(NSNotification *notification) {
                            EAAccessory *accessory =
                                notification.userInfo[EAAccessoryKey];
                            if (guardedReceiver && accessory) {
                              guardedReceiver->handleAccessoryDisconnected(
                                  accessory.connectionID);
                            }
                        }];

    mDelegate = [[QfExternalAccessoryStreamDelegate alloc]
        initWithHandler:[this](NSStream *stream, NSStreamEvent event) {
          handleStreamEvent(stream, event);
        }];
  }

  ~SessionContainer() {
    close();
    NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
    [center removeObserver:mConnectObserver];
    [center removeObserver:mDisconnectObserver];
    mConnectObserver = nil;
    mDisconnectObserver = nil;
    mDelegate = nil;
    [[EAAccessoryManager sharedAccessoryManager]
        unregisterForLocalNotifications];
  }

  bool isOpen() const { return mSession != nil; }

  bool isOpenWith(unsigned long connectionId) const {
    return mSession && mSession.accessory.connectionID == connectionId;
  }

  //! Opens a session with the first connected accessory supporting the
  //! protocol, returns FALSE if none could be opened
  bool open(QString &error) {
    EAAccessory *accessory = nil;
    const AccessoryProtocol *protocol = nullptr;
    for (EAAccessory *candidate in
         [[EAAccessoryManager sharedAccessoryManager] connectedAccessories]) {
      for (const AccessoryProtocol &knownProtocol : sProtocols) {
        if ([candidate.protocolStrings containsObject:@(knownProtocol.name)]) {
          accessory = candidate;
          protocol = &knownProtocol;
          break;
        }
      }
      if (accessory) {
        break;
      }
    }
    if (!accessory) {
      return false;
    }

    qInfo() << "ExternalAccessoryReceiver: Opening session with"
            << QString::fromNSString(accessory.name)
            << QString::fromNSString(accessory.serialNumber);
    mSession = [[EASession alloc] initWithAccessory:accessory
                                        forProtocol:@(protocol->name)];
    if (!mSession) {
      error = QObject::tr("Could not open a session with the accessory");
      return false;
    }

    NSStream *streams[] = {mSession.inputStream, mSession.outputStream};
    for (NSStream *stream : streams) {
      stream.delegate = mDelegate;
      [stream scheduleInRunLoop:[NSRunLoop mainRunLoop]
                        forMode:NSRunLoopCommonModes];
      [stream open];
    }
    if (protocol->startMessage) {
      mPendingWrite = protocol->startMessage();
    }
    return true;
  }

  void close() {
    if (!mSession) {
      return;
    }

    NSStream *streams[] = {mSession.inputStream, mSession.outputStream};
    for (NSStream *stream : streams) {
      [stream close];
      [stream removeFromRunLoop:[NSRunLoop mainRunLoop]
                        forMode:NSRunLoopCommonModes];
      stream.delegate = nil;
    }
    mSession = nil;
    mPendingWrite.clear();
  }

private:
  void handleStreamEvent(NSStream *stream, NSStreamEvent event) {
    switch (event) {
    case NSStreamEventHasBytesAvailable: {
      NSInputStream *inputStream = mSession.inputStream;
      uint8_t chunk[1024];
      QByteArray bytes;
      while ([inputStream hasBytesAvailable]) {
        const NSInteger count = [inputStream read:chunk
                                        maxLength:sizeof(chunk)];
        if (count <= 0) {
          break;
        }
        bytes.append(reinterpret_cast<const char *>(chunk), count);
      }
      if (!bytes.isEmpty()) {
        mReceiver->handleBytesReceived(bytes);
      }
      break;
    }

    case NSStreamEventHasSpaceAvailable: {
      if (!mPendingWrite.isEmpty()) {
        const NSInteger written =
            [mSession.outputStream write:reinterpret_cast<const uint8_t *>(
                                             mPendingWrite.constData())
                               maxLength:mPendingWrite.size()];
        if (written > 0) {
          mPendingWrite.remove(0, written);
        }
      }
      break;
    }

    case NSStreamEventErrorOccurred:
    case NSStreamEventEndEncountered:
      qInfo() << "ExternalAccessoryReceiver: Stream ended"
              << (stream.streamError
                      ? QString::fromNSString(
                            stream.streamError.localizedDescription)
                      : QString());
      mReceiver->handleSessionEnded();
      break;

    default:
      break;
    }
  }

  QfExternalAccessoryReceiver *mReceiver = nullptr;
  QfExternalAccessoryStreamDelegate *mDelegate = nil;
  EASession *mSession = nil;
  id mConnectObserver = nil;
  id mDisconnectObserver = nil;
  QByteArray mPendingWrite;
};

QfExternalAccessoryReceiver::QfExternalAccessoryReceiver(QObject *parent)
    : QfNmeaGnssReceiver(parent), mBuffer(new QBuffer(this)) {
  initNmeaConnection(mBuffer);
  setValid(true);
}

QfExternalAccessoryReceiver::~QfExternalAccessoryReceiver() {
  // Stream and notification callbacks must be gone before the base classes
  // delete the buffer
  delete mContainer;
  mContainer = nullptr;
}

void QfExternalAccessoryReceiver::handleConnectDevice() {
  if (!mContainer) {
    mContainer = new SessionContainer(this);
  }
  openSession();
}

void QfExternalAccessoryReceiver::handleDisconnectDevice() {
  closeSession();
  delete mContainer;
  mContainer = nullptr;
  setSocketState(QAbstractSocket::UnconnectedState);
}

void QfExternalAccessoryReceiver::openSession() {
  if (!mContainer || mContainer->isOpen()) {
    return;
  }

  QString error;
  if (mContainer->open(error)) {
    mBufferData.clear();
    mBuffer->open(QIODevice::ReadWrite);
    if (!mLastError.isEmpty()) {
      mLastError.clear();
      emit lastErrorChanged(mLastError);
    }
    setSocketState(QAbstractSocket::ConnectedState);
    return;
  }

  if (!error.isEmpty()) {
    mLastError = error;
    qInfo() << QStringLiteral("ExternalAccessoryReceiver: Error: %1")
                   .arg(mLastError);
    emit lastErrorChanged(mLastError);
  }
  // Wait for the accessory to (re)connect
  setSocketState(QAbstractSocket::ConnectingState);
}

void QfExternalAccessoryReceiver::closeSession() {
  if (mContainer) {
    mContainer->close();
  }
  mBuffer->close();
  mBufferData.clear();
  mLastGnssPositionValid = false;
}

void QfExternalAccessoryReceiver::handleAccessoryDisconnected(
    unsigned long connectionId) {
  if (mContainer && mContainer->isOpenWith(connectionId)) {
    handleSessionEnded();
  }
}

void QfExternalAccessoryReceiver::handleSessionEnded() {
  closeSession();
  // Wait for the accessory to reconnect
  setSocketState(QAbstractSocket::ConnectingState);
}

void QfExternalAccessoryReceiver::handleBytesReceived(const QByteArray &bytes) {
  Q_ASSERT(QThread::currentThread() == thread());

  mBufferData.append(bytes);
  const qsizetype endSentenceIndex =
      mBufferData.lastIndexOf(QByteArrayLiteral("\r\n"));
  if (endSentenceIndex > -1) {
    // Keep sentences the NMEA connection has not read yet, its readyRead
    // handling is queued
    mBuffer->buffer().remove(0, mBuffer->pos());
    mBuffer->seek(mBuffer->size());
    mBuffer->write(mBufferData.left(endSentenceIndex + 2));
    mBuffer->seek(0);

    mBufferData = mBufferData.mid(endSentenceIndex + 2);
  } else if (mBufferData.size() > 64 * 1024) {
    // Not an NMEA stream, avoid growing without bounds
    mBufferData.clear();
  }
}
