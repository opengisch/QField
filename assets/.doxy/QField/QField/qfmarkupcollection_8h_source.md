

# File qfmarkupcollection.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfmarkupcollection.h**](qfmarkupcollection_8h.md)

[Go to the documentation of this file](qfmarkupcollection_8h.md)


```C++
/***************************************************************************
 qfmarkupcollection.h - QfMarkupCollection

 ---------------------
 begin                : 12.09.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFMARKUPCOLLECTION_H
#define QFMARKUPCOLLECTION_H

#include "qfmarkupitem.h"

#include <QObject>
#include <qgsannotationlayer.h>
#include <qgsvectorlayer.h>

class QfMarkupCollection : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString uuid READ uuid NOTIFY uuidChanged )
    Q_PROPERTY( QString name READ name NOTIFY nameChanged )
    Q_PROPERTY( qsizetype count READ count NOTIFY countChanged )
    Q_PROPERTY( QList<QfMarkupItem> items READ items NOTIFY itemsChanged )

  public:
    explicit QfMarkupCollection( const QString &name = QString(), QObject *parent = nullptr );

    QString uuid() const { return mUuid; }

    QString name() const { return mName; }

    void setName( const QString &name );

    qsizetype count() const { return mItems.size(); }

    QList<QfMarkupItem> items() const { return mItems.values(); }

    Q_INVOKABLE QString addItem( const QfMarkupItem &item, bool resetVectorLayer = true );

    Q_INVOKABLE void replaceItem( const QString &uuid, const QfMarkupItem &item, bool resetVectorLayer = true );

    Q_INVOKABLE void removeItem( const QString &uuid, bool resetVectorLayer = true );

    bool readGeoJson( const QString &path );

    bool writeGeoJson( const QString &path );

    QgsAnnotationLayer *asAnnotationLayer();

    QgsVectorLayer *asVectorLayer();

    Q_INVOKABLE static QfMarkupItem createItem( const QString &label, const QString &description, const QgsGeometry &geometry, const QColor &color );

  signals:
    void uuidChanged();

    void nameChanged();

    void countChanged();

    void itemsChanged();

  private:
    void processAttributeValueChanged( QgsFeatureId fid, int idx, const QVariant &value );
    void processFeatureDeleted( QgsFeatureId fid );
    void processGeometryChanged( QgsFeatureId fid, const QgsGeometry &geometry );

    QString mUuid;
    QString mName;
    QMap<QString, QfMarkupItem> mItems;
    std::unique_ptr<QgsAnnotationLayer> mAnnotationLayer;
    std::unique_ptr<QgsVectorLayer> mVectorLayer;
    QMap<QgsFeatureId, QString> mFeatureUuids;
};

#endif // QFMARKUPCOLLECTION_H
```


