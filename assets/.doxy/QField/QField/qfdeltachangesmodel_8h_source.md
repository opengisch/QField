

# File qfdeltachangesmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfdeltachangesmodel.h**](qfdeltachangesmodel_8h.md)

[Go to the documentation of this file](qfdeltachangesmodel_8h.md)


```C++
/***************************************************************************
  qfdeltachangesmodel.h - QfDeltaChangesModel

 ---------------------
 begin                : 15.8.2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFDELTACHANGESMODEL_H
#define QFDELTACHANGESMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>
#include <QPointer>
#include <qgis.h>

class QfDeltaFileWrapper;
class QJsonObject;
class QgsVectorLayer;

class QfDeltaAttributeChange
{
    Q_GADGET

    Q_PROPERTY( QString name MEMBER name )
    Q_PROPERTY( QVariant oldValue MEMBER oldValue )
    Q_PROPERTY( QVariant newValue MEMBER newValue )

  public:
    explicit QfDeltaAttributeChange( const QString &name = QString(), const QVariant &oldValue = QVariant(), const QVariant &newValue = QVariant() )
      : name( name )
      , oldValue( oldValue )
      , newValue( newValue )
    {}

    QString name;
    QVariant oldValue;
    QVariant newValue;
};

Q_DECLARE_METATYPE( QfDeltaAttributeChange )



class QfDeltaChangesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QfDeltaFileWrapper *deltaFileWrapper READ deltaFileWrapper WRITE setDeltaFileWrapper NOTIFY deltaFileWrapperChanged )

  public:
    enum ColumnRole
    {
      LayerIdRole = Qt::UserRole + 1,
      MethodRole,
      FeatureNameRole,
      AttributeChangesRole,
      HasGeometryChangeRole,
    };
    Q_ENUM( ColumnRole )

    enum DeltaMethod
    {
      UnknownMethod,
      CreateMethod,
      PatchMethod,
      DeleteMethod,
    };
    Q_ENUM( DeltaMethod )

    explicit QfDeltaChangesModel( QObject *parent = nullptr );

    QfDeltaFileWrapper *deltaFileWrapper() const;

    void setDeltaFileWrapper( QfDeltaFileWrapper *deltaFileWrapper );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role ) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QString layerName( const QString &layerId ) const;

    Q_INVOKABLE Qgis::GeometryType layerGeometryType( const QString &layerId ) const;

    Q_INVOKABLE int layerChangesCount( const QString &layerId ) const;

  signals:
    void deltaFileWrapperChanged();

  private:
    struct DeltaChange
    {
        QString layerId;
        DeltaMethod method = UnknownMethod;
        QString featureName;
        QList<QfDeltaAttributeChange> attributeChanges;
        bool hasGeometryChange = false;
    };

    struct LayerSummary
    {
        QString name;
        Qgis::GeometryType geometryType = Qgis::GeometryType::Unknown;
        QJsonArray deltas;
        QList<DeltaChange> changes;
    };

    static DeltaMethod deltaMethod( const QJsonObject &delta );

    void refresh();

    QList<DeltaChange> layerChanges( const QString &layerId, const QJsonArray &deltas, QgsVectorLayer *layer ) const;

    QHash<QString, QString> featureDisplayNames( QgsVectorLayer *layer, const QJsonArray &deltas ) const;

    QList<QfDeltaAttributeChange> attributeChanges( const QJsonObject &delta, DeltaMethod method ) const;

    QPointer<QfDeltaFileWrapper> mDeltaFileWrapper;
    QList<DeltaChange> mDeltaChanges;
    QHash<QString, LayerSummary> mLayerSummaries;
};

#endif // QFDELTACHANGESMODEL_H
```


