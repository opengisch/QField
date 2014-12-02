#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include <QAbstractListModel>
#include "qgsfeature.h"

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

    void setFeature( const QgsFeature& feature );

    QHash<int, QByteArray> roleNames() const;
    int rowCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;

  signals:

  public slots:

  private:
    QgsFeature mFeature;
};

#endif // FEATUREMODEL_H
