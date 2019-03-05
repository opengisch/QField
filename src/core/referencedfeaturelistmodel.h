#ifndef REFERENCEDFEATURELISTMODEL_H
#define REFERENCEDFEATURELISTMODEL_H

#include <QAbstractItemModel>
#include "qgsvectorlayer.h"
#include "attributeformmodel.h"

class QgsVectorLayer;
class AttributeFormModel;

class ReferencedFeatureListModel : public QStandardItemModel
{
  Q_OBJECT

  /**
   * The relation
   */
  Q_PROPERTY( AttributeFormModel *attributeFormModel READ attributeFormModel WRITE setAttributeFormModel )
  Q_PROPERTY( QgsFeatureId featureId WRITE setFeatureId READ featureId )
  Q_PROPERTY( QgsRelation relation WRITE setRelation READ relation )

public:
  explicit ReferencedFeatureListModel(QObject *parent = nullptr);

  enum ReferencedFeatureListRoles
  {
    DisplayString = Qt::UserRole,
    FeatureId
  };

  QHash<int, QByteArray> roleNames() const override;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  void setFeatureId( const QgsFeatureId &featureId );
  QgsFeatureId featureId () const;

  void setRelation( const QgsRelation &relation );
  QgsRelation relation() const;

  AttributeFormModel *attributeFormModel() const;
  void setAttributeFormModel( AttributeFormModel *attributeFormModel );

  void reload();

private:
  struct Entry
  {
    Entry( const QString &displayString, const QgsFeatureId &featureId )
      : displayString( displayString )
       , featureId(featureId)
    {}

    Entry() = default;

    QString displayString;
    QgsFeatureId featureId;
  };

  QList<Entry> mEntries;

  AttributeFormModel *mAttributeFormModel;
  QgsFeatureId mFeatureId=-1;
  QgsRelation mRelation;

};

#endif // REFERENCEDFEATURELISTMODEL_H
