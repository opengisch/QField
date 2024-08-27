/***************************************************************************
  expressionvariablemodel.cpp - ExpressionVariableModel

 ---------------------
 begin                : 29.9.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "expressionvariablemodel.h"
#include "utils/expressioncontextutils.h"

#include <QDebug>
#include <QSettings>
#include <qgsexpressioncontext.h>
#include <qgsexpressioncontextutils.h>

ExpressionVariableModel::ExpressionVariableModel( QObject *parent )
  : QStandardItemModel( parent )
{
  reloadVariables();

  connect( this, &QStandardItemModel::dataChanged, this, &ExpressionVariableModel::onDataChanged );
}

bool ExpressionVariableModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  QStandardItem *rowItem = item( index.row() );
  if ( !rowItem || !rowItem->data( VariableEditableRole ).toBool() )
  {
    return false;
  }

  switch ( role )
  {
    case VariableNameRole:
      if ( rowItem->data( VariableNameRole ) == value )
      {
        return false;
      }

      rowItem->setData( value, VariableNameRole );
      return true;

    case VariableValueRole:

      if ( rowItem->data( VariableValueRole ) == value )
      {
        return false;
      }

      rowItem->setData( value, VariableValueRole );
      return true;

    case VariableEditableRole:
    case VariableScopeRole:
      break;
  }

  return false;
}

int ExpressionVariableModel::addVariable( VariableScope scope, const QString &name, const QString &value, bool editable, bool preexisting )
{
  int lastVariableInScope = rowCount();
  for ( int i = 0; i < rowCount(); ++i )
  {
    if ( item( i )->data( VariableScopeRole ).value<VariableScope>() == scope )
    {
      lastVariableInScope = i + 1;
    }
  }

  QStandardItem *nameItem = new QStandardItem( name );
  nameItem->setData( name, VariableNameRole );
  nameItem->setData( value, VariableValueRole );
  nameItem->setData( QVariant::fromValue( scope ), VariableScopeRole );
  nameItem->setData( editable, VariableEditableRole );
  nameItem->setData( preexisting ? name : QString(), VariableOriginalNameRole );
  nameItem->setEditable( editable );

  insertRow( lastVariableInScope, QList<QStandardItem *>() << nameItem );

  return lastVariableInScope;
}

void ExpressionVariableModel::removeVariable( VariableScope scope, const QString &name )
{
  for ( int i = 0; i < rowCount(); ++i )
  {
    const QStandardItem *rowItem = item( i );
    const QString variableName = rowItem->data( VariableNameRole ).toString();
    const VariableScope variableScope = rowItem->data( VariableScopeRole ).value<VariableScope>();

    if ( variableName == name && variableScope == scope )
    {
      mRemovedVariables << QPair<VariableScope, QString>( variableScope, variableName );
      removeRow( i );
      return;
    }
  }
}

void ExpressionVariableModel::save()
{
  for ( const QPair<VariableScope, QString> &variable : mRemovedVariables )
  {
    if ( variable.first == VariableScope::GlobalScope )
    {
      QgsExpressionContextUtils::removeGlobalVariable( variable.second );
    }
  }

  for ( int i = 0; i < rowCount(); ++i )
  {
    const QStandardItem *currentItem = item( i );
    const VariableScope itemScope = currentItem->data( VariableScopeRole ).value<VariableScope>();
    const QString itemName = currentItem->data( VariableNameRole ).toString();
    const QString itemOriginalName = currentItem->data( VariableOriginalNameRole ).toString();
    const QString itemValue = currentItem->data( VariableValueRole ).toString();

    if ( currentItem->isEditable() && itemScope == VariableScope::GlobalScope )
    {
      if ( !itemOriginalName.isEmpty() && itemName != itemOriginalName )
      {
        // Remove renamed variable
        QgsExpressionContextUtils::removeGlobalVariable( itemOriginalName );
      }
      QgsExpressionContextUtils::setGlobalVariable( itemName, itemValue );
    }
    else if ( itemScope == VariableScope::ProjectScope )
    {
      ExpressionContextUtils::setProjectVariable( mCurrentProject, itemName, itemValue );
    }
  }
}

void ExpressionVariableModel::reloadVariables()
{
  clear();

  mRemovedVariables.clear();

  std::unique_ptr<QgsExpressionContextScope> scope( QgsExpressionContextUtils::globalScope() );
  QStringList variableNames = scope->variableNames();
  variableNames.sort();

  // First add readonly app variables
  for ( const QString &varName : variableNames )
  {
    if ( scope->isReadOnly( varName ) )
    {
      QVariant varValue = scope->variable( varName );
      if ( QString::compare( varValue.toString(), QStringLiteral( "Not available" ) ) == 0 )
        varValue = QVariant( QT_TR_NOOP( "Not Available" ) );

      addVariable( VariableScope::GlobalScope, varName, varValue.toString(), false, true );
    }
  }
  // Second add custom variables
  for ( const QString &varName : variableNames )
  {
    if ( !scope->isReadOnly( varName ) )
    {
      addVariable( VariableScope::GlobalScope, varName, scope->variable( varName ).toString(), true, true );
    }
  }
  // Finally add readonly project variables
  QVariantMap projectVariables = ExpressionContextUtils::projectVariables( mCurrentProject );
  const QStringList projectVariableKeys = projectVariables.keys();
  for ( const QString &varName : projectVariableKeys )
  {
    QVariant varValue = projectVariables.value( varName ).toString();

    addVariable( VariableScope::ProjectScope, varName, varValue.toString(), true, true );
  }
}


QHash<int, QByteArray> ExpressionVariableModel::roleNames() const
{
  QHash<int, QByteArray> names = QStandardItemModel::roleNames();
  names[VariableNameRole] = "VariableName";
  names[VariableValueRole] = "VariableValue";
  names[VariableScopeRole] = "VariableScope";
  names[VariableEditableRole] = "VariableEditable";
  names[VariableOriginalNameRole] = "VariableOriginalName";
  return names;
}

void ExpressionVariableModel::onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  Q_UNUSED( bottomRight )
  Q_UNUSED( roles )
}

QgsProject *ExpressionVariableModel::currentProject() const
{
  return mCurrentProject;
}

void ExpressionVariableModel::setCurrentProject( QgsProject *project )
{
  if ( mCurrentProject == project )
    return;
  mCurrentProject = project;
  reloadVariables();
  emit currentProjectChanged();
}
