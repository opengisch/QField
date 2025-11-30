/***************************************************************************
  relationutils.cpp - RelationUtils

 ---------------------
 begin                : 19.03.2023
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

#include "relationutils.h"

#include <qgsproject.h>
#include <qgsrelationmanager.h>
#include <qgsvectorlayer.h>

RelationUtils::RelationUtils( QObject *parent )
  : QObject( parent )
{
}

QgsRelation RelationUtils::resolveReferencingRelation( QgsProject *project, QgsVectorLayer *layer, const QString &fieldName, const QString &relationId )
{
  if ( !project )
    return QgsRelation();

  QgsRelationManager *relationManager = project->relationManager();
  QgsRelation relation = relationManager->relation( relationId );
  if ( relation.isValid() )
    return relation;
  if ( !layer )
    return relation;

  // In case we don't find the relation by id, fall back to any suitable relation, that's how QGIS does it too
  int index = layer->fields().indexFromName( fieldName );
  QList<QgsRelation> relations = relationManager->referencingRelations( layer, index );
  if ( !relations.empty() )
    return relations.first();
  return QgsRelation();
}

QgsRelation RelationUtils::createRelation( const QgsProject *project, const QgsVectorLayer *parentLayer, const QString &parentFieldName, const QgsVectorLayer *childLayer, const QString &childFieldName )
{
  if ( !project || !parentLayer || !childLayer )
    return QgsRelation();

  const QString relationId = QStringLiteral( "%1_%2_%3_%4" ).arg( parentLayer->name(), parentFieldName, childLayer->name(), childFieldName );

  QgsRelation relation;
  relation.setId( relationId );
  relation.setName( relationId );
  relation.setReferencedLayer( parentLayer->id() );
  relation.setReferencingLayer( childLayer->id() );
  relation.addFieldPair( childFieldName, parentFieldName );

  return relation;
}
