/***************************************************************************
  messagelogmodel.h - MessageLogModel

 ---------------------
 begin                : 13.7.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MESSAGELOGMODEL_H
#define MESSAGELOGMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <qgsmessagelog.h>
#include "qfieldcore_global.h"

/**
 * This model will connect to the message log and publish any
 * messages received from there.
 */
class QFIELDCORE_EXPORT MessageLogModel : public QAbstractListModel
{
    Q_OBJECT

    struct LogMessage
    {
      LogMessage()
      {}

      LogMessage( const QString &tag, const QString &message, Qgis::MessageLevel level )
      {
        this->datetime = QDateTime::currentDateTime().toString( QStringLiteral( "yyyy-MM-dd hh:mm:ss:zzz" ) );
        this->tag = tag;
        this->message = message;
        this->level = level;
      }

      QString datetime;
      QString tag;
      QString message;
      Qgis::MessageLevel level;
    };

    enum Roles
    {
      MessageRole = Qt::UserRole,
      MessageTagRole,
      MessageLevelRole,
      MessageDateTimeRole
    };

  public:
    MessageLogModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    //! activates suppression of messages wit specific tags
    Q_INVOKABLE void suppressTags( const QList <QString> &tags );
    //! deactivates suppression of messages wit specific tags
    Q_INVOKABLE void unsuppressTags( const QList <QString> &tags );

  private slots:
    void onMessageReceived( const QString &message, const QString &tag, Qgis::MessageLevel level );

  private:
    QgsMessageLog *mMessageLog;
    QVector<LogMessage> mMessages;
    QList< QString > mSuppressedTags;
};

#endif // MESSAGELOGMODEL_H
