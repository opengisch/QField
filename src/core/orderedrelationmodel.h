
#include "qgsrelation.h"
#include "qgsfeature.h"
#include "qgsfeaturerequest.h"
#include "qgsvectorlayer.h"
#include <QAbstractTableModel>

class FeatureExpressionValuesGatherer;

class OrderedRelationModel: public QAbstractTableModel
{
  public:

  enum Roles
  {
    ImagePathRole = Qt::UserRole + 1,
    DescriptionRole,
    FeatureIdRole,
  };

  Q_PROPERTY( QgsRelation relation READ relation WRITE relation NOTIFY relationChanged)
  Q_PROPERTY( QgsFeature feature READ feature WRITE feature NOTIFY featureChanged)
  Q_PROPERTY( QString orderingField READ orderingField WRITE orderingField NOTIFY orderingFieldChanged)
  Q_PROPERTY( QString imagePath READ imagePath WRITE imagePath NOTIFY imagePathChanged)
  Q_PROPERTY( QString description READ description WRITE description NOTIFY descriptionChanged)

  OrderedRelationModel( QObject *parent );

  QgsRelation relation() const;
  void setRelation( const QgsRelation &relation );
  QString relationId() const;
  void setRelationId( const QString &relationId );
  QgsFeature feature() const;
  void setFeature( const QgsFeature &feature );
  QString orderingField() const;
  void setOrderingField( const QString &orderingField );
  QString imagePath() const;
  void setImagePath( const QString &imagePath );
  QString description() const;
  void setDescription( const QString &description );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex index, Roles role );
  bool setData( const QModelIndex index, const QVariant value, Roles role);


  QHash<int, QByteArray> roleNames() const override;

  void reloadData();
  void processFeatureList();

  signals:
  void relationChanged();
  void featureChanged();
  void orderingFieldChanged();
  void imagePathChanged();
  void descriptionChanged();


  private:

    QgsRelation mRelation;
    QString mOrderingField;
    QString mImagePath;
    QString mDescription;
    QgsFeature mFeature;
    QList<QgsFeature> mRelatedFeatures;

    FeatureExpressionValuesGatherer *mGatherer = nullptr;
};
