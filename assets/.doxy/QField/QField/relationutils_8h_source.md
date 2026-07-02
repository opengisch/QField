

# File relationutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**relationutils.h**](relationutils_8h.md)

[Go to the documentation of this file](relationutils_8h.md)


```C++
/***************************************************************************
  relationutils.h - RelationUtils

 ---------------------
 begin                : 19.03.2021
 copyright            : (C) 2023 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef RELATIONUTILS_H
#define RELATIONUTILS_H

#include <QObject>
#include <qgspolymorphicrelation.h>
#include <qgsrelation.h>

class RelationUtils : public QObject
{
    Q_OBJECT

  public:
    explicit RelationUtils( QObject *parent = nullptr );

    Q_INVOKABLE QgsRelation resolveReferencingRelation( QgsProject *project, QgsVectorLayer *layer, const QString &fieldName, const QString &relationId );

    Q_INVOKABLE QgsRelation createRelation( const QgsVectorLayer *referencedLayer,
                                            const QgsVectorLayer *referencingLayer,
                                            const QVariantMap &fieldPairs );

    Q_INVOKABLE QgsPolymorphicRelation createPolymorphicRelation( const QVariantList &referencedLayers,
                                                                  const QgsVectorLayer *referencingLayer,
                                                                  const QVariantMap &fieldPairs,
                                                                  const QString &referencedLayerField,
                                                                  const QString &referencedLayerExpression );

    Q_INVOKABLE QgsPolymorphicRelation addPolymorphicRelation( QgsProject *project,
                                                               const QVariantList &referencedLayers,
                                                               const QgsVectorLayer *referencingLayer,
                                                               const QVariantMap &fieldPairs,
                                                               const QString &referencedLayerField,
                                                               const QString &referencedLayerExpression );
};

#endif // RELATIONUTILS_H
```


