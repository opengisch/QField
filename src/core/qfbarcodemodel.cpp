/***************************************************************************
 qfbarcodemodel.cpp - QfBarcodeModel

 ---------------------
 begin                : October 2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfbarcodemodel.h"

#include <QSet>

QfBarcodeModel::QfBarcodeModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

int QfBarcodeModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast<int>( mTrackedBarcodes.size() );
}

QVariant QfBarcodeModel::data( const QModelIndex &index, int role ) const
{
  const int row = index.row();
  if ( row < 0 || row >= mTrackedBarcodes.size() )
  {
    return QVariant();
  }

  switch ( role )
  {
    case BarcodeText:
      return mTrackedBarcodes.at( row ).barcode.text;

    case BarcodeBounds:
      return mTrackedBarcodes.at( row ).barcode.bounds;
  }

  return QVariant();
}

QHash<int, QByteArray> QfBarcodeModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractListModel::roleNames();
  roleNames[BarcodeText] = "BarcodeText";
  roleNames[BarcodeBounds] = "BarcodeBounds";
  return roleNames;
}

void QfBarcodeModel::updateBarcodes( const QList<Barcode> &barcodes )
{
  QSet<QString> seenTexts;

  for ( const Barcode &barcode : barcodes )
  {
    if ( seenTexts.contains( barcode.text ) )
    {
      continue;
    }
    seenTexts << barcode.text;

    const int row = rowForText( barcode.text );
    if ( row >= 0 )
    {
      mTrackedBarcodes[row] = TrackedBarcode { barcode };
      emit dataChanged( index( row ), index( row ), { BarcodeBounds } );
    }
    else
    {
      const int newRow = static_cast<int>( mTrackedBarcodes.size() );
      beginInsertRows( QModelIndex(), newRow, newRow );
      mTrackedBarcodes << TrackedBarcode { barcode };
      endInsertRows();
    }
  }

  for ( int row = static_cast<int>( mTrackedBarcodes.size() ) - 1; row >= 0; row-- )
  {
    if ( seenTexts.contains( mTrackedBarcodes.at( row ).barcode.text ) )
    {
      continue;
    }

    mTrackedBarcodes[row].missedFrameCount++;
    if ( mTrackedBarcodes.at( row ).missedFrameCount > sMaximumMissedFrameCount )
    {
      beginRemoveRows( QModelIndex(), row, row );
      mTrackedBarcodes.removeAt( row );
      endRemoveRows();
    }
  }
}

int QfBarcodeModel::rowForText( const QString &text ) const
{
  for ( int row = 0; row < mTrackedBarcodes.size(); row++ )
  {
    if ( mTrackedBarcodes.at( row ).barcode.text == text )
    {
      return row;
    }
  }

  return -1;
}

void QfBarcodeModel::clear()
{
  if ( mTrackedBarcodes.isEmpty() )
  {
    return;
  }

  beginResetModel();
  mTrackedBarcodes.clear();
  endResetModel();
}
