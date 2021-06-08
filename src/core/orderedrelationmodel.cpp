
#include "orderedrelationmodel.h"
#include "fieldexpressionvaluesgatherer.h"
#include "qgsrelation.h"
#include "qgsfeature.h"
#include "qgsfeaturerequest.h"
#include "qgsvectorlayer.h"
#include "qgsproject.h"
#include "qgsexpressioncontextutils.h"
#include <QAbstractTableModel>

#include "qfield_core_export.h"

OrderedRelationModel::OrderedRelationModel( QObject *parent )
  : ReferencingFeatureListModel( parent )
{
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
  reload();
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
  reload();
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
  reload();
  emit descriptionChanged();
}

QHash<int, QByteArray> OrderedRelationModel::roleNames() const
{
  QHash<int, QByteArray> roles = ReferencingFeatureListModel::roleNames();

  roles[OrderedRelationModel::ImagePathRole] = "ImagePath";
  roles[OrderedRelationModel::DescriptionRole] = "Description";
  roles[OrderedRelationModel::FeatureIdRole] = "FeatureId";

  return roles;
}

void OrderedRelationModel::updateModel()
{
  beginResetModel();

  if ( mGatherer )
    mEntries = mGatherer->entries();

  std::sort( mEntries.begin(), mEntries.end(), [=]( const Entry & e1, const Entry & e2 )
  {
    return e1.referencingFeature.attribute( mOrderingField ).toFloat() < e2.referencingFeature.attribute( mOrderingField ).toFloat();
  } );

  endResetModel();
  emit modelUpdated();
}

QVariant OrderedRelationModel::data(const QModelIndex &index, int role ) const
{
  QVariant result;

  if (index.row() < 0 || index.row() >= rowCount(QModelIndex()))
    return QVariant();

  QgsExpression expr;
  QgsExpressionContext context;
  context.appendScopes(QgsExpressionContextUtils::globalProjectLayerScopes(mRelation.referencingLayer()));
  context.setFeature( mEntries.at(index.row()).referencingFeature );

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
      return mEntries[index.row()].referencingFeature.id();
  }

  return ReferencingFeatureListModel::data( index, role );
}

void OrderedRelationModel::onViewCurrentFeatureChanged( int index )
{
  if (index < 0 || index >= rowCount(QModelIndex()))
    return;

  emit currentFeatureChanged( mEntries[index].referencingFeature );
}

void OrderedRelationModel::moveItems(const int fromIdx, const int toIdx)
{
  if ( fromIdx == toIdx )
    return;

  if ( !mRelation.isValid() )
    return;

  int orderingFieldIdx = mRelation.referencingLayer()->fields().indexFromName( mOrderingField );

  if ( orderingFieldIdx == -1 )
    return;

  int startIdx = std::min( fromIdx, toIdx );
  int endIdx = std::max( fromIdx, toIdx );
  int delta = fromIdx > toIdx ? 1 : -1;

  beginResetModel();

//      for i in range(start_index, end_index+1):
//          f = self._related_features[i]
//          if i == index_from:
//              self._related_features[i][self._ordering_field] = index_to + 1  # ranks are index +1 (start at 1)
//          else:
//              self._related_features[i][self._ordering_field] += delta

//          res = self._relation.referencingLayer().changeAttributeValue(f.id(), field_index, f[self._ordering_field])
//          print(res)

//      self._related_features = sorted(self._related_features, key=lambda _f: _f[self._ordering_field])

  endResetModel();
}
