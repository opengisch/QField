

# File qfprintlayoutlistmodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfprintlayoutlistmodel.cpp**](qfprintlayoutlistmodel_8cpp.md)

[Go to the documentation of this file](qfprintlayoutlistmodel_8cpp.md)


```C++
/***************************************************************************
  qfprintlayoutlistmodel.cpp
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

#include "qfprintlayoutlistmodel.h"

#include <qgslayoutatlas.h>
#include <qgsproject.h>

QfPrintLayoutListModel::QfPrintLayoutListModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

QHash<int, QByteArray> QfPrintLayoutListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[TitleRow] = "Title";
  roles[IndexRow] = "Index";

  return roles;
}

QgsProject *QfPrintLayoutListModel::project() const
{
  return mProject;
}

void QfPrintLayoutListModel::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  mProject = project;
  emit projectChanged();
}

QgsVectorLayer *QfPrintLayoutListModel::atlasCoverageLayer() const
{
  return mAtlasCoverageLayr;
}

void QfPrintLayoutListModel::setAtlasCoverageLayer( QgsVectorLayer *layer )
{
  if ( mAtlasCoverageLayr == layer )
    return;

  mAtlasCoverageLayr = layer;
  emit atlasCoverageLayerChanged();

  reloadModel();
}

void QfPrintLayoutListModel::reloadModel()
{
  beginResetModel();
  mPrintLayouts.clear();

  const QList<QgsPrintLayout *> layouts( mProject->layoutManager()->printLayouts() );
  for ( const auto &layout : layouts )
  {
    if ( mAtlasCoverageLayr )
    {
      if ( layout->atlas() )
      {
        if ( layout->atlas()->coverageLayer() == mAtlasCoverageLayr )
        {
          mPrintLayouts.append( PrintLayout( layout->name() ) );
        }
      }
    }
    else
    {
      mPrintLayouts.append( PrintLayout( layout->name() ) );
    }
  }
  endResetModel();
}

int QfPrintLayoutListModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mPrintLayouts.size() ) : 0;
}

QVariant QfPrintLayoutListModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mPrintLayouts.size() || index.row() < 0 )
    return QVariant();

  if ( role == TitleRow )
    return mPrintLayouts.at( index.row() ).title;
  else if ( role == IndexRow )
    return index.row();

  return QVariant();
}

const QString QfPrintLayoutListModel::titleAt( int row ) const
{
  if ( row < 0 || row >= mPrintLayouts.size() )
    return QString();

  return mPrintLayouts.at( row ).title;
}
```


