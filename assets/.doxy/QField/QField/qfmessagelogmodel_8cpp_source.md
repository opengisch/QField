

# File qfmessagelogmodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfmessagelogmodel.cpp**](qfmessagelogmodel_8cpp.md)

[Go to the documentation of this file](qfmessagelogmodel_8cpp.md)


```C++
/***************************************************************************
  qfmessagelogmodel.cpp - QfMessageLogModel

 ---------------------
 begin                : 13.7.2016
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
#include "qfmessagelogmodel.h"

#include <QDebug>
#include <qgsapplication.h>

QfMessageLogModel::QfMessageLogModel( QObject *parent )
  : QAbstractListModel( parent )
  , mMessageLog( QgsApplication::messageLog() )
{
  connect( mMessageLog, static_cast<void ( QgsMessageLog::* )( const QString &message, const QString &tag, Qgis::MessageLevel level, Qgis::StringFormat format )>( &QgsMessageLog::messageReceivedWithFormat ), this, &QfMessageLogModel::onMessageReceived );
}

QHash<int, QByteArray> QfMessageLogModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[MessageRole] = "Message";
  roles[MessageTagRole] = "MessageTag";
  roles[MessageLevelRole] = "MessageLevel";
  roles[MessageDateTimeRole] = "MessageDateTime";

  return roles;
}

int QfMessageLogModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return static_cast<int>( mMessages.size() );
}

QVariant QfMessageLogModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mMessages.size() )
    return QVariant();

  if ( role == MessageRole )
    return mMessages.at( index.row() ).message;
  else if ( role == MessageTagRole )
    return mMessages.at( index.row() ).tag;
  else if ( role == MessageLevelRole )
    return mMessages.at( index.row() ).level;
  else if ( role == MessageDateTimeRole )
    return mMessages.at( index.row() ).datetime;

  return QVariant();
}

void QfMessageLogModel::suppress( const QVariantMap &filters )
{
  for ( const QString &tags : filters.keys() )
  {
    if ( mSuppressedFilters.contains( tags ) )
    {
      for ( const QVariant &filter : filters[tags].toList() )
      {
        if ( !mSuppressedFilters[tags].contains( filter.toString() ) )
        {
          mSuppressedFilters[tags].push_back( filter.toString() );
        }
      }
    }
    else
    {
      mSuppressedFilters[tags] = filters[tags].toStringList();
    }
  }
}

void QfMessageLogModel::unsuppress( const QVariantMap &filters )
{
  for ( const QString &tags : filters.keys() )
  {
    if ( mSuppressedFilters.contains( tags ) )
    {
      if ( filters[tags].toList().isEmpty() )
      {
        mSuppressedFilters.remove( tags );
        continue;
      }
      for ( const QVariant &filter : filters[tags].toList() )
      {
        mSuppressedFilters[tags].removeAll( filter.toString() );
      }
    }
  }
}

void QfMessageLogModel::clear()
{
  beginResetModel();
  mMessages.clear();
  endResetModel();
}

void QfMessageLogModel::onMessageReceived( const QString &message, const QString &tag, Qgis::MessageLevel level, Qgis::StringFormat format )
{
  if ( tag == QLatin1String( "3D" ) )
  {
    return;
  }
  else if ( mSuppressedFilters.contains( tag ) )
  {
    if ( std::any_of( mSuppressedFilters[tag].begin(), mSuppressedFilters[tag].end(), [&message]( const QString &filter ) { return message.contains( filter ); } ) )
    {
      return;
    }
  }

  beginInsertRows( QModelIndex(), 0, 0 );
  mMessages.prepend( LogMessage( tag, message, level ) );
  if ( tag == QLatin1String( "Plugin Manager" ) )
  {
    // Make plugin-related errors extra visible to the console
    qInfo() << QStringLiteral( "%1: %2" ).arg( tag, message );
  }
  else
  {
    qDebug() << QStringLiteral( "%1: %2" ).arg( tag, message );
  }
  endInsertRows();
}
```


