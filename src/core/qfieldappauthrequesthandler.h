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

    //! stores the credentials after the information is entered in the login dialog
    Q_INVOKABLE void enterCredentials( const QString realm, const QString username, const QString password );

    //! handles each realm after the project has been loaded
    Q_INVOKABLE void handleLayerLogins();

    //! resets the cancel state after reopening the project
    Q_INVOKABLE void resetCanceledRealms();


  signals:
    void showLoginDialog( const QString realm );
    void loginDialogClosed( const QString realm, const bool canceled );
    void reloadEverything();

  private:

    //! adds the realm to the list on loading the project
    void authNeeded( const QString realm );

    //! returns an unhandled realm
    QString getFirstUnhandledRealm();

    //! the realms that are not (yet) successfully logged in into
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
