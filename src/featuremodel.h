#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include <QAbstractListModel>
#include "feature.h"

class Feature;

class FeatureModel : public QAbstractListModel
{
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

    Q_INVOKABLE void setFeature( Feature feature );

    QHash<int, QByteArray> roleNames() const;
    int rowCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;

  signals:

  public slots:

  private:
    Feature mFeature;
};

#endif // FEATUREMODEL_H
