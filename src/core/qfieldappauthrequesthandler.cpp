/***************************************************************************
              qfieldappauthrequesthandler.cpp
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

#include "qfieldappauthrequesthandler.h"

// Check QGIS Version
#if VERSION_INT >= 30600
#include <QAuthenticator>
#include <QThread>
#include <qgsmessagelog.h>

#include <qgscredentials.h>

QFieldAppAuthRequestHandler::QFieldAppAuthRequestHandler()
{

}

void QFieldAppAuthRequestHandler::enterCredentials( const QString realm, const QString username, const QString password )
{
  QgsCredentials::instance()->put( realm, username, password );
}

QString QFieldAppAuthRequestHandler::getFirstUnhandledRealm()
{
  for ( const RealmEntry &entry : qgis::as_const( mRealms ) )
  {
    if ( !entry.canceled )
    {
      return entry.realm;
    }
  }
  return QString();
}

bool QFieldAppAuthRequestHandler::handleLayerLogins()
{
  if ( !getFirstUnhandledRealm().isEmpty() )
  {
    emit showLoginDialog( getFirstUnhandledRealm() );

    connect( this, &QFieldAppAuthRequestHandler::loginDialogClosed, [ = ]( const QString &realm, bool canceled )
    {
      if ( canceled )
      {
        //realm not successful handled - but canceled
        for ( int i = 0; i < mRealms.count(); i++ )
        {
          if ( mRealms.at( i ).realm == realm )
          {
            mRealms.replace( i, RealmEntry( realm, true ) );
            break;
          }
        }
      }
      else
      {
        //realm successful handled (credentials saved) - remove realm
        for ( int i = 0; i < mRealms.count(); i++ )
        {
          if ( mRealms.at( i ).realm == realm )
          {
            mRealms.removeAt( i );
            break;
          }
        }
      }

      if ( !getFirstUnhandledRealm().isEmpty() )
      {
        //show dialog as long as there are unhandled realms
        emit showLoginDialog( getFirstUnhandledRealm() );
      }
      else
      {
        emit reloadEverything();
      }
    } );
  }
  else
  {
    return false;
  }
  return true;
}

void QFieldAppAuthRequestHandler::clearStoredRealms()
{
  mRealms.clear();
}

void QFieldAppAuthRequestHandler::authNeeded( const QString realm )
{
  for ( const RealmEntry &entry : qgis::as_const( mRealms ) )
  {
    if ( entry.realm == realm )
    {
      //realm already in list
      return;
    }
  }

  RealmEntry unhandledRealm( realm );
  mRealms << unhandledRealm;
}

void QFieldAppAuthRequestHandler::handleAuthRequest( QNetworkReply *reply, QAuthenticator *auth )
{
  Q_ASSERT( qApp->thread() == QThread::currentThread() );

  QString username = auth->user();
  QString password = auth->password();

  if ( username.isEmpty() && password.isEmpty() && reply->request().hasRawHeader( "Authorization" ) )
  {
    QByteArray header( reply->request().rawHeader( "Authorization" ) );
    if ( header.startsWith( "Basic " ) )
    {
      QByteArray auth( QByteArray::fromBase64( header.mid( 6 ) ) );
      int pos = auth.indexOf( ':' );
      if ( pos >= 0 )
      {
        username = auth.left( pos );
        password = auth.mid( pos + 1 );
      }
    }
  }

  for ( ;; )
  {
    bool ok = QgsCredentials::instance()->get(
                QStringLiteral( "%1 at %2" ).arg( auth->realm(), reply->url().host() ),
                username, password,
                QObject::tr( "Authentication required" ) );

    if ( !ok )
    {
      authNeeded( QStringLiteral( "%1 at %2" ).arg( auth->realm(), reply->url().host() ) );
      return;
    }

    if ( auth->user() != username || ( password != auth->password() && !password.isNull() ) )
    {
      // save credentials
      QgsCredentials::instance()->put(
        QStringLiteral( "%1 at %2" ).arg( auth->realm(), reply->url().host() ),
        username, password
      );
      break;
    }
    else
    {
      // credentials didn't change - stored ones probably wrong? clear password and retry
      QgsCredentials::instance()->put(
        QStringLiteral( "%1 at %2" ).arg( auth->realm(), reply->url().host() ),
        username, QString() );
    }
  }

  auth->setUser( username );
  auth->setPassword( password );
}
#endif // VERSION_INT >= 30600
