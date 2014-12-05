#ifndef FEATURELISTMODEL_H
#define FEATURELISTMODEL_H

#include <QAbstractItemModel>

#include <qgsmaptoolidentify.h>

#include "feature.h"

class FeatureListModel : public QAbstractItemModel
{
    Q_OBJECT

  public:
    enum FeatureListRoles
    {
      FeatureIdRole = Qt::UserRole + 1,
      FeatureRole
    };

    explicit FeatureListModel( QObject *parent = 0 );
    FeatureListModel( QList<QgsMapToolIdentify::IdentifyResult> features, QObject* parent = 0 );

    void setFeatures( const QList<QgsMapToolIdentify::IdentifyResult>& results );

    QHash<int, QByteArray> roleNames() const;
    QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& child ) const;
    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;

    Q_INVOKABLE unsigned int count()
    {
      return mCount;
    }

  private:
    inline QgsMapLayer* toLayer( const QModelIndex& index ) const
    {
      return static_cast<QgsMapLayer*>( index.internalPointer() );
    }
    inline Feature* toFeature( const QModelIndex& index ) const
    {
      return static_cast<Feature*>( index.internalPointer() );
    }

  private:
    QList<QgsMapLayer*> mLayers;
    QMap<QgsMapLayer*, QVector<Feature*> > mFeatures;
    unsigned int mCount;
};

#endif // FEATURELISTMODEL_H
