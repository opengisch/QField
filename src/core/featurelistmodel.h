/***************************************************************************
  featurelistmodel.h - FeatureListModel

 ---------------------
 begin                : 1.2.2017
 copyright            : (C) 2017 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef FEATURELISTMODEL_H
#define FEATURELISTMODEL_H

#include <QAbstractItemModel>
#include <QTimer>

class QgsVectorLayer;

/**
 * Provides access to a list of features from a layer.
 * For each feature, the display expression is exposed as DisplayRole
 * and a keyField as KeyFieldRole for a unique identifier.
 * If a displayValueField is set it replaces the display expression of the layer.
 */
class FeatureListModel : public QAbstractItemModel
{
    Q_OBJECT

    /**
     * The vector layer to list
     */
    Q_PROPERTY( QgsVectorLayer *currentLayer READ currentLayer WRITE setCurrentLayer NOTIFY currentLayerChanged )
    /**
     * The primary key field
     */
    Q_PROPERTY( QString keyField READ keyField WRITE setKeyField NOTIFY keyFieldChanged )
    /**
     * The display value field
     */
    Q_PROPERTY( QString displayValueField READ displayValueField WRITE setDisplayValueField NOTIFY displayValueFieldChanged )

    Q_PROPERTY( bool orderByValue READ orderByValue WRITE setOrderByValue NOTIFY orderByValueChanged )

    Q_PROPERTY( bool addNull READ addNull WRITE setAddNull NOTIFY addNullChanged )

  public:
    enum FeatureListRoles
    {
      KeyFieldRole = Qt::UserRole + 1
    };

    Q_ENUM( FeatureListRoles )

    FeatureListModel();

    virtual QModelIndex index( int row, int column, const QModelIndex &parent ) const override;
    virtual QModelIndex parent( const QModelIndex &child ) const override;
    virtual int rowCount( const QModelIndex &parent ) const override;
    virtual int columnCount( const QModelIndex &parent ) const override;
    virtual QVariant data( const QModelIndex &index, int role ) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    QgsVectorLayer *currentLayer() const;
    void setCurrentLayer( QgsVectorLayer *currentLayer );

    QString keyField() const;
    void setKeyField( const QString &keyField );

    QString displayValueField() const;
    void setDisplayValueField( const QString &displayValueField );

    /**
     * Get the row for a given key value.
     */
    Q_INVOKABLE int findKey( const QVariant &key ) const;

    /**
     * Orders all the values alphabethically by their displayString.
     */
    bool orderByValue() const;

    /**
     * Orders all the values alphabethically by their displayString.
     */
    void setOrderByValue( bool orderByValue );

    /**
     * Add a NULL value as the first entry.
     */
    bool addNull() const;

    /**
     * Add a NULL value as the first entry.
     */
    void setAddNull( bool addNull );

  signals:
    void currentLayerChanged();
    void keyFieldChanged();
    void displayValueFieldChanged();
    void addNullChanged();
    void orderByValueChanged();

  private slots:
    void onFeatureAdded();
    void onFeatureDeleted();
    /**
     * Reloads a layer. This will normally be triggered
     * by \see reloadLayer and should not be called directly.
     */
    void processReloadLayer();

  private:
    struct Entry
    {
      Entry( const QString &displayString, const QVariant &key )
        : displayString( displayString )
        , key( key )
      {}

      Entry() = default;

      QString displayString;
      QVariant key;
    };

    /**
     * Triggers a reload of the values from the layer.
     * To avoid having the (expensive) reload operation happening for
     * every property change, it will only execute this after a very short delay.
     * This allows changing multiple properties at once and have a single reload
     * in the end.
     */
    void reloadLayer();

    QgsVectorLayer *mCurrentLayer;

    QList<Entry> mEntries;
    QString mKeyField;
    QString mDisplayValueField;
    bool mOrderByValue = false;
    bool mAddNull = false;

    QTimer mReloadTimer;
};

#endif // FEATURELISTMODEL_H
