/***************************************************************************
  referencingfeaturelistmodel.h - ReferencingFeatureListModel

 ---------------------
 begin                : 1.3.2019
 copyright            : (C) 2019 by David Signer
 email                : david@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef REFERENCINGFEATURELISTMODEL_H
#define REFERENCINGFEATURELISTMODEL_H

#include <QAbstractItemModel>
#include "qgsvectorlayer.h"
#include "attributeformmodel.h"

class QgsVectorLayer;
class AttributeFormModel;

class ReferencingFeatureListModel : public QStandardItemModel
{
  Q_OBJECT

  /**
   * The relation
   */
  Q_PROPERTY( QgsFeature feature WRITE setFeature READ feature NOTIFY featureChanged )
  Q_PROPERTY( QgsRelation relation WRITE setRelation READ relation NOTIFY relationChanged )

public:
  explicit ReferencingFeatureListModel(QObject *parent = nullptr);

  enum ReferencedFeatureListRoles
  {
    DisplayString = Qt::UserRole,
    ReferencingFeatureId,
    ReferencingFeature
  };

  QHash<int, QByteArray> roleNames() const override;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  void setFeature( const QgsFeature &feature );
  QgsFeature feature() const;

  void setRelation( const QgsRelation &relation );
  QgsRelation relation() const;

  Q_INVOKABLE void reload();
  Q_INVOKABLE void deleteFeature( QgsFeatureId referencingFeatureId );

signals:
  void attributeFormModelChanged();
  void featureChanged();
  void relationChanged();

private:
  struct Entry
  {
    Entry( const QString &displayString, const QgsFeatureId &referencingFeatureId )
      : displayString( displayString )
       , referencingFeatureId(referencingFeatureId)
    {}

    Entry() = default;

    QString displayString;
    QgsFeatureId referencingFeatureId;
  };

  QList<Entry> mEntries;

  QgsFeature mFeature;
  QgsRelation mRelation;

};

#endif // REFERENCINGFEATURELISTMODEL_H
