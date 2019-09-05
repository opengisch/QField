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

// QGIS includes
#include <qgsapplication.h>
#include <qgsconfig.h>

#if VERSION_INT >= 30600
#include "qgsnetworkaccessmanager.h"

/**
 *
 * When a layer requests and authentication, this handler is triggered. When the credentials are found in
 * QgsCredentials (because it's stored before), they are used for the login. If not, QGIS would popup a
 * Login Dialog, but QField does not. The realms (where it should log in to) are just added to the list
 * of unhandled realms. When all layers are handled, the login dialog is diplayed for each of them. On
 * successful login, the credentials are stored in QgsCredentials and removed from the list. When the list
 * is empty (or contains only canceled realms), the project is reloaded. This time the logins are contained
 * in QgsCredentials and no dialog needs to pop up.
 *
 */
class QFieldAppAuthRequestHandler : public QObject, public QgsNetworkAuthenticationHandler
{
    Q_OBJECT

  public:

    QFieldAppAuthRequestHandler();

    //! handles the auth request - triggered by the authRequestOccurred signal
    void handleAuthRequest( QNetworkReply *reply, QAuthenticator *auth );

    //! stores the credentials after the information is entered in the login dialog
    Q_INVOKABLE void enterCredentials( const QString realm, const QString username, const QString password );

    //! handles each realm after the project has been loaded
    Q_INVOKABLE bool handleLayerLogins();

    //! clears the stored realms
    Q_INVOKABLE void clearStoredRealms();


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
#endif // VERSION_INT >= 30600
#endif // QFIELDAPPAUTHREQUESTHANDLER_H
