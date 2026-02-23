/***************************************************************************
  theme.cpp - Theme

---------------------
begin                : 22.2.2026
copyright            : (C) 2026 by Kaustuv Pokharel
email                : kaustuv@opengis.ch
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "platformutilities.h"
#include "theme.h"

#include <QFile>
#include <QFont>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaObject>
#include <QMetaProperty>
#include <QSettings>

Theme::Theme( QObject *parent )
  : QObject( parent )
{
  loadFromJson();
  applyAppearance();
  applyFontScale();
}

void Theme::loadFromJson()
{
  QFile file( QStringLiteral( ":/theme/theme.json" ) );
  if ( !file.open( QIODevice::ReadOnly ) )
  {
    qWarning( "Theme: could not open :/theme/theme.json" );
    return;
  }

  const QJsonDocument doc = QJsonDocument::fromJson( file.readAll() );
  if ( doc.isNull() || !doc.isObject() )
  {
    qWarning( "Theme: theme.json is not valid JSON" );
    return;
  }

  const QJsonObject root = doc.object();
  mDarkThemeColors = root.value( QStringLiteral( "darkThemeColors" ) ).toObject().toVariantMap();
  mLightThemeColors = root.value( QStringLiteral( "lightThemeColors" ) ).toObject().toVariantMap();

  // Fixed colors have no WRITE accessor so they cannot go through applyColors()
  // Assign directly via a lambda, only applied if the color string is valid
  const QVariantMap fixed = root.value( QStringLiteral( "fixedColors" ) ).toObject().toVariantMap();
  const auto set = [&fixed]( QColor &member, const char *key ) {
    const QColor color( fixed.value( QLatin1String( key ) ).toString() );
    if ( color.isValid() )
    {
      member = color;
    }
  };

  set( mMainColorSemiOpaque, "mainColorSemiOpaque" );
  set( mDarkRed, "darkRed" );
  set( mDarkGray, "darkGray" );
  set( mDarkGraySemiOpaque, "darkGraySemiOpaque" );
  set( mGray, "gray" );
  set( mLightGray, "lightGray" );
  set( mLightestGray, "lightestGray" );
  set( mLightestGraySemiOpaque, "lightestGraySemiOpaque" );
  set( mLight, "light" );
  set( mGoodColor, "goodColor" );
  set( mWarningColor, "warningColor" );
  set( mCloudColor, "cloudColor" );
  set( mPositionColor, "positionColor" );
  set( mPositionColorSemiOpaque, "positionColorSemiOpaque" );
  set( mPositionBackgroundColor, "positionBackgroundColor" );
  set( mDarkPositionColor, "darkPositionColor" );
  set( mDarkPositionColorSemiOpaque, "darkPositionColorSemiOpaque" );
  set( mAccuracyBad, "accuracyBad" );
  set( mAccuracyTolerated, "accuracyTolerated" );
  set( mAccuracyExcellent, "accuracyExcellent" );
  set( mNavigationColor, "navigationColor" );
  set( mNavigationColorSemiOpaque, "navigationColorSemiOpaque" );
  set( mNavigationBackgroundColor, "navigationBackgroundColor" );
  set( mSensorBackgroundColor, "sensorBackgroundColor" );
  set( mBookmarkDefault, "bookmarkDefault" );
  set( mBookmarkOrange, "bookmarkOrange" );
  set( mBookmarkRed, "bookmarkRed" );
  set( mBookmarkBlue, "bookmarkBlue" );
  set( mVertexColor, "vertexColor" );
  set( mVertexColorSemiOpaque, "vertexColorSemiOpaque" );
  set( mVertexSelectedColor, "vertexSelectedColor" );
  set( mVertexSelectedColorSemiOpaque, "vertexSelectedColorSemiOpaque" );
  set( mVertexNewColor, "vertexNewColor" );
  set( mVertexNewColorSemiOpaque, "vertexNewColorSemiOpaque" );
  set( mProcessingPreview, "processingPreview" );

  emit themeDataLoaded();
}

void Theme::applyAppearance( const QVariantMap &extraColors, const QString &baseAppearance )
{
  QString appearance = baseAppearance;
  if ( appearance.isEmpty() )
  {
    appearance = QSettings().value( QStringLiteral( "appearance" ), QStringLiteral( "system" ) ).toString();
  }

  if ( appearance == QStringLiteral( "system" ) )
  {
    mDarkTheme = PlatformUtilities::instance()->isSystemDarkTheme();
  }
  else
  {
    mDarkTheme = ( appearance == QStringLiteral( "dark" ) );
  }

  emit darkThemeChanged();

  applyColors( mDarkTheme ? mDarkThemeColors : mLightThemeColors );

  if ( !extraColors.isEmpty() )
  {
    applyColors( extraColors );
  }
}

void Theme::applyColors( const QVariantMap &colors )
{
  const QMetaObject *meta = metaObject();
  for ( auto it = colors.cbegin(); it != colors.cend(); ++it )
  {
    const int idx = meta->indexOfProperty( qPrintable( it.key() ) );
    if ( idx < 0 )
    {
      continue;
    }

    const QMetaProperty prop = meta->property( idx );
    if ( !prop.isWritable() )
    {
      continue;
    }

    const QColor color = it.value().canConvert<QColor>() ? it.value().value<QColor>() : QColor( it.value().toString() );
    if ( !color.isValid() )
    {
      continue;
    }

    prop.write( this, color );
  }
}

void Theme::applyFontScale()
{
  const qreal scale = QSettings().value( QStringLiteral( "fontScale" ), 1.0 ).toReal();
  setFontScale( scale );
}

void Theme::setSystemFontPointSize( qreal size )
{
  if ( size > 0.0 && !qFuzzyCompare( mSystemFontPointSize, size ) )
  {
    mSystemFontPointSize = size;
    emit fontScaleChanged();
  }
}

void Theme::setScreenPpi( qreal ppi )
{
  mScreenPpi = ppi;
}

QFont Theme::makeFont( qreal scaleFactor, bool bold ) const
{
  QFont font;
  font.setPointSizeF( mSystemFontPointSize * mFontScale * scaleFactor );
  font.setBold( bold );
  font.setWeight( bold ? QFont::Bold : QFont::Normal );
  return font;
}

QString Theme::getThemeIcon( const QString &name ) const
{
  QString density;
  if ( mScreenPpi >= 360 )
  {
    density = QStringLiteral( "xxxhdpi" );
  }
  else if ( mScreenPpi >= 270 )
  {
    density = QStringLiteral( "xxhdpi" );
  }
  else if ( mScreenPpi >= 180 )
  {
    density = QStringLiteral( "xhdpi" );
  }
  else if ( mScreenPpi >= 135 )
  {
    density = QStringLiteral( "hdpi" );
  }
  else
  {
    density = QStringLiteral( "mdpi" );
  }

  return QStringLiteral( "qrc:/themes/qfield/%1/%2.png" ).arg( density, name );
}

QString Theme::getThemeVectorIcon( const QString &name ) const
{
  return QStringLiteral( "qrc:/themes/qfield/nodpi/%1.svg" ).arg( name );
}

QString Theme::colorToHtml( const QColor &color ) const
{
  return QStringLiteral( "rgba(%1,%2,%3,%4)" )
    .arg( static_cast<int>( color.redF() * 255 ) )
    .arg( static_cast<int>( color.greenF() * 255 ) )
    .arg( static_cast<int>( color.blueF() * 255 ) )
    .arg( static_cast<int>( color.alphaF() * 255 ) );
}

QString Theme::toInlineStyles( const QVariantMap &styleProperties ) const
{
  QString styles;
  for ( auto it = styleProperties.cbegin(); it != styleProperties.cend(); ++it )
  {
    styles += it.key() + QStringLiteral( ": " );
    if ( it.value().canConvert<QColor>() )
    {
      styles += colorToHtml( it.value().value<QColor>() );
    }
    else
    {
      styles += it.value().toString();
    }
    styles += QStringLiteral( ";" );
  }

  return styles;
}
