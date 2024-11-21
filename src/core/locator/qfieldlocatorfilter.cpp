/***************************************************************************
  qfieldlocatorfilter.cpp

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

#include "qfieldlocatorfilter.h"

#include <QEventLoop>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrlQuery>
#include <qgsfeedback.h>

QFieldLocatorFilter::QFieldLocatorFilter( QObject *parent )
  : QgsLocatorFilter( parent )
{
  setUseWithoutPrefix( false );
}

QFieldLocatorFilter *QFieldLocatorFilter::clone() const
{
  QFieldLocatorFilter *filter = new QFieldLocatorFilter();
  filter->setName( mName );
  filter->setDisplayName( mDisplayName );
  filter->setPrefix( mPrefix );
  filter->setSource( mSource );
  filter->setLocatorBridge( mLocatorBridge );
  return filter;
}

void QFieldLocatorFilter::setSource( const QUrl &source )
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

void QFieldLocatorFilter::setLocatorBridge( LocatorModelSuperBridge *locatorBridge )
{
  if ( mLocatorBridge == locatorBridge )
    return;

  mLocatorBridge = locatorBridge;
  emit locatorBridgeChanged();
}

void QFieldLocatorFilter::setName( const QString &name )
{
  if ( mName == name )
    return;

  mName = name;
  emit nameChanged();
}

void QFieldLocatorFilter::setDisplayName( const QString &displayName )
{
  if ( mDisplayName == displayName )
    return;

  mDisplayName = displayName;
  emit displayNameChanged();
}

void QFieldLocatorFilter::setPrefix( const QString &prefix )
{
  if ( mPrefix == prefix )
    return;

  mPrefix = prefix;
  emit prefixChanged();
}

void QFieldLocatorFilter::fetchResultsEnded()
{
  mFetchResultsEnded = true;
}

void QFieldLocatorFilter::prepareResult( QVariant details )
{
  QVariantMap detailsMap = details.toMap();
  QgsLocatorResult result;
  result.userData() = detailsMap.value( QStringLiteral( "userData" ) );
  result.displayString = detailsMap.value( QStringLiteral( "displayString" ), QString() ).toString();
  result.description = detailsMap.value( QStringLiteral( "description" ), QString() ).toString();
  result.score = detailsMap.value( QStringLiteral( "score" ), 0.5 ).toDouble();
  result.group = detailsMap.value( QStringLiteral( "group" ), QString() ).toString();
  result.groupScore = detailsMap.value( QStringLiteral( "groupScore" ), 0.5 ).toDouble();
  emit resultFetched( result );
}

void QFieldLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback )
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
    connect( object, SIGNAL( prepareResult( QVariant ) ), this, SLOT( prepareResult( QVariant ) ) );
    connect( object, SIGNAL( fetchResultsEnded() ), this, SLOT( fetchResultsEnded() ) );

    QEventLoop loop;
    connect( object, SIGNAL( fetchResultsEnded() ), &loop, SLOT( quit() ) );
    connect( feedback, &QgsFeedback::canceled, &loop, &QEventLoop::quit );

    QMetaObject::invokeMethod( object, QStringLiteral( "fetchResults" ).toStdString().c_str(), QVariant( string ), QVariant::fromValue<QgsLocatorContext>( context ) );
    if ( !mFetchResultsEnded )
    {
      loop.exec();
    }
    object->deleteLater();
  }
}

void QFieldLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  QMetaObject::invokeMethod( this, QStringLiteral( "triggerResult" ).toStdString().c_str(), QVariant::fromValue<QgsLocatorResult>( result ) );
}

void QFieldLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int actionId )
{
}
