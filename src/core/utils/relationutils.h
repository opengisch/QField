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
     * \param referencingLayer The referencing (child) layer
     * \param fieldPairs A map where keys are referenced field names and values are referencing field names
     * \returns A QgsRelation object
     */
    Q_INVOKABLE QgsRelation createRelation( const QgsVectorLayer *referencedLayer,
                                            const QgsVectorLayer *referencingLayer,
                                            const QVariantMap &fieldPairs );

    /**
     * Creates a polymorphic relation between multiple referenced layers and a referencing layer.
     * Note: Layers must be added to the project first
     *
     * \param referencedLayers The list of referenced (parent) layers
     * \param referencingLayer The referencing (child) layer
     * \param fieldPairs A map where keys are referenced field names and values are referencing field names
     * \param referencedLayerField The field name in the child layer that stores the parent layer name
     * \param referencedLayerExpression The expression to identify the parent layer name (e.g., "@layer_name")
     * \returns A QgsPolymorphicRelation object
     */
    Q_INVOKABLE QgsPolymorphicRelation createPolymorphicRelation( const QVariantList &referencedLayers,
                                                                  const QgsVectorLayer *referencingLayer,
                                                                  const QVariantMap &fieldPairs,
                                                                  const QString &referencedLayerField,
                                                                  const QString &referencedLayerExpression );

    /**
     * Creates and adds a polymorphic relation to the project.
     * This is a convenience function that creates the relation and adds it to the project's relation manager.
     *
     * \param project The project to add the relation to
     * \param referencedLayers The list of referenced (parent) layers
     * \param referencingLayer The referencing (child) layer
     * \param fieldPairs A map where keys are referenced field names and values are referencing field names
     * \param referencedLayerField The field name in the child layer that stores the parent layer name
     * \param referencedLayerExpression The expression to identify the parent layer name (e.g., "@layer_name")
     * \returns A QgsPolymorphicRelation object
     */
    Q_INVOKABLE QgsPolymorphicRelation addPolymorphicRelation( QgsProject *project,
                                                               const QVariantList &referencedLayers,
                                                               const QgsVectorLayer *referencingLayer,
                                                               const QVariantMap &fieldPairs,
                                                               const QString &referencedLayerField,
                                                               const QString &referencedLayerExpression );
};

#endif // RELATIONUTILS_H
