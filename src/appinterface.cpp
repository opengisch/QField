#include "appinterface.h"
#include "qgismobileapp.h"

#include <qgsmaptoolidentify.h>

AppInterface::AppInterface( QgisMobileapp* app )
  : mApp( app )
{
}

void AppInterface::identifyFeatures( const QPointF point )
{
  mApp->identifyFeatures( point );
}

void AppInterface::showFeatureForm( const Feature& feature )
{
  mApp->showFeatureForm( feature );
}

void AppInterface::openProjectDialog()
{
  mApp->openProjectDialog();
}
