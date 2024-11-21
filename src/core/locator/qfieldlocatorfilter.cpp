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
  if ( mSource.toString( QUrl::RemoveQuery ) == source.toString( QUrl::RemoveQuery ) )
    return;

  mSource = source;
  mSource.setQuery( QStringLiteral( "t=%1" ).arg( QDateTime::currentSecsSinceEpoch() ) );
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
}

void QFieldLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int actionId )
{
}
