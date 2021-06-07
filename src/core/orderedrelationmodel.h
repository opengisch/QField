/***************************************************************************
  orderedrelationmodel.h - OrderedRelationModel

 ---------------------
 begin                : Jun 2021
 copyright            : (C) 2021 by Ivan Ivanov
 email                : ivan (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ORDEREDRELATIONMODEL_H
#define ORDEREDRELATIONMODEL_H

#include "qgsrelation.h"
#include "qgsfeature.h"
#include "qgsfeaturerequest.h"
#include "qgsvectorlayer.h"
#include <QAbstractTableModel>

class FeatureExpressionValuesGatherer;

class OrderedRelationModel: public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY( QgsRelation relation READ relation WRITE setRelation NOTIFY relationChanged)
    Q_PROPERTY( QString currentRelationId READ currentRelationId WRITE setCurrentRelationId NOTIFY relationChanged)
    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged)
    Q_PROPERTY( QString orderingField READ orderingField WRITE setOrderingField NOTIFY orderingFieldChanged)
    Q_PROPERTY( QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY( QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY( QString isLoading READ isLoading NOTIFY isLoadingChanged)

  public:

    explicit OrderedRelationModel( QObject *parent = nullptr );

    enum Roles
    {
      ImagePathRole = Qt::UserRole + 1,
      DescriptionRole,
      FeatureIdRole,
    };

    QgsRelation relation() const;
    void setRelation( const QgsRelation &relation );
    QString currentRelationId() const;
    void setCurrentRelationId( const QString &relationId );
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
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;
    bool isLoading() const;

    QHash<int, QByteArray> roleNames() const override;

    void reload();
    void processFeatureList();

  signals:
    void relationChanged();
    void featureChanged();
    void orderingFieldChanged();
    void imagePathChanged();
    void descriptionChanged();
    void isLoadingChanged();


  private:

    QgsRelation mRelation;
    QString mOrderingField;
    QString mImagePath;
    QString mDescription;
    QgsFeature mFeature;
    QList<QgsFeature> mRelatedFeatures;
    bool mIsLoading = false;

    FeatureExpressionValuesGatherer *mGatherer = nullptr;
};

#endif // ORDEREDRELATIONMODEL_H
