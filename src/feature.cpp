#include "feature.h"

Feature::Feature( const QgsFeature& feature, QgsVectorLayer* layer )
  : mFeature( feature )
  , mLayer( layer )
{
}

Feature::Feature()
  : mLayer( 0 )
{
}

const QString Feature::displayText() const
{
  if ( !mLayer->displayExpression().isEmpty() )
    return QgsExpression( mLayer->displayExpression() ).evaluate( mFeature ).toString();
  else
    return mFeature.attribute( mLayer->displayField() ).toString();
}
