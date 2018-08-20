/***************************************************************************
  printlayoutlistmodel.cpp
 ---------------------
 begin                : 20.8.2018
 copyright            : (C) 2018 by david
 email                : david at opengisch dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "printlayoutlistmodel.h"
#include <qgsproject.h>

PrintLayoutListModel::PrintLayoutListModel(QObject* parent)
  : QAbstractListModel( parent )
{
}

QHash<int, QByteArray> PrintLayoutListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[TitleRow]  = "Title";
  roles[IndexRow] = "Index";

  return roles;
}

QgsProject* PrintLayoutListModel::project() const
{
  return mProject;
}

void PrintLayoutListModel::setProject( QgsProject* project )
{
  mProject = project;

  reloadModel();
}

void PrintLayoutListModel::reloadModel()
{
  beginResetModel();
  mPrintLayouts.clear();

  for ( const auto &layout : mProject->layoutManager()->printLayouts() )
  {
    mPrintLayouts.append( PrintLayout( layout->name() ) );
  }
  endResetModel();

}

int PrintLayoutListModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED( parent )
  int test = mPrintLayouts.size();
  return test;
}

QVariant PrintLayoutListModel::data( const QModelIndex& index, int role ) const
{
  if ( index.row() >= mPrintLayouts.size() )
    return QVariant();

  if ( role == TitleRow )
    return mPrintLayouts.at( index.row() ).title;
  else if ( role == IndexRow )
    return  index.row();

  return QVariant();
}
