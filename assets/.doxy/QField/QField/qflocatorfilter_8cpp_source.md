

# File qflocatorfilter.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**qflocatorfilter.cpp**](qflocatorfilter_8cpp.md)

[Go to the documentation of this file](qflocatorfilter_8cpp.md)


```C++
/***************************************************************************
  qflocatorfilter.cpp

 ---------------------
  Date                 : November 2024
  Copyright            : (C) 2024 by Mathieu Pellerin
  Email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qflocatorfilter.h"

#include <QEventLoop>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrlQuery>
#include <qgsfeedback.h>

QfLocatorFilter::QfLocatorFilter( QObject *parent )
  : QgsLocatorFilter( parent )
{
  setUseWithoutPrefix( false );
}

QfLocatorFilter *QfLocatorFilter::clone() const
{
  QfLocatorFilter *filter = new QfLocatorFilter();
  filter->setFetchResultsDelay( fetchResultsDelay() );
  filter->setName( mName );
  filter->setDisplayName( mDisplayName );
  filter->setPrefix( mPrefix );
  filter->setParameters( mParameters );
  filter->setSource( mSource );
  filter->setLocatorBridge( mLocatorBridge );
  return filter;
}

void QfLocatorFilter::setParameters( const QVariantMap &parameters )
{
  if ( mParameters == parameters )
    return;

  mParameters = parameters;
  emit parametersChanged();
}

void QfLocatorFilter::setSource( const QUrl &source )
{
  if ( mSource.path() == source.path() )
    return;

  mSource = source;
  QUrlQuery query( mSource );
  if ( !query.hasQueryItem( "t" ) )
  {
    // Bypass caching to insure updated QML content is reloaded when the plugin is loaded
    query.addQueryItem( QStringLiteral( "t" ), QStringLiteral( "t=%1" ).arg( QDateTime::currentSecsSinceEpoch() ) );
    mSource.setQuery( query );
  }
  emit sourceChanged();
}

void QfLocatorFilter::setLocatorBridge( QfLocatorModelSuperBridge *locatorBridge )
{
  if ( mLocatorBridge == locatorBridge )
    return;

  mLocatorBridge = locatorBridge;
  emit locatorBridgeChanged();
}

void QfLocatorFilter::setDelay( int delay )
{
  if ( fetchResultsDelay() == delay )
    return;

  setFetchResultsDelay( delay );
  emit delayChanged();
}

void QfLocatorFilter::setName( const QString &name )
{
  if ( mName == name )
    return;

  mName = name;
  emit nameChanged();
}

void QfLocatorFilter::setDisplayName( const QString &displayName )
{
  if ( mDisplayName == displayName )
    return;

  mDisplayName = displayName;
  emit displayNameChanged();
}

void QfLocatorFilter::setPrefix( const QString &prefix )
{
  if ( mPrefix == prefix )
    return;

  mPrefix = prefix;
  emit prefixChanged();
}

void QfLocatorFilter::setDescription( const QString &description )
{
  if ( mDescription == description )
    return;

  mDescription = description;
  emit descriptionChanged();
}

void QfLocatorFilter::fetchResultsEnded()
{
  mFetchResultsEnded = true;
}

void QfLocatorFilter::prepareResult( const QVariant &details )
{
  const QVariantMap detailsMap = details.toMap();
  QgsLocatorResult result;
  result.setUserData( detailsMap.value( QStringLiteral( "userData" ) ) );
  result.displayString = detailsMap.value( QStringLiteral( "displayString" ), QString() ).toString();
  result.description = detailsMap.value( QStringLiteral( "description" ), QString() ).toString();
  result.score = detailsMap.value( QStringLiteral( "score" ), 0.5 ).toDouble();
  result.group = detailsMap.value( QStringLiteral( "group" ), QString() ).toString();
  result.groupScore = detailsMap.value( QStringLiteral( "groupScore" ), 0.5 ).toDouble();
  const QVariantList actions = detailsMap.value( QStringLiteral( "actions" ) ).toList();
  for ( const QVariant &action : actions )
  {
    const QVariantMap actionMap = action.toMap();
    result.actions << QgsLocatorResult::ResultAction( actionMap.value( QStringLiteral( "id" ), 0 ).toInt(), actionMap.value( QStringLiteral( "name" ), QString() ).toString(), actionMap.value( QStringLiteral( "icon" ), QString() ).toString() );
  }
  emit resultFetched( result );
}

void QfLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback )
{
  if ( mSource.isEmpty() )
    return;

  QQmlEngine engine;
  QQmlComponent component( &engine );
  component.loadUrl( mSource );
  QObject *object = component.create();
  if ( object )
  {
    mFetchResultsEnded = false;
    // These SIGNAL() SLOT() macros are needed, it is the only connect syntax that will work with signals declared within the QML environment itself
    connect( object, SIGNAL( prepareResult( QVariant ) ), this, SLOT( prepareResult( QVariant ) ) );
    connect( object, SIGNAL( fetchResultsEnded() ), this, SLOT( fetchResultsEnded() ) );

    QEventLoop loop;
    connect( object, SIGNAL( fetchResultsEnded() ), &loop, SLOT( quit() ) );
    connect( feedback, &QgsFeedback::canceled, &loop, &QEventLoop::quit );

    QMetaObject::invokeMethod( object, QStringLiteral( "fetchResults" ).toStdString().c_str(), QVariant( string ), QVariant::fromValue<QgsLocatorContext>( context ), QVariant::fromValue( mParameters ) );
    if ( !mFetchResultsEnded )
    {
      loop.exec();
    }
    object->deleteLater();
  }
}

void QfLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  QMetaObject::invokeMethod( this, QStringLiteral( "triggerResult" ).toStdString().c_str(), QVariant::fromValue<QgsLocatorResult>( result ) );
}

void QfLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int actionId )
{
  QMetaObject::invokeMethod( this, QStringLiteral( "triggerResultFromAction" ).toStdString().c_str(), QVariant::fromValue<QgsLocatorResult>( result ), QVariant( actionId ) );
}
```


