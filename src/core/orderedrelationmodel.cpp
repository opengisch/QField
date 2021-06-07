
#include "orderedrelationmodel.h"
#include "fieldexpressionvaluesgatherer.h"
#include "qgsrelation.h"
#include "qgsfeature.h"
#include "qgsfeaturerequest.h"
#include "qgsvectorlayer.h"
#include "qgsexpressioncontextutils.h"
#include <QAbstractTableModel>


OrderedRelationModel::OrderedRelationModel( QObject *parent )
  :QAbstractTableModel( parent )
{
}

QgsRelation OrderedRelationModel::relation() const
{
  return mRelation;
}

void OrderedRelationModel::setRelation( const QgsRelation &relation )
{
  mRelation = relation;
  reload();
  emit relationChanged();
}

QString OrderedRelationModel::currentRelationId() const
{
  return mRelation.isValid() ? mRelation.id() : QString();
}

void OrderedRelationModel::setCurrentRelationId( const QString &relationId )
{

  if ( relationId == currentRelationId() )
    return;


  mRelation = QgsProject::instance()->relationManager()->relation( relationId );
  reload();
  emit relationChanged();
}


QgsFeature OrderedRelationModel::feature() const
{
  return mFeature;
}

void OrderedRelationModel::setFeature( const QgsFeature &feature )
{
  if ( mFeature == feature )
    return;

  mFeature = feature;
  emit featureChanged();
}

QString OrderedRelationModel::orderingField() const
{
  return mOrderingField;
}

void OrderedRelationModel::setOrderingField( const QString &orderingField )
{
  if ( mOrderingField == orderingField )
    return;

  mOrderingField = orderingField;
  emit orderingFieldChanged();
}

QString OrderedRelationModel::imagePath() const
{
  return mImagePath;
}

void OrderedRelationModel::setImagePath( const QString &imagePath )
{
  if ( mImagePath == imagePath )
    return;

  mImagePath = imagePath;
  emit imagePathChanged();
}

QString OrderedRelationModel::description() const
{
  return mDescription;
}

void OrderedRelationModel::setDescription( const QString &description )
{
  if ( mDescription == description )
    return;

  mDescription = description;
  emit descriptionChanged();
}

int OrderedRelationModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )

  return mRelatedFeatures.count();
}

int OrderedRelationModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )

  return 1;
}

QHash<int, QByteArray> OrderedRelationModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractTableModel::roleNames();

  roles[OrderedRelationModel::ImagePathRole] = "ImagePath";
  roles[OrderedRelationModel::DescriptionRole] = "Description";
  roles[OrderedRelationModel::FeatureIdRole] = "FeatureId";

  return roles;
}

void OrderedRelationModel::reloadData()
{
  if ( mOrderingField.isEmpty() || !mRelation.isValid() )
    return;

  QgsFeatureRequest request = mRelation.getRelatedFeaturesRequest( mFeature );
  mGatherer = new FeatureExpressionValuesGatherer(
        mRelation.referencingLayer(),
        mRelation.referencingLayer()->displayExpression(),
        request
  );
  connect( mGatherer, &QThread::finished, this, &OrderedRelationModel::processFeatureList );
  mGatherer->start();
}

void OrderedRelationModel::processFeatureList()
{
  if ( !mGatherer )
    return;

  QList<QgsFeature> relatedFeatures;

  for ( const FeatureExpressionValuesGatherer::Entry &gatheredEntry : mGatherer->entries() )
    relatedFeatures.append( gatheredEntry.feature );

  std::sort( relatedFeatures.begin(), relatedFeatures.end(), [ = ]( const QgsFeature & f1, const QgsFeature & f2 )
  {
    return f1.attribute( mOrderingField ).toFloat() < f2.attribute( mOrderingField ).toFloat();
  } );

  mGatherer->deleteLater();
  mGatherer = nullptr;

  beginResetModel();
  mRelatedFeatures = relatedFeatures;
  endResetModel();
}

QVariant OrderedRelationModel::data(const QModelIndex index, Roles role )
{
  QVariant result;

  if (index.row() < 0 || index.row() >= rowCount(QModelIndex()))
    return QVariant();

  QgsExpression expr;
  QgsExpressionContext context;
  context.appendScopes(QgsExpressionContextUtils::globalProjectLayerScopes(mRelation.referencingLayer()));
  context.setFeature(mRelatedFeatures.at(index.row()));

  switch ( role ) {
    case ImagePathRole:
      expr = QgsExpression(mImagePath);
      result = expr.evaluate(&context);
      return result;
    case DescriptionRole:
      expr = QgsExpression(mDescription);
      result = expr.evaluate(&context);
      return result;
    case FeatureIdRole:
      return mRelatedFeatures[index.row()].id();
  }

  return QVariant();
}


bool OrderedRelationModel::setData( const QModelIndex index, const QVariant value, Roles role)
{
  Q_UNUSED( index );
  Q_UNUSED( value );
  Q_UNUSED( role );
  return false;
}

//def moveitems(self, index_from, index_to):
//    print(index_from, index_to)
//    if index_from == index_to:
//        return

//    field_index = self._relation.referencingLayer().fields().indexFromName(self._ordering_field)
//    if field_index < 0:
//        return

//    start_index = min(index_from, index_to)
//    end_index = max(index_from, index_to)
//    delta = 1 if index_from > index_to else -1

//    self.beginResetModel()

//    for i in range(start_index, end_index+1):
//        f = self._related_features[i]
//        if i == index_from:
//            self._related_features[i][self._ordering_field] = index_to + 1  # ranks are index +1 (start at 1)
//        else:
//            self._related_features[i][self._ordering_field] += delta

//        res = self._relation.referencingLayer().changeAttributeValue(f.id(), field_index, f[self._ordering_field])
//        print(res)

//    self._related_features = sorted(self._related_features, key=lambda _f: _f[self._ordering_field])

//    self.endResetModel()
