

# File messagelogmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**messagelogmodel.cpp**](messagelogmodel_8cpp.md)

[Go to the documentation of this file](messagelogmodel_8cpp.md)


```C++
/***************************************************************************
  messagelogmodel.cpp - MessageLogModel

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
#include "messagelogmodel.h"

#include <QDebug>
#include <qgsapplication.h>

MessageLogModel::MessageLogModel( QObject *parent )
  : QAbstractListModel( parent )
  , mMessageLog( QgsApplication::messageLog() )
{
  connect( mMessageLog, static_cast<void ( QgsMessageLog::* )( const QString &message, const QString &tag, Qgis::MessageLevel level )>( &QgsMessageLog::messageReceived ), this, &MessageLogModel::onMessageReceived );
}

QHash<int, QByteArray> MessageLogModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[MessageRole] = "Message";
  roles[MessageTagRole] = "MessageTag";
  roles[MessageLevelRole] = "MessageLevel";
  roles[MessageDateTimeRole] = "MessageDateTime";

  return roles;
}

int MessageLogModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return static_cast<int>( mMessages.size() );
}

QVariant MessageLogModel::data( const QModelIndex &index, int role ) const
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

void MessageLogModel::suppress( const QVariantMap &filters )
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

void MessageLogModel::unsuppress( const QVariantMap &filters )
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

void MessageLogModel::clear()
{
  beginResetModel();
  mMessages.clear();
  endResetModel();
}

void MessageLogModel::onMessageReceived( const QString &message, const QString &tag, Qgis::MessageLevel level )
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


