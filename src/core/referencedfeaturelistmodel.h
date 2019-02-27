#ifndef REFERENCEDFEATURELISTMODEL_H
#define REFERENCEDFEATURELISTMODEL_H

#include <QAbstractItemModel>
#include "qgsvectorlayer.h"

class QgsVectorLayer;

class ReferencedFeatureListModel : public QAbstractItemModel
{
  Q_OBJECT

  /**
   * The relation
   */
  Q_PROPERTY( QgsFeatureId featureId WRITE setFeatureId READ featureId )
  Q_PROPERTY( QgsRelation relation WRITE setRelation READ relation )

public:
  explicit ReferencedFeatureListModel(QObject *parent = nullptr);

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  void setFeatureId( const QgsFeatureId &featureId );
  QgsFeatureId featureId () const;

  void setRelation( const QgsRelation &relation );
  QgsRelation relation() const;

  void feedTheModel();

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
  QgsFeatureId mFeatureId;
  QgsRelation mRelation;

};

#endif // REFERENCEDFEATURELISTMODEL_H
