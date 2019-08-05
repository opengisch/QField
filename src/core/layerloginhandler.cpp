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

void LayerLoginHandler::authNeeded( const QString realm )
{
  if ( !mRealmList.contains( realm ) )
  {
    mRealmList << realm;
  }

}
