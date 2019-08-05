#ifndef QFIELDAPPAUTHREQUESTHANDLER_H
#define QFIELDAPPAUTHREQUESTHANDLER_H

// QGIS includes
#include <qgsapplication.h>

#include "qgsnetworkaccessmanager.h"

class QFieldAppAuthRequestHandler : public QObject, public QgsNetworkAuthenticationHandler
{
    Q_OBJECT

  public:

    QFieldAppAuthRequestHandler();
    void handleAuthRequest( QNetworkReply *reply, QAuthenticator *auth );

    Q_INVOKABLE void enterCredentials( const QString realm, const QString username, const QString password );
    Q_INVOKABLE void handleLayerLogins();

  signals:
    void showLoginDialog( const QString realm );
    void loginDialogClosed( const QString realm, const bool canceled );
    void reloadEverything();

  private:

    void authNeeded( const QString realm );
    QString getFirstUnhandledRealm();

    struct RealmEntry
    {
      RealmEntry( const QString &realm, bool canceled = false )
        : realm( realm )
        , canceled( canceled )
      {}

      QString realm;
      bool canceled = false;
    };

    QList<RealmEntry> mRealms;
};

#endif // QFIELDAPPAUTHREQUESTHANDLER_H
