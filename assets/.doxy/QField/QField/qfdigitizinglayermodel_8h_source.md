

# File qfdigitizinglayermodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfdigitizinglayermodel.h**](qfdigitizinglayermodel_8h.md)

[Go to the documentation of this file](qfdigitizinglayermodel_8h.md)


```C++
/***************************************************************************
  qfdigitizinglayermodel.h

 ---------------------
 begin                : 18.09.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFDIGITIZINGLAYERMODEL_H
#define QFDIGITIZINGLAYERMODEL_H

#include "qfmarkupmanager.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <qgsproject.h>

class QfDigitizingLayerBaseModel;


class QfDigitizingLayerModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )

    
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    Q_PROPERTY( QfMarkupManager *markups READ markupManager WRITE setMarkupManager NOTIFY markupManagerChanged )

  public:
    enum Role
    {
      IdRole = Qt::UserRole + 1,
      NameRole,
      LayerTypeRole,
      LayerRole,
      GeometryTypeRole,
    };
    Q_ENUM( Role )

    enum LayerType
    {
      MapLayer = 1,
      MarkupCollection
    };
    Q_ENUM( LayerType )

    
    explicit QfDigitizingLayerModel( QObject *parent = nullptr );

    bool enabled() const;
    void setEnabled( bool enabled );

    QgsProject *project() const;
    void setProject( QgsProject *project );

    QfMarkupManager *markupManager() const;
    void setMarkupManager( QfMarkupManager *markupManager );

    Q_INVOKABLE int findLayer( QgsMapLayer *layer ) const;

    Q_INVOKABLE int findLayerName( const QString &name ) const;

    Q_INVOKABLE int findCollection( QfMarkupCollection *collection ) const;

    Q_INVOKABLE int findCollectionName( const QString &name ) const;

    Q_INVOKABLE QVariantMap get( int row ) const;

    bool lessThan( const QModelIndex &left, const QModelIndex &right ) const override;

  signals:
    void enabledChanged();
    void projectChanged();
    void markupManagerChanged();

  private:
    QfDigitizingLayerBaseModel *mModel = nullptr;
};


class QfDigitizingLayerBaseModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit QfDigitizingLayerBaseModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool enabled() const { return mEnabled; }
    void setEnabled( bool enabled );

    QgsProject *project() const { return mProject; }
    void setProject( QgsProject *project );

    QfMarkupManager *markupManager() const { return mMarkupManager; }
    void setMarkupManager( QfMarkupManager *markupManager );

    int findLayer( QgsMapLayer *layer ) const;

    int findLayerName( const QString &name ) const;

    int findCollection( QfMarkupCollection *collection ) const;

    int findCollectionName( const QString &name ) const;

  signals:
    void enabledChanged();
    void projectChanged();
    void markupManagerChanged();

  protected slots:
    void removeLayers( const QStringList &layerIds );
    void addLayers( const QList<QgsMapLayer *> &layers );

    void removeCollections( const QStringList &collectionUuids );
    void addCollections( const QStringList &collectionUuids );

  private:
    struct DigitizingLayer
    {
        QfDigitizingLayerModel::LayerType type = QfDigitizingLayerModel::MapLayer;
        QString name;
        QgsMapLayer *mapLayer = nullptr;
        QfMarkupCollection *markupCollection = nullptr;
    };

    void resetModel();

    bool mEnabled = true;
    QList<DigitizingLayer> mLayers;
    QgsProject *mProject = nullptr;
    QfMarkupManager *mMarkupManager = nullptr;
};

#endif // QFDIGITIZINGLAYERMODEL_H
```


