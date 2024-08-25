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
  return QStandardItemModel::setData( index, value, role );
}

int ExpressionVariableModel::addVariable( VariableScope scope, const QString &name, const QString &value, bool editable )
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
  nameItem->setData( name, VariableName );
  nameItem->setData( value, VariableValue );
  nameItem->setData( QVariant::fromValue( scope ), VariableScopeRole );
  nameItem->setData( editable, VariableEditable );
  nameItem->setEditable( editable );

  insertRow( lastVariableInScope, QList<QStandardItem *>() << nameItem );

  return lastVariableInScope;
}

void ExpressionVariableModel::removeVariable( VariableScope scope, const QString &name )
{
  for ( int i = 0; i < rowCount(); ++i )
  {
    QStandardItem *rowItem = item( i );
    QString variableName = rowItem->data( VariableName ).toString();
    VariableScope variableScope = rowItem->data( VariableScopeRole ).value<VariableScope>();

    if ( variableName == name && variableScope == scope )
    {
      removeRow( i );
      return;
    }
  }
}

void ExpressionVariableModel::save()
{
  QVariantMap variables;
  for ( int i = 0; i < rowCount(); ++i )
  {
    if ( item( i )->isEditable() )
    {
      variables.insert( item( i )->data( VariableName ).toString(), item( i )->data( VariableValue ) );
    }
  }

  QgsExpressionContextUtils::setGlobalVariables( variables );
}

void ExpressionVariableModel::reloadVariables()
{
  clear();

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

      addVariable( VariableScope::ApplicationScope, varName, varValue.toString(), false );
    }
  }
  // Second add custom variables
  for ( const QString &varName : variableNames )
  {
    if ( !scope->isReadOnly( varName ) )
    {
      addVariable( VariableScope::ApplicationScope, varName, scope->variable( varName ).toString(), true );
    }
  }
  // Finally add readonly project variables
  QVariantMap projectVariables = ExpressionContextUtils::projectVariables( mCurrentProject );
  for ( const QString &varName : projectVariables.keys() )
  {
    QVariant varValue = projectVariables.value( varName ).toString();

    addVariable( VariableScope::ProjectScope, varName, varValue.toString(), false );
  }
}

void ExpressionVariableModel::setName( int row, const QString &name )
{
  QStandardItem *rowItem = item( row );

  if ( !rowItem )
    return;

  if ( rowItem->data( VariableName ).toString() == name )
    return;

  rowItem->setData( name, VariableName );
}

void ExpressionVariableModel::setValue( int row, const QString &value )
{
  QStandardItem *rowItem = item( row );

  if ( !rowItem )
    return;

  if ( rowItem->data( VariableValue ).toString() == value )
    return;

  rowItem->setData( value, VariableValue );
}

QHash<int, QByteArray> ExpressionVariableModel::roleNames() const
{
  QHash<int, QByteArray> names = QStandardItemModel::roleNames();
  names[VariableName] = "VariableName";
  names[VariableValue] = "VariableValue";
  names[VariableScopeRole] = "VariableScope";
  names[VariableEditable] = "VariableEditable";
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
