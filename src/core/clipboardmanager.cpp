/***************************************************************************
  clipboardmanager.cpp - ClipboardManager

 ---------------------
 begin                : 27.5.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "clipboardmanager.h"
#include "featureutils.h"
#include "qgsvectorlayerutils.h"

#include <QApplication>
#include <QDomDocument>
#include <qgsfeature.h>
#include <qgsmessagelog.h>
#include <qgsvectorlayer.h>

ClipboardManager::ClipboardManager( QObject *parent )
  : QObject( parent )
  , mClipboard( QApplication::clipboard() )
{
  connect( QApplication::clipboard(), &QClipboard::dataChanged, this, &ClipboardManager::dataChanged );
}

void ClipboardManager::dataChanged()
{
  if ( mSkipDataChanged )
  {
    mSkipDataChanged = false;
    return;
  }

  const QMimeData *mimeData = mClipboard->mimeData();
  if ( mimeData->hasHtml() && !mHtmlFeature.isEmpty() && mimeData->html() == mHtmlFeature )
  {
    // On Android, data changed can be triggered multiple times, yet the content
    // stays the same, assume this is what is happening here.
    return;
  }

  mHoldsFeature = false;
  mHasNativeFeature = false;
  mNativeFeature = QgsFeature();
  mHtmlFeature.clear();

  bool holdsFeature = false;

  if ( mimeData->hasHtml() )
  {
    QDomDocument doc;
    doc.setContent( mimeData->html() );
    const QDomNodeList nodes = doc.elementsByTagName( QStringLiteral( "table" ) );
    if ( !nodes.isEmpty() )
    {
      const QDomElement table = nodes.at( 0 ).toElement();
      if ( table.hasAttribute( QStringLiteral( "qfield" ) ) )
      {
        holdsFeature = true;
      }
    }
  }

  if ( mHoldsFeature != holdsFeature )
  {
    mHoldsFeature = holdsFeature;
    emit holdsFeatureChanged();
  }
}

void ClipboardManager::copyFeatureToClipboard( QgsVectorLayer *layer, QgsFeatureId fid, bool includeGeometry )
{
  if ( layer )
  {
    const QgsFeature feature = layer->getFeature( fid );
    copyFeatureToClipboard( feature, includeGeometry );
  }
}

void ClipboardManager::copyFeatureToClipboard( const QgsFeature &feature, bool includeGeometry )
{
  QStringList textLines;
  QStringList htmlLines;

  if ( includeGeometry )
  {
    textLines << QStringLiteral( "%1: %2" ).arg( QStringLiteral( "%1:" ).arg( tr( "Geometry" ) ), feature.geometry().asWkt() );
    htmlLines << QStringLiteral( "<td geometry=\"1\">%1</td><td>%2</td>" ).arg( QStringLiteral( "%1:" ).arg( tr( "Geometry" ) ), feature.geometry().asWkt().toHtmlEscaped() );
  }

  const QgsFields fields = feature.fields();
  const QgsAttributes attributes = feature.attributes();
  for ( int i = 0; i < fields.count(); i++ )
  {
    textLines << QStringLiteral( "%1: %2" ).arg( fields[i].displayName(), attributes[i].toString() );
    htmlLines << QStringLiteral( "<td>%1</td><td>%2</td>" ).arg( fields[i].displayName(), attributes[i].toString().toHtmlEscaped() );
  }

  QMimeData *mimeData = new QMimeData();
  mimeData->setText( textLines.join( '\n' ) );
  mimeData->setHtml( QStringLiteral( R"""(
<!DOCTYPE html>
<html>
 <head>
  <meta http-equiv="content-type" content="text/html; charset=utf-8"/>
 </head>
 <body>
  <table border="1" qfield="1"><tr>%1</tr></table>
 </body>
</html>)""" )
                       .arg( htmlLines.join( QStringLiteral( "</tr><tr>" ) ) ) );
  mHtmlFeature = mimeData->html();

  mSkipDataChanged = true;
  mClipboard->setMimeData( mimeData );

  mNativeFeature = feature;
  mHasNativeFeature = true;

  if ( mHoldsFeature != true )
  {
    mHoldsFeature = true;
    emit holdsFeatureChanged();
  }
}

QgsFeature ClipboardManager::pasteFeatureFromClipboard()
{
  if ( mHasNativeFeature )
  {
    return mNativeFeature;
  }

  QgsFeature feature;
  QgsFields fields;
  QgsAttributes attributes;

  const QMimeData *mimeData = mClipboard->mimeData();
  if ( mimeData->hasHtml() )
  {
    QDomDocument doc;
    doc.setContent( mimeData->html() );
    const QDomNodeList nodes = doc.elementsByTagName( QStringLiteral( "table" ) );
    if ( !nodes.isEmpty() )
    {
      const QDomElement table = nodes.at( 0 ).toElement();
      if ( table.hasAttribute( QStringLiteral( "qfield" ) ) )
      {
        const QDomNodeList trs = table.elementsByTagName( QStringLiteral( "tr" ) );
        for ( int i = 0; trs.size(); i++ )
        {
          const QDomElement tr = trs.at( i ).toElement();
          const QDomNodeList tds = tr.elementsByTagName( QStringLiteral( "td" ) );
          if ( tds.size() >= 2 )
          {
            if ( tds.at( 0 ).toElement().hasAttribute( QStringLiteral( "geometry" ) ) )
            {
              const QgsGeometry geometry = QgsGeometry::fromWkt( tds.at( 1 ).toElement().text() );
              feature.setGeometry( geometry );
            }
            else
            {
              const QString fieldName = tds.at( 0 ).toElement().text();
              fields.append( QgsField( fieldName, QMetaType::QString ) );
              attributes << tds.at( 1 ).toElement().text();
            }
          }
        }
        feature.setFields( fields );
        feature.setAttributes( attributes );
      }
    }
  }

  return feature;
}

bool ClipboardManager::pasteAsNewFeatureFromClipboardIntoLayer( QgsVectorLayer *layer )
{
  if ( !layer )
  {
    QgsMessageLog::logMessage( tr( "Paste failed: no destination layer provided" ), "QField" );
    return false;
  }

  QgsFeature feature = pasteFeatureFromClipboard();
  if ( !feature.isValid() )
  {
    QgsMessageLog::logMessage( tr( "Paste failed: clipboard feature is invalid" ), "QField" );
    return false;
  }

  const QgsFeatureList compatible = QgsVectorLayerUtils::makeFeaturesCompatible( { feature }, layer );
  if ( compatible.isEmpty() )
  {
    QgsMessageLog::logMessage( tr( "Paste failed: no compatible features could be created" ), "QField" );
    return false;
  }

  if ( !layer->isEditable() )
  {
    if ( !layer->startEditing() )
    {
      QgsMessageLog::logMessage( tr( "Paste failed: could not start editing on layer %1" ).arg( layer->name() ), "QField" );
      return false;
    }
  }

  for ( const QgsFeature &f : compatible )
  {
    QgsFeature copyFeature = f;
    copyFeature.setId( QgsFeatureId() ); // ensure it's a new feature ???
    if ( !layer->addFeature( copyFeature ) )
    {
      QgsMessageLog::logMessage( tr( "Paste failed: could not add feature to layer %1" ).arg( layer->name() ), "QField" );
      layer->rollBack();
      return false;
    }
  }

  if ( !layer->commitChanges( true ) )
  {
    QgsMessageLog::logMessage( tr( "Paste failed: commitChanges failed on layer %1" ).arg( layer->name() ), "QField" );
    layer->rollBack();
    return false;
  }

  return true;
}
