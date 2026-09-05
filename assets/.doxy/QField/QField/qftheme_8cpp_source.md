

# File qftheme.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qftheme.cpp**](qftheme_8cpp.md)

[Go to the documentation of this file](qftheme_8cpp.md)


```C++
/***************************************************************************
  qftheme.cpp - QfTheme

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

#include "qfplatformutilities.h"
#include "qftheme.h"

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaObject>
#include <QMetaProperty>
#include <QPalette>
#include <QSettings>

QfTheme::QfTheme( QObject *parent )
  : QObject( parent )
{
  mFontScale = std::clamp( QSettings().value( QStringLiteral( "fontScale" ), 1.0 ).toReal(), 0.2, 5.0 );
  mAppearance = QSettings().value( QStringLiteral( "appearance" ), QStringLiteral( "system" ) ).toString();

  loadFromJson();
  applyAppearance();
}

void QfTheme::loadFromJson()
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
  set( mQfieldcloudBlue, "qfieldcloudBlue" );
  set( mVertexColor, "vertexColor" );
  set( mVertexColorSemiOpaque, "vertexColorSemiOpaque" );
  set( mVertexSelectedColor, "vertexSelectedColor" );
  set( mVertexSelectedColorSemiOpaque, "vertexSelectedColorSemiOpaque" );
  set( mVertexNewColor, "vertexNewColor" );
  set( mVertexNewColorSemiOpaque, "vertexNewColorSemiOpaque" );
  set( mProcessingPreview, "processingPreview" );

  emit themeDataLoaded();
}

void QfTheme::setAppearance( const QString &appearance )
{
  if ( mAppearance == appearance )
  {
    return;
  }

  mAppearance = appearance;
  emit appearanceChanged();

  QSettings().setValue( QStringLiteral( "appearance" ), appearance );
  applyAppearance();
}

void QfTheme::applyAppearance( const QVariantMap &extraColors, BaseAppearance baseAppearance )
{
  if ( baseAppearance == UseSettingsAppearance )
  {
    if ( mAppearance == QStringLiteral( "dark" ) )
    {
      baseAppearance = DarkAppearance;
    }
    else if ( mAppearance == QStringLiteral( "light" ) )
    {
      baseAppearance = LightAppearance;
    }
    else
    {
      baseAppearance = SystemAppearance;
    }
  }

  bool darkTheme = false;
  if ( baseAppearance == SystemAppearance )
  {
    darkTheme = QfPlatformUtilities::instance()->isSystemDarkTheme();
  }
  else
  {
    darkTheme = ( baseAppearance == DarkAppearance );
  }

  if ( mDarkTheme != darkTheme )
  {
    mDarkTheme = darkTheme;
    emit darkThemeChanged();
  }

  applyColors( mDarkTheme ? mDarkThemeColors : mLightThemeColors );

  if ( !extraColors.isEmpty() )
  {
    applyColors( extraColors );
  }

  QPalette palette = qApp->palette();
  palette.setColor( QPalette::Link, mMainColor );
  palette.setColor( QPalette::LinkVisited, mMainColor );
  qApp->setPalette( palette );
}

void QfTheme::applyColors( const QVariantMap &colors )
{
  const QMetaObject *meta = &QfTheme::staticMetaObject;
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

void QfTheme::setSystemFontPointSize( qreal size )
{
  if ( size == 0.0 || qFuzzyCompare( mSystemFontPointSize, size ) )
  {
    return;
  }

  mSystemFontPointSize = size;
  emit fontScaleChanged();
}

void QfTheme::setScreenPpi( qreal ppi )
{
  if ( ppi == 0.0 || qFuzzyCompare( mScreenPpi, ppi ) )
  {
    return;
  }

  mScreenPpi = ppi;
  emit screenPpiChanged();
}

void QfTheme::setMainColor( const QColor &color )
{
  if ( mMainColor == color )
  {
    return;
  }

  mMainColor = color;
  emit mainColorChanged();

  // mainColorSemiOpaque always tracks mainColor with reduced opacity
  QColor semiOpaque = color;
  semiOpaque.setAlpha( 0xbb );
  mMainColorSemiOpaque = semiOpaque;
  emit themeDataLoaded();
}

void QfTheme::setMainOverlayColor( const QColor &color )
{
  if ( mMainOverlayColor == color )
  {
    return;
  }

  mMainOverlayColor = color;
  emit mainOverlayColorChanged();
}

void QfTheme::setMainBackgroundColor( const QColor &color )
{
  if ( mMainBackgroundColor == color )
  {
    return;
  }

  mMainBackgroundColor = color;
  emit mainBackgroundColorChanged();
}

void QfTheme::setMainBackgroundColorSemiOpaque( const QColor &color )
{
  if ( mMainBackgroundColorSemiOpaque == color )
  {
    return;
  }

  mMainBackgroundColorSemiOpaque = color;
  emit mainBackgroundColorSemiOpaqueChanged();
}

void QfTheme::setMainTextColor( const QColor &color )
{
  if ( mMainTextColor == color )
  {
    return;
  }

  mMainTextColor = color;
  emit mainTextColorChanged();
}

void QfTheme::setMainTextDisabledColor( const QColor &color )
{
  if ( mMainTextDisabledColor == color )
  {
    return;
  }

  mMainTextDisabledColor = color;
  emit mainTextDisabledColorChanged();
}

void QfTheme::setSecondaryTextColor( const QColor &color )
{
  if ( mSecondaryTextColor == color )
  {
    return;
  }

  mSecondaryTextColor = color;
  emit secondaryTextColorChanged();
}

void QfTheme::setControlBackgroundColor( const QColor &color )
{
  if ( mControlBackgroundColor == color )
  {
    return;
  }

  mControlBackgroundColor = color;
  emit controlBackgroundColorChanged();
}

void QfTheme::setControlBackgroundAlternateColor( const QColor &color )
{
  if ( mControlBackgroundAlternateColor == color )
  {
    return;
  }

  mControlBackgroundAlternateColor = color;
  emit controlBackgroundAlternateColorChanged();
}

void QfTheme::setControlBackgroundDisabledColor( const QColor &color )
{
  if ( mControlBackgroundDisabledColor == color )
  {
    return;
  }

  mControlBackgroundDisabledColor = color;
  emit controlBackgroundDisabledColorChanged();
}

void QfTheme::setControlBorderColor( const QColor &color )
{
  if ( mControlBorderColor == color )
  {
    return;
  }

  mControlBorderColor = color;
  emit controlBorderColorChanged();
}

void QfTheme::setButtonColor( const QColor &color )
{
  if ( mButtonColor == color )
  {
    return;
  }

  mButtonColor = color;
  emit buttonColorChanged();
}

void QfTheme::setButtonBackgroundColor( const QColor &color )
{
  if ( mButtonBackgroundColor == color )
  {
    return;
  }

  mButtonBackgroundColor = color;
  emit buttonBackgroundColorChanged();
}

void QfTheme::setToolButtonColor( const QColor &color )
{
  if ( mToolButtonColor == color )
  {
    return;
  }

  mToolButtonColor = color;
  emit toolButtonColorChanged();
}

void QfTheme::setToolButtonBackgroundColor( const QColor &color )
{
  if ( mToolButtonBackgroundColor == color )
  {
    return;
  }

  mToolButtonBackgroundColor = color;
  emit toolButtonBackgroundColorChanged();
}

void QfTheme::setToolButtonBackgroundSemiOpaqueColor( const QColor &color )
{
  if ( mToolButtonBackgroundSemiOpaqueColor == color )
  {
    return;
  }

  mToolButtonBackgroundSemiOpaqueColor = color;
  emit toolButtonBackgroundSemiOpaqueColorChanged();
}

void QfTheme::setScrollBarBackgroundColor( const QColor &color )
{
  if ( mScrollBarBackgroundColor == color )
  {
    return;
  }

  mScrollBarBackgroundColor = color;
  emit scrollBarBackgroundColorChanged();
}

void QfTheme::setGroupBoxBackgroundColor( const QColor &color )
{
  if ( mGroupBoxBackgroundColor == color )
  {
    return;
  }

  mGroupBoxBackgroundColor = color;
  emit groupBoxBackgroundColorChanged();
}

void QfTheme::setGroupBoxSurfaceColor( const QColor &color )
{
  if ( mGroupBoxSurfaceColor == color )
  {
    return;
  }

  mGroupBoxSurfaceColor = color;
  emit groupBoxSurfaceColorChanged();
}

void QfTheme::setGoodColor( const QColor &color )
{
  if ( mGoodColor == color )
  {
    return;
  }

  mGoodColor = color;
  emit goodColorChanged();
}

void QfTheme::setWarningColor( const QColor &color )
{
  if ( mWarningColor == color )
  {
    return;
  }

  mWarningColor = color;
  emit warningColorChanged();
}

void QfTheme::setErrorColor( const QColor &color )
{
  if ( mErrorColor == color )
  {
    return;
  }

  mErrorColor = color;
  emit errorColorChanged();
}

void QfTheme::setDarkTheme( bool dark )
{
  if ( mDarkTheme == dark )
  {
    return;
  }

  mDarkTheme = dark;
  emit darkThemeChanged();
}

void QfTheme::setFontScale( qreal scale )
{
  scale = std::clamp( scale, 0.2, 5.0 );
  if ( qFuzzyCompare( mFontScale, scale ) )
  {
    return;
  }

  QSettings().setValue( QStringLiteral( "fontScale" ), scale );

  mFontScale = scale;
  emit fontScaleChanged();
}

void QfTheme::setToolButtonSize( int size )
{
  if ( mToolButtonSize == size )
  {
    return;
  }

  mToolButtonSize = size;
  emit toolButtonSizeChanged();
}

QFont QfTheme::makeFont( qreal scaleFactor, bool bold ) const
{
  QFont font;
  font.setPointSizeF( mSystemFontPointSize * mFontScale * scaleFactor );
  font.setBold( bold );
  font.setWeight( bold ? QFont::Bold : QFont::Normal );
  return font;
}

QString QfTheme::getThemeIcon( const QString &name ) const
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

QString QfTheme::getThemeVectorIcon( const QString &name ) const
{
  return QStringLiteral( "qrc:/themes/qfield/nodpi/%1.svg" ).arg( name );
}

QString QfTheme::colorToHtml( const QColor &color ) const
{
  return QStringLiteral( "rgba(%1,%2,%3,%4)" )
    .arg( static_cast<int>( color.redF() * 255 ) )
    .arg( static_cast<int>( color.greenF() * 255 ) )
    .arg( static_cast<int>( color.blueF() * 255 ) )
    .arg( static_cast<int>( color.alphaF() * 255 ) );
}

QString QfTheme::toInlineStyles( const QVariantMap &styleProperties ) const
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
```


