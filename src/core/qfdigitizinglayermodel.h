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


/**
 * \brief A digitizing layers (map layers and markup collections) model.
 * \ingroup core
 */
class QfDigitizingLayerModel : public QSortFilterProxyModel
{
    Q_OBJECT

    /**
     * The enabled state of the model.
     * \note When disabled, any changes to the project's map layers registry and markup manager will be ignored
     */
    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )

    //! The project from which map layers will be taken from.
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    //! The markup manager from which makrup collections will be taken from.
    Q_PROPERTY( QfMarkupManager *markupManager READ markupManager WRITE setMarkupManager NOTIFY markupManagerChanged )

  public:
    enum Role
    {
      IdRole = Qt::UserRole + 1,
      TypeRole,
      NameRole,
      CollectionRole,
      LayerRole,
      LayerTypeRole,
      GeometryTypeRole,
    };
    Q_ENUM( Role )

    enum Type
    {
      MapLayer = 1,
      MarkupCollection
    };
    Q_ENUM( Type )

    //! The constructor
    explicit QfDigitizingLayerModel( QObject *parent = nullptr );

    //! \copydoc QfDigitizingLayerModel::enabled
    bool enabled() const;
    //! \copydoc QfDigitizingLayerModel::enabled
    void setEnabled( bool enabled );

    //! \copydoc QfDigitizingLayerModel::project
    QgsProject *project() const;
    //! \copydoc QfDigitizingLayerModel::project
    void setProject( QgsProject *project );

    //! \copydoc QfDigitizingLayerModel::markupManager
    QfMarkupManager *markupManager() const;
    //! \copydoc QfDigitizingLayerModel::markupManager
    void setMarkupManager( QfMarkupManager *markupManager );

    /**
     * Returns the model row for given map \a layer.
     * \note If the map layer is not found, -1 will be returned
     */
    Q_INVOKABLE int findLayer( QgsMapLayer *layer ) const;

    /**
     * Returns the model row for given map layer \a name.
     * \note If the map layer name is not found, -1 will be returned
     */
    Q_INVOKABLE int findLayerName( const QString &name ) const;

    /**
     * Returns the model row for given markup \a collection.
     * \note If the collection is not found, -1 will be returned
     */
    Q_INVOKABLE int findCollection( QfMarkupCollection *collection ) const;

    /**
     * Returns the model row for given markup collection \a name.
     * \note If the collection name is not found, -1 will be returned
     */
    Q_INVOKABLE int findCollectionName( const QString &name ) const;

    /**
     * Returns a model data map for a given \a row.
     */
    Q_INVOKABLE QVariantMap get( int row ) const;

    bool lessThan( const QModelIndex &left, const QModelIndex &right ) const override;

  signals:
    void enabledChanged();
    void projectChanged();
    void markupManagerChanged();

  private:
    QfDigitizingLayerBaseModel *mModel = nullptr;
};


/**
 * \brief A digitizing layers (map layers and markup collections) base model.
 * \ingroup core
 */
class QfDigitizingLayerBaseModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit QfDigitizingLayerBaseModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    //! \copydoc QfDigitizingLayerModel::enabled
    bool enabled() const { return mEnabled; }
    //! \copydoc QfDigitizingLayerModel::enabled
    void setEnabled( bool enabled );

    //! \copydoc QfDigitizingLayerModel::project
    QgsProject *project() const { return mProject; }
    //! \copydoc QfDigitizingLayerModel::project
    void setProject( QgsProject *project );

    //! \copydoc QfDigitizingLayerModel::markupManager
    QfMarkupManager *markupManager() const { return mMarkupManager; }
    //! \copydoc QfDigitizingLayerModel::markupManager
    void setMarkupManager( QfMarkupManager *markupManager );

    //! \copydoc QfDigitizingLayerModel::findLayer
    int findLayer( QgsMapLayer *layer ) const;

    //! \copydoc QfDigitizingLayerModel::findLayerName
    int findLayerName( const QString &name ) const;

    //! \copydoc QfDigitizingLayerModel::findCollection
    int findCollection( QfMarkupCollection *collection ) const;

    //! \copydoc QfDigitizingLayerModel::findCollectionName
    int findCollectionName( const QString &name ) const;

  signals:
    void enabledChanged();
    void projectChanged();
    void markupManagerChanged();

  protected slots:
    void removeLayers( const QStringList &layerIds );
    void addLayers( const QList<QgsMapLayer *> &layers );

    void removeCollections( const QStringList &collectionUuids );
    void addCollections( const QList<QfMarkupCollection *> &collections );

  private:
    struct DigitizingLayer
    {
        QfDigitizingLayerModel::Type type = QfDigitizingLayerModel::MapLayer;
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
