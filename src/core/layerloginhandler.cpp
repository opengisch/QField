#include "layerloginhandler.h"

#include <QDebug>
#include <QNetworkReply>
#include <QAuthenticator>

#include "qgsproject.h"
#include "qgscredentials.h"
#include "qgsnetworkaccessmanager.h"

#include "qfieldappauthrequesthandler.h"
#include "appinterface.h"

LayerLoginHandler::LayerLoginHandler()
{
  connect( QgsNetworkAccessManager::instance(), &QgsNetworkAccessManager::authRequestOccurred, [ = ]( QNetworkReply * reply, QAuthenticator * auth )
  {
    qDebug() << "We received the host: " << reply->url().host() << " and realm: " << auth->realm() << " and user: " << auth->user() << " and password:" << auth->password();

    QString realm = QStringLiteral( "%1 at %2" ).arg( auth->realm(), reply->url().host() );

    //in case that we have wrong credentials, it's handled like a success because the auth values are not null
    if ( !mRealmList.contains( realm ) && ( auth->user().isNull() || auth->password().isNull() ) )
    {
      mRealmList << realm;
    }
  } );
}

void LayerLoginHandler::enterCredentials( const QString realm, const QString username, const QString password )
{
  QgsCredentials::instance()->put( realm, username, password );
}

void LayerLoginHandler::handleLayerLogins()
{
  if ( mRealmList.count() > 0 )
  {
    qDebug() << "open login for: " << mRealmList.first();

    emit showLoginDialog( mRealmList.first() );

    connect( this, &LayerLoginHandler::loginDialogClosed, [ = ]( QString realm )
    {
      qDebug() << "close login for: " << realm;
      mRealmList.removeAll( realm );

      if ( !mRealmList.isEmpty() )
      {
        qDebug() << "open login for: " << mRealmList.first();
        emit showLoginDialog( mRealmList.first() );
      }
      else
      {
        qDebug() << "Reload everything";
        emit reloadEverything();
      }
    } );
  }
}
