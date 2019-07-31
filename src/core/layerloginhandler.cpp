#include "layerloginhandler.h"

#include <QDebug>
#include <QNetworkReply>
#include <QAuthenticator>

#include "qgsproject.h"
#include "qgscredentials.h"
#include "qgsnetworkaccessmanager.h"

#include "qfieldappauthrequesthandler.h"
#include "appinterface.h"

LayerLoginHandler::LayerLoginHandler( QObject *parent )
  : QStandardItemModel( parent )
{
  connect( QgsNetworkAccessManager::instance(), &QgsNetworkAccessManager::authRequestOccurred, [ = ]( QNetworkReply * reply, QAuthenticator * auth )
  {
    qDebug() << "Woop, finally got host: " << reply->url().host() << " and realm: " << auth->realm() << " and user: " << auth->user() << " and password:" << auth->password();

    QString realm = QStringLiteral( "%1 at %2" ).arg( auth->realm(), reply->url().host() );

    //create the fish
    //in case that we have wrong credentials, it's handled like a success because the auth values are not null
    //this needs to be improved
    if ( !mRealmList.contains( realm ) && ( auth->user().isNull() || auth->password().isNull() ) )
    {
      mRealmList << realm;
    }
  } );
}

QHash<int, QByteArray> LayerLoginHandler::roleNames() const
{
  QHash<int, QByteArray> roleNames = QStandardItemModel::roleNames();

  roleNames[DataSourceRole] = "DataSource";
  roleNames[LayerNameRole] = "LayerName";
  roleNames[LayerIdRole] = "LayerId";

  return roleNames;
}

QgsProject *LayerLoginHandler::project() const
{
  return mProject;
}

void LayerLoginHandler::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  mProject = project;

  emit projectChanged();
}

void LayerLoginHandler::enterCredentials( const QString realm, const QString username, const QString password )
{
  QgsCredentials::instance()->put( realm, username, password );
}

void LayerLoginHandler::reloadLayers()
{
  for ( int r = 0; r < rowCount(); ++r )
  {
    QgsMapLayer *mapLayer = mProject->mapLayer( data( index( r, 0 ), LayerIdRole ).toString() );
    //test hack
    //mapLayer->setDataSource( { QgsProject::instance()->pathResolver().readPath( "contextualWMSLegend=0&crs=EPSG:31287&dpiMode=7&featureCount=10&format=image/jpeg&layers=ch.swisstopo.swissbathy3d-reliefschattierung&password=2581qmtmq0t2schz&styles=default&url=https://wms.swisstopo.admin.ch/?&username=user_bsv0a" ) }, data(index(r,0), LayerNameRole).toString(), "wms", QgsDataProvider::ProviderOptions());

    mapLayer->reload();
    if ( mapLayer->isValid() )
    {
      qDebug() << "trallala";
      removeRow( r );
    }
  }
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
        //QgsProject::instance()->read();
        //QgsProject::instance()->reloadAllLayers();
        emit reloadEverything();
      }
    } );
  }
}
