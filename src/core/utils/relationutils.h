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
#include <qgsrelation.h>

/**
 * \ingroup core
 */
class RelationUtils : public QObject
{
    Q_OBJECT

  public:
    explicit RelationUtils( QObject *parent = nullptr );

    Q_INVOKABLE QgsRelation resolveReferencingRelation( QgsProject *project, QgsVectorLayer *layer, const QString &fieldName, const QString &relationId );

    /**
     * Creates a relation between referenced and referencing layers.
     * Note: Layers must be added to the project first
     *
     * \param referencedLayer The referenced (parent) layer
     * \param referencedFieldName The field name in the referenced layer
     * \param referencingLayer The referencing (child) layer
     * \param referencingFieldName The field name in the referencing layer that references the parent
     * \returns A QgsRelation object
     */
    Q_INVOKABLE QgsRelation createRelation( const QgsVectorLayer *referencedLayer, const QString &referencedFieldName,
                                            const QgsVectorLayer *referencingLayer, const QString &referencingFieldName );
};

#endif // RELATIONUTILS_H
