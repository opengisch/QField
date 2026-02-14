

# File expressionvariablemodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**expressionvariablemodel.cpp**](expressionvariablemodel_8cpp.md)

[Go to the documentation of this file](expressionvariablemodel_8cpp.md)


```C++
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
    default:
      break;
  }

  return false;
}

void ExpressionVariableModel::appendVariable( VariableScope scope, const QString &name, const QString &value, bool editable )
{
  QStandardItem *nameItem = new QStandardItem( name );
  nameItem->setData( name, VariableNameRole );
  nameItem->setData( value, VariableValueRole );
  nameItem->setData( QVariant::fromValue( scope ), VariableScopeRole );
  nameItem->setData( editable, VariableEditableRole );
  nameItem->setData( name, VariableOriginalNameRole );
  nameItem->setEditable( editable );

  appendRow( QList<QStandardItem *>() << nameItem );
}

int ExpressionVariableModel::addVariable( VariableScope scope, const QString &name, const QString &value )
{
  int lastEditableVariable = 0;
  while ( lastEditableVariable < rowCount() )
  {
    if ( !item( lastEditableVariable )->data( VariableEditableRole ).toBool() )
    {
      break;
    }
    lastEditableVariable++;
  }

  QStandardItem *nameItem = new QStandardItem( name );
  nameItem->setData( name, VariableNameRole );
  nameItem->setData( value, VariableValueRole );
  nameItem->setData( QVariant::fromValue( scope ), VariableScopeRole );
  nameItem->setData( true, VariableEditableRole );
  nameItem->setData( QString(), VariableOriginalNameRole );
  nameItem->setEditable( true );

  insertRow( lastEditableVariable, QList<QStandardItem *>() << nameItem );

  return lastEditableVariable;
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

  // First, add project variables
  QVariantMap projectVariables = ExpressionContextUtils::projectVariables( mCurrentProject );
  const QStringList projectVariableKeys = projectVariables.keys();
  for ( const QString &varName : projectVariableKeys )
  {
    QVariant varValue = projectVariables.value( varName ).toString();

    appendVariable( VariableScope::ProjectScope, varName, varValue.toString(), true );
  }

  std::unique_ptr<QgsExpressionContextScope> scope( QgsExpressionContextUtils::globalScope() );
  QStringList variableNames = scope->variableNames();
  variableNames.sort();

  // Second add user-provided global variables
  for ( const QString &name : variableNames )
  {
    if ( !scope->isReadOnly( name ) )
    {
      appendVariable( VariableScope::GlobalScope, name, scope->variable( name ).toString(), true );
    }
  }

  // Finally, add read-only global variables
  for ( const QString &name : variableNames )
  {
    if ( scope->isReadOnly( name ) )
    {
      QVariant varValue = scope->variable( name );
      if ( QString::compare( varValue.toString(), QStringLiteral( "Not available" ) ) == 0 )
        varValue = QVariant( QT_TR_NOOP( "Not Available" ) );

      appendVariable( VariableScope::GlobalScope, name, varValue.toString(), false );
    }
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
```


