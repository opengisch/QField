#pragma once

#include <QObject>
#include <QStringList>
#include <QTcpSocket>

class NtripSourceTableFetcher : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool fetching READ fetching NOTIFY fetchingChanged )
    Q_PROPERTY( QStringList mountpoints READ mountpoints NOTIFY mountpointsChanged )

  public:
    explicit NtripSourceTableFetcher( QObject *parent = nullptr );
    ~NtripSourceTableFetcher();

    bool fetching() const { return mFetching; }
    QStringList mountpoints() const { return mMountpoints; }

    Q_INVOKABLE void fetch( const QString &host, int port, const QString &username, const QString &password );
    Q_INVOKABLE void cancel();

  signals:
    void fetchingChanged();
    void mountpointsChanged();
    void fetchError( const QString &message );

  private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onSocketError( QAbstractSocket::SocketError error );

  private:
    void cleanup();
    QStringList parseSourceTable( const QByteArray &data ) const;

    QTcpSocket *mSocket = nullptr;
    QString mHost;
    quint16 mPort = 0;
    QString mUsername;
    QString mPassword;
    QByteArray mBuffer;
    bool mFetching = false;
    QStringList mMountpoints;
};
