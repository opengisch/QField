#include "qfieldappauthrequesthandler.h"

#include <QAuthenticator>
#include <QThread>

#include "qgscredentials.h"

QFieldAppAuthRequestHandler *QFieldAppAuthRequestHandler::sAppAuthRequestHandler = nullptr;

QFieldAppAuthRequestHandler *QFieldAppAuthRequestHandler::instance()
{
  if ( !sAppAuthRequestHandler )
  {
    sAppAuthRequestHandler = new QFieldAppAuthRequestHandler();
  }
  return sAppAuthRequestHandler;
}

QFieldAppAuthRequestHandler::QFieldAppAuthRequestHandler()
{

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
      return;

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

void QFieldAppAuthRequestHandler::setInstance(QFieldAppAuthRequestHandler *appAuthRequestHandler)
{
  if( appAuthRequestHandler )
    sAppAuthRequestHandler = appAuthRequestHandler;
}
