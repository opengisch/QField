#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include <QAbstractListModel>
#include "feature.h"

class Feature;

class FeatureModel : public QAbstractListModel
{
    Q_PROPERTY( QVariant feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_OBJECT

  public:
    enum FeatureRoles
    {
      AttributeName = Qt::UserRole + 1,
      AttributeValue,
      AttributeEditable
    };

    explicit FeatureModel( QObject *parent = 0 );
    explicit FeatureModel( const QgsFeature& feat, QObject *parent = 0 );

    void setFeature( QVariant feature );
    QVariant feature();

    QHash<int, QByteArray> roleNames() const;
    int rowCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;

  signals:
    void featureChanged();

  public slots:

  private:
    Feature mFeature;
};

#endif // FEATUREMODEL_H
