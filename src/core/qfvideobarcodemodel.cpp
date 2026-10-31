/***************************************************************************
 qfvideobarcodemodel.cpp - QfVideoBarcodeModel

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

#include "qfvideobarcodemodel.h"

#include <QSet>

QfVideoBarcodeModel::QfVideoBarcodeModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

int QfVideoBarcodeModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast<int>( mVisibleBarcodes.size() );
}

QVariant QfVideoBarcodeModel::data( const QModelIndex &index, int role ) const
{
  const int row = index.row();
  if ( row < 0 || row >= mVisibleBarcodes.size() )
  {
    return QVariant();
  }

  switch ( role )
  {
    case BarcodeText:
      return mVisibleBarcodes.at( row ).barcode.text;

    case BarcodeBounds:
      return mVisibleBarcodes.at( row ).barcode.bounds;

    default:
      return QVariant();
  }
}

QHash<int, QByteArray> QfVideoBarcodeModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractListModel::roleNames();
  roleNames[BarcodeText] = "BarcodeText";
  roleNames[BarcodeBounds] = "BarcodeBounds";
  return roleNames;
}

void QfVideoBarcodeModel::updateBarcodes( const QList<Barcode> &barcodes )
{
  QSet<QString> texts;

  for ( const Barcode &barcode : barcodes )
  {
    if ( texts.contains( barcode.text ) )
    {
      continue;
    }
    texts << barcode.text;

    const int row = rowForText( barcode.text );
    if ( row >= 0 )
    {
      mVisibleBarcodes[row] = TrackedBarcode( barcode );
      emit dataChanged( index( row ), index( row ), { BarcodeBounds } );
    }
    else
    {
      const int newRow = static_cast<int>( mVisibleBarcodes.size() );
      beginInsertRows( QModelIndex(), newRow, newRow );
      mVisibleBarcodes << TrackedBarcode( barcode );
      endInsertRows();
    }
  }

  for ( int row = static_cast<int>( mVisibleBarcodes.size() ) - 1; row >= 0; row-- )
  {
    if ( texts.contains( mVisibleBarcodes.at( row ).barcode.text ) )
    {
      continue;
    }

    mVisibleBarcodes[row].missedFrameCount++;
    if ( mVisibleBarcodes.at( row ).missedFrameCount > sMaximumMissedFrameCount )
    {
      beginRemoveRows( QModelIndex(), row, row );
      mVisibleBarcodes.removeAt( row );
      endRemoveRows();
    }
  }
}

int QfVideoBarcodeModel::rowForText( const QString &text ) const
{
  for ( int row = 0; row < mVisibleBarcodes.size(); row++ )
  {
    if ( mVisibleBarcodes.at( row ).barcode.text == text )
    {
      return row;
    }
  }

  return -1;
}

void QfVideoBarcodeModel::clear()
{
  if ( mVisibleBarcodes.isEmpty() )
  {
    return;
  }

  beginResetModel();
  mVisibleBarcodes.clear();
  endResetModel();
}
