#ifndef FEATURELISTMODEL_H
#define FEATURELISTMODEL_H

#include <QAbstractItemModel>

#include <qgsmaptoolidentify.h>

#include "feature.h"

class FeatureListModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY( int count READ count NOTIFY countChanged )

  public:
    enum FeatureListRoles
    {
      FeatureIdRole = Qt::UserRole + 1,
      FeatureRole,
      LayerNameRole
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

    int count() const;

  signals:
    void countChanged();

  private:
    inline Feature* toFeature( const QModelIndex& index ) const
    {
      return static_cast<Feature*>( index.internalPointer() );
    }

  private:
    QList<Feature*> mFeatures;
};

#endif // FEATURELISTMODEL_H
