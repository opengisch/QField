#include "qfieldappauthrequesthandler.h"

#include <QAuthenticator>
#include <QThread>

#include "qgscredentials.h"

QFieldAppAuthRequestHandler::QFieldAppAuthRequestHandler()
{

}


void QFieldAppAuthRequestHandler::enterCredentials( const QString realm, const QString username, const QString password )
{
  QgsCredentials::instance()->put( realm, username, password );
}

QString QFieldAppAuthRequestHandler::getFirstUnhandledRealm()
{
  for ( const RealmEntry &entry : mRealms )
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

    connect( this, &QFieldAppAuthRequestHandler::loginDialogClosed, [ = ]( QString realm, bool canceled )
    {
      qDebug() << "close login for: " << realm;

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
        qDebug() << "Reload everything";
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

void QFieldAppAuthRequestHandler::resetCanceledRealms()
{
  for ( int i = 0; i < mRealms.count(); i++ )
  {
    mRealms.replace( i, RealmEntry( mRealms.at( i ).realm, false ) );
  }
}

void QFieldAppAuthRequestHandler::authNeeded( const QString realm )
{
  for ( const RealmEntry &entry : mRealms )
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

    qDebug() << "We have U: " << username << " and P: " << password << " on " << auth->realm() << " @ " << reply->url().host() << " here QFieldAppAuthRequestHandler::handleAuthRequest";

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
