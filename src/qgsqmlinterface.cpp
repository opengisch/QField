#include "qgsqmlinterface.h"
#include "qgismobileapp.h"

#include <qgsmaptoolidentify.h>

QgsQmlInterface::QgsQmlInterface( QgisMobileapp* app )
  : mApp( app )
{
}

void QgsQmlInterface::identifyFeatures( const QPointF point )
{
  mApp->identifyFeatures( point );
}

void QgsQmlInterface::openProjectDialog()
{
  mApp->openProjectDialog();
}
