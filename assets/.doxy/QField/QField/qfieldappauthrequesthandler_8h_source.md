

# File qfieldappauthrequesthandler.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldappauthrequesthandler.h**](qfieldappauthrequesthandler_8h.md)

[Go to the documentation of this file](qfieldappauthrequesthandler_8h.md)


```C++
/***************************************************************************
              qfieldappauthrequesthandler.h
              -------------------
              begin                : August 2019
              copyright            : (C) 2019 by David Signer
              email                : david (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFIELDAPPAUTHREQUESTHANDLER_H
#define QFIELDAPPAUTHREQUESTHANDLER_H

#include <qgsapplication.h>
#include <qgsconfig.h>
#include <qgscredentials.h>
#include <qgsnetworkaccessmanager.h>

class QFieldAppAuthRequestHandler : public QObject, public QgsCredentials, public QgsNetworkAuthenticationHandler
{
    Q_OBJECT

    Q_PROPERTY( bool isProjectLoading READ isProjectLoading WRITE setIsProjectLoading NOTIFY isProjectLoadingChanged )
    Q_PROPERTY( bool hasPendingAuthRequest READ hasPendingAuthRequest NOTIFY hasPendingAuthRequestChanged )

  public:
    QFieldAppAuthRequestHandler();

    void handleAuthRequest( QNetworkReply *reply, QAuthenticator *auth ) override;
    void handleAuthRequestOpenBrowser( const QUrl &url ) override;
    void handleAuthRequestCloseBrowser() override;

    Q_INVOKABLE void enterCredentials( const QString &realm, const QString &username, const QString &password );

    Q_INVOKABLE bool handleLayerLogins();

    Q_INVOKABLE void clearStoredRealms();

    Q_INVOKABLE void abortAuthBrowser();

    bool isProjectLoading() const;

    void setIsProjectLoading( bool loading );

    bool hasPendingAuthRequest() const;

  signals:
    void showLoginDialog( const QString &realm, const QString &title );
    void loginDialogClosed( const QString &realm, const bool canceled );
    void reloadEverything();
    void showLoginBrowser( const QString &url );
    void hideLoginBrowser();
    void isProjectLoadingChanged();
    void hasPendingAuthRequestChanged();

  protected:
    bool request( const QString &realm, QString &username, QString &password, const QString &message = QString() ) override;
    bool requestMasterPassword( QString &password, bool stored = false ) override { return false; }

  private:
    void showLogin();

    void authNeeded( const QString &realm );

    QString getFirstUnhandledRealm() const;

    QString getCredentialTitle( const QString &realm );

    struct RealmEntry
    {
        explicit RealmEntry( const QString &realm, bool canceled = false )
          : realm( realm )
          , canceled( canceled )
        {}

        QString realm;
        bool canceled = false;
    };

    QList<RealmEntry> mRealms;
    bool mBrowserAuthenticationOngoing = false;

    bool mIsProjectLoading = false;
};

#endif // QFIELDAPPAUTHREQUESTHANDLER_H
```


