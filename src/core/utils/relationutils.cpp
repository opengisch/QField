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

QgsRelation RelationUtils::createRelation( const QgsVectorLayer *referencedLayer, const QgsVectorLayer *referencingLayer, const QVariantMap &fieldPairs )
{
  if ( !referencedLayer || !referencingLayer || fieldPairs.isEmpty() )
    return QgsRelation();

  QStringList fieldPairStrings;
  for ( QVariantMap::const_iterator it = fieldPairs.constBegin(); it != fieldPairs.constEnd(); ++it )
  {
    fieldPairStrings.append( QStringLiteral( "%1_%2" ).arg( it.key(), it.value().toString() ) );
  }
  const QString relationId = QStringLiteral( "%1_%2_%3" ).arg( referencedLayer->id(), fieldPairStrings.join( "_" ), referencingLayer->id() );

  QgsRelation relation;
  relation.setId( relationId );
  relation.setName( QStringLiteral( "%1 (%2) <-> %3 (%4)" ).arg( referencedLayer->name(), fieldPairs.constBegin().key(), referencingLayer->name(), fieldPairs.constBegin().value().toString() ) );
  relation.setReferencedLayer( referencedLayer->id() );
  relation.setReferencingLayer( referencingLayer->id() );

  for ( QVariantMap::const_iterator it = fieldPairs.constBegin(); it != fieldPairs.constEnd(); ++it )
  {
    relation.addFieldPair( it.value().toString(), it.key() );
  }
  return relation;
}
