/***************************************************************************
  theme.h - Theme

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

#ifndef THEME_H
#define THEME_H

#include "qfield_core_export.h"

#include <QColor>
#include <QFont>
#include <QObject>
#include <QVariantMap>

/**
 * \ingroup core
 *
 * \brief Provides all color, font scale, and layout constants used throughout
 * the QField UI. Registered as a QML singleton under URI "Theme" 1.0.
 *
 * \note Default colors are loaded from :/theme/theme.json
 */
class QFIELD_CORE_EXPORT Theme final : public QObject
{
    Q_OBJECT

    // Palette tables, exposed for QML code that reads them directly
    Q_PROPERTY( QVariantMap darkThemeColors READ darkThemeColors CONSTANT )
    Q_PROPERTY( QVariantMap lightThemeColors READ lightThemeColors CONSTANT )

    // Switchable colors, replaced on every applyAppearance() call
    Q_PROPERTY( QColor mainColor READ mainColor WRITE setMainColor NOTIFY mainColorChanged )
    Q_PROPERTY( QColor mainOverlayColor READ mainOverlayColor WRITE setMainOverlayColor NOTIFY mainOverlayColorChanged )
    Q_PROPERTY( QColor mainBackgroundColor READ mainBackgroundColor WRITE setMainBackgroundColor NOTIFY mainBackgroundColorChanged )
    Q_PROPERTY( QColor mainBackgroundColorSemiOpaque READ mainBackgroundColorSemiOpaque WRITE setMainBackgroundColorSemiOpaque NOTIFY mainBackgroundColorSemiOpaqueChanged )
    Q_PROPERTY( QColor mainTextColor READ mainTextColor WRITE setMainTextColor NOTIFY mainTextColorChanged )
    Q_PROPERTY( QColor mainTextDisabledColor READ mainTextDisabledColor WRITE setMainTextDisabledColor NOTIFY mainTextDisabledColorChanged )
    Q_PROPERTY( QColor secondaryTextColor READ secondaryTextColor WRITE setSecondaryTextColor NOTIFY secondaryTextColorChanged )
    Q_PROPERTY( QColor controlBackgroundColor READ controlBackgroundColor WRITE setControlBackgroundColor NOTIFY controlBackgroundColorChanged )
    Q_PROPERTY( QColor controlBackgroundAlternateColor READ controlBackgroundAlternateColor WRITE setControlBackgroundAlternateColor NOTIFY controlBackgroundAlternateColorChanged )
    Q_PROPERTY( QColor controlBackgroundDisabledColor READ controlBackgroundDisabledColor WRITE setControlBackgroundDisabledColor NOTIFY controlBackgroundDisabledColorChanged )
    Q_PROPERTY( QColor controlBorderColor READ controlBorderColor WRITE setControlBorderColor NOTIFY controlBorderColorChanged )
    Q_PROPERTY( QColor buttonTextColor READ buttonTextColor WRITE setButtonTextColor NOTIFY buttonTextColorChanged )
    Q_PROPERTY( QColor toolButtonColor READ toolButtonColor WRITE setToolButtonColor NOTIFY toolButtonColorChanged )
    Q_PROPERTY( QColor toolButtonBackgroundColor READ toolButtonBackgroundColor WRITE setToolButtonBackgroundColor NOTIFY toolButtonBackgroundColorChanged )
    Q_PROPERTY( QColor toolButtonBackgroundSemiOpaqueColor READ toolButtonBackgroundSemiOpaqueColor WRITE setToolButtonBackgroundSemiOpaqueColor NOTIFY toolButtonBackgroundSemiOpaqueColorChanged )
    Q_PROPERTY( QColor scrollBarBackgroundColor READ scrollBarBackgroundColor WRITE setScrollBarBackgroundColor NOTIFY scrollBarBackgroundColorChanged )
    Q_PROPERTY( QColor groupBoxBackgroundColor READ groupBoxBackgroundColor WRITE setGroupBoxBackgroundColor NOTIFY groupBoxBackgroundColorChanged )
    Q_PROPERTY( QColor groupBoxSurfaceColor READ groupBoxSurfaceColor WRITE setGroupBoxSurfaceColor NOTIFY groupBoxSurfaceColorChanged )

    // Fixed colors, set once from JSON, never change at runtime
    Q_PROPERTY( QColor mainColorSemiOpaque READ mainColorSemiOpaque NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor darkRed READ darkRed NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor darkGray READ darkGray NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor darkGraySemiOpaque READ darkGraySemiOpaque NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor gray READ gray NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor lightGray READ lightGray NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor lightestGray READ lightestGray NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor lightestGraySemiOpaque READ lightestGraySemiOpaque NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor light READ light NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor goodColor READ goodColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor warningColor READ warningColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor cloudColor READ cloudColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor positionColor READ positionColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor positionColorSemiOpaque READ positionColorSemiOpaque NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor positionBackgroundColor READ positionBackgroundColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor darkPositionColor READ darkPositionColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor darkPositionColorSemiOpaque READ darkPositionColorSemiOpaque NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor accuracyBad READ accuracyBad NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor accuracyTolerated READ accuracyTolerated NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor accuracyExcellent READ accuracyExcellent NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor navigationColor READ navigationColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor navigationColorSemiOpaque READ navigationColorSemiOpaque NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor navigationBackgroundColor READ navigationBackgroundColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor sensorBackgroundColor READ sensorBackgroundColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor bookmarkDefault READ bookmarkDefault NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor bookmarkOrange READ bookmarkOrange NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor bookmarkRed READ bookmarkRed NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor bookmarkBlue READ bookmarkBlue NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor vertexColor READ vertexColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor vertexColorSemiOpaque READ vertexColorSemiOpaque NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor vertexSelectedColor READ vertexSelectedColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor vertexSelectedColorSemiOpaque READ vertexSelectedColorSemiOpaque NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor vertexNewColor READ vertexNewColor NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor vertexNewColorSemiOpaque READ vertexNewColorSemiOpaque NOTIFY themeDataLoaded )
    Q_PROPERTY( QColor processingPreview READ processingPreview NOTIFY themeDataLoaded )

    // Derived from darkTheme state
    Q_PROPERTY( QColor errorColor READ errorColor NOTIFY darkThemeChanged )

    Q_PROPERTY( bool darkTheme READ darkTheme WRITE setDarkTheme NOTIFY darkThemeChanged )
    Q_PROPERTY( qreal fontScale READ fontScale WRITE setFontScale NOTIFY fontScaleChanged )

    Q_PROPERTY( QFont defaultFont READ defaultFont NOTIFY fontScaleChanged )
    Q_PROPERTY( QFont tinyFont READ tinyFont NOTIFY fontScaleChanged )
    Q_PROPERTY( QFont tipFont READ tipFont NOTIFY fontScaleChanged )
    Q_PROPERTY( QFont resultFont READ resultFont NOTIFY fontScaleChanged )
    Q_PROPERTY( QFont strongFont READ strongFont NOTIFY fontScaleChanged )
    Q_PROPERTY( QFont strongTipFont READ strongTipFont NOTIFY fontScaleChanged )
    Q_PROPERTY( QFont secondaryTitleFont READ secondaryTitleFont NOTIFY fontScaleChanged )
    Q_PROPERTY( QFont titleFont READ titleFont NOTIFY fontScaleChanged )
    Q_PROPERTY( QFont strongTitleFont READ strongTitleFont NOTIFY fontScaleChanged )

    Q_PROPERTY( int popupScreenEdgeVerticalMargin READ popupScreenEdgeVerticalMargin CONSTANT )
    Q_PROPERTY( int popupScreenEdgeHorizontalMargin READ popupScreenEdgeHorizontalMargin CONSTANT )
    Q_PROPERTY( int menuItemIconlessLeftPadding READ menuItemIconlessLeftPadding CONSTANT )
    Q_PROPERTY( int menuItemLeftPadding READ menuItemLeftPadding CONSTANT )
    Q_PROPERTY( int menuItemCheckLeftPadding READ menuItemCheckLeftPadding CONSTANT )

  public:
    explicit Theme( QObject *parent = nullptr );

    /**
   * Applies the dark/light/system appearance setting and the matching color palette.
   * An optional \a extraColors map is applied on top for per-call overrides.
   * \note Material.theme and Application.styleHints.colorScheme are QML-only
   * APIs; bind them in QML to \c Theme.darkTheme instead
   */
    Q_INVOKABLE void applyAppearance( const QVariantMap &extraColors = QVariantMap(), BaseAppearance baseAppearance = UseSettingsAppearance );

    /**
   * Applies a map of { propertyName -> color } to this object.
   * Only writable properties are touched; invalid colors and unknown keys are skipped.
   */
    Q_INVOKABLE void applyColors( const QVariantMap &colors );

    /**
   * Reads \c fontScale from QSettings and updates all font properties.
   */
    Q_INVOKABLE void applyFontScale();

    /**
   * Returns the raster icon resource path for \a name, selecting the density
   * bucket that best matches the current screen PPI.
   */
    Q_INVOKABLE QString getThemeIcon( const QString &name ) const;

    /**
   * Returns the SVG icon resource path for \a name.
   */
    Q_INVOKABLE QString getThemeVectorIcon( const QString &name ) const;

    /**
   * Converts \a color to a CSS \c rgba() string.
   */
    Q_INVOKABLE QString colorToHtml( const QColor &color ) const;

    /**
   * Converts a map of property/value pairs to an inline style string.
   * QColor values are converted via colorToHtml().
   */
    Q_INVOKABLE QString toInlineStyles( const QVariantMap &styleProperties ) const;

    /**
   * Sets the system font point size used for all font properties.
   * Called by qgismobileapp.cpp after the QML engine is up.
   */
    Q_INVOKABLE void setSystemFontPointSize( qreal size );

    /**
   * Sets the screen PPI used for icon density selection.
   * Called by qgismobileapp.cpp after the QML engine is up.
   */
    Q_INVOKABLE void setScreenPpi( qreal ppi );

    QVariantMap darkThemeColors() const { return mDarkThemeColors; }
    QVariantMap lightThemeColors() const { return mLightThemeColors; }

    QColor mainColor() const { return mMainColor; }
    void setMainColor( const QColor &c );

    QColor mainOverlayColor() const { return mMainOverlayColor; }
    void setMainOverlayColor( const QColor &c );

    QColor mainBackgroundColor() const { return mMainBackgroundColor; }
    void setMainBackgroundColor( const QColor &c );

    QColor mainBackgroundColorSemiOpaque() const { return mMainBackgroundColorSemiOpaque; }
    void setMainBackgroundColorSemiOpaque( const QColor &c );

    QColor mainTextColor() const { return mMainTextColor; }
    void setMainTextColor( const QColor &c );

    QColor mainTextDisabledColor() const { return mMainTextDisabledColor; }
    void setMainTextDisabledColor( const QColor &c );

    QColor secondaryTextColor() const { return mSecondaryTextColor; }
    void setSecondaryTextColor( const QColor &c );

    QColor controlBackgroundColor() const { return mControlBackgroundColor; }
    void setControlBackgroundColor( const QColor &c );

    QColor controlBackgroundAlternateColor() const { return mControlBackgroundAlternateColor; }
    void setControlBackgroundAlternateColor( const QColor &c );

    QColor controlBackgroundDisabledColor() const { return mControlBackgroundDisabledColor; }
    void setControlBackgroundDisabledColor( const QColor &c );

    QColor controlBorderColor() const { return mControlBorderColor; }
    void setControlBorderColor( const QColor &c );

    QColor buttonTextColor() const { return mButtonTextColor; }
    void setButtonTextColor( const QColor &c );

    QColor toolButtonColor() const { return mToolButtonColor; }
    void setToolButtonColor( const QColor &c );

    QColor toolButtonBackgroundColor() const { return mToolButtonBackgroundColor; }
    void setToolButtonBackgroundColor( const QColor &c );

    QColor toolButtonBackgroundSemiOpaqueColor() const { return mToolButtonBackgroundSemiOpaqueColor; }
    void setToolButtonBackgroundSemiOpaqueColor( const QColor &c );

    QColor scrollBarBackgroundColor() const { return mScrollBarBackgroundColor; }
    void setScrollBarBackgroundColor( const QColor &c );

    QColor groupBoxBackgroundColor() const { return mGroupBoxBackgroundColor; }
    void setGroupBoxBackgroundColor( const QColor &c );

    QColor groupBoxSurfaceColor() const { return mGroupBoxSurfaceColor; }
    void setGroupBoxSurfaceColor( const QColor &c );

    QColor mainColorSemiOpaque() const { return mMainColorSemiOpaque; }
    QColor darkRed() const { return mDarkRed; }
    QColor darkGray() const { return mDarkGray; }
    QColor darkGraySemiOpaque() const { return mDarkGraySemiOpaque; }
    QColor gray() const { return mGray; }
    QColor lightGray() const { return mLightGray; }
    QColor lightestGray() const { return mLightestGray; }
    QColor lightestGraySemiOpaque() const { return mLightestGraySemiOpaque; }
    QColor light() const { return mLight; }
    QColor goodColor() const { return mGoodColor; }
    QColor warningColor() const { return mWarningColor; }
    QColor cloudColor() const { return mCloudColor; }
    QColor positionColor() const { return mPositionColor; }
    QColor positionColorSemiOpaque() const { return mPositionColorSemiOpaque; }
    QColor positionBackgroundColor() const { return mPositionBackgroundColor; }
    QColor darkPositionColor() const { return mDarkPositionColor; }
    QColor darkPositionColorSemiOpaque() const { return mDarkPositionColorSemiOpaque; }
    QColor accuracyBad() const { return mAccuracyBad; }
    QColor accuracyTolerated() const { return mAccuracyTolerated; }
    QColor accuracyExcellent() const { return mAccuracyExcellent; }
    QColor navigationColor() const { return mNavigationColor; }
    QColor navigationColorSemiOpaque() const { return mNavigationColorSemiOpaque; }
    QColor navigationBackgroundColor() const { return mNavigationBackgroundColor; }
    QColor sensorBackgroundColor() const { return mSensorBackgroundColor; }
    QColor bookmarkDefault() const { return mBookmarkDefault; }
    QColor bookmarkOrange() const { return mBookmarkOrange; }
    QColor bookmarkRed() const { return mBookmarkRed; }
    QColor bookmarkBlue() const { return mBookmarkBlue; }
    QColor vertexColor() const { return mVertexColor; }
    QColor vertexColorSemiOpaque() const { return mVertexColorSemiOpaque; }
    QColor vertexSelectedColor() const { return mVertexSelectedColor; }
    QColor vertexSelectedColorSemiOpaque() const { return mVertexSelectedColorSemiOpaque; }
    QColor vertexNewColor() const { return mVertexNewColor; }
    QColor vertexNewColorSemiOpaque() const { return mVertexNewColorSemiOpaque; }
    QColor processingPreview() const { return mProcessingPreview; }

    QColor errorColor() const { return mDarkTheme ? QColor( QStringLiteral( "#df3422" ) ) : QColor( QStringLiteral( "#c0392b" ) ); }

    bool darkTheme() const { return mDarkTheme; }
    void setDarkTheme( bool dark );

    qreal fontScale() const { return mFontScale; }
    void setFontScale( qreal scale );

    QFont defaultFont() const { return makeFont( 1.0, false ); }
    QFont tinyFont() const { return makeFont( 0.75, false ); }
    QFont tipFont() const { return makeFont( 0.875, false ); }
    QFont resultFont() const { return makeFont( 0.8125, false ); }
    QFont strongFont() const { return makeFont( 1.0, true ); }
    QFont strongTipFont() const { return makeFont( 0.875, true ); }
    QFont secondaryTitleFont() const { return makeFont( 1.125, false ); }
    QFont titleFont() const { return makeFont( 1.25, false ); }
    QFont strongTitleFont() const { return makeFont( 1.25, true ); }

    int popupScreenEdgeVerticalMargin() const { return 40; }
    int popupScreenEdgeHorizontalMargin() const { return 20; }
    int menuItemIconlessLeftPadding() const { return 52; }
    int menuItemLeftPadding() const { return 12; }
    int menuItemCheckLeftPadding() const { return 16; }

    enum BaseAppearance
    {
      UseSettingsAppearance,
      SystemAppearance,
      DarkAppearance,
      LightAppearance
    };

  signals:
    void mainColorChanged();
    void mainOverlayColorChanged();
    void mainBackgroundColorChanged();
    void mainBackgroundColorSemiOpaqueChanged();
    void mainTextColorChanged();
    void mainTextDisabledColorChanged();
    void secondaryTextColorChanged();
    void controlBackgroundColorChanged();
    void controlBackgroundAlternateColorChanged();
    void controlBackgroundDisabledColorChanged();
    void controlBorderColorChanged();
    void buttonTextColorChanged();
    void toolButtonColorChanged();
    void toolButtonBackgroundColorChanged();
    void toolButtonBackgroundSemiOpaqueColorChanged();
    void scrollBarBackgroundColorChanged();
    void groupBoxBackgroundColorChanged();
    void groupBoxSurfaceColorChanged();
    void darkThemeChanged();
    void fontScaleChanged();
    void themeDataLoaded();

  private:
    QFont makeFont( qreal scaleFactor, bool bold ) const;
    void loadFromJson();

    QVariantMap mDarkThemeColors;
    QVariantMap mLightThemeColors;

    QColor mMainColor;
    QColor mMainOverlayColor;
    QColor mMainBackgroundColor;
    QColor mMainBackgroundColorSemiOpaque;
    QColor mMainTextColor;
    QColor mMainTextDisabledColor;
    QColor mSecondaryTextColor;
    QColor mControlBackgroundColor;
    QColor mControlBackgroundAlternateColor;
    QColor mControlBackgroundDisabledColor;
    QColor mControlBorderColor;
    QColor mButtonTextColor;
    QColor mToolButtonColor;
    QColor mToolButtonBackgroundColor;
    QColor mToolButtonBackgroundSemiOpaqueColor;
    QColor mScrollBarBackgroundColor;
    QColor mGroupBoxBackgroundColor;
    QColor mGroupBoxSurfaceColor;

    QColor mMainColorSemiOpaque;
    QColor mDarkRed;
    QColor mDarkGray;
    QColor mDarkGraySemiOpaque;
    QColor mGray;
    QColor mLightGray;
    QColor mLightestGray;
    QColor mLightestGraySemiOpaque;
    QColor mLight;
    QColor mGoodColor;
    QColor mWarningColor;
    QColor mCloudColor;
    QColor mPositionColor;
    QColor mPositionColorSemiOpaque;
    QColor mPositionBackgroundColor;
    QColor mDarkPositionColor;
    QColor mDarkPositionColorSemiOpaque;
    QColor mAccuracyBad;
    QColor mAccuracyTolerated;
    QColor mAccuracyExcellent;
    QColor mNavigationColor;
    QColor mNavigationColorSemiOpaque;
    QColor mNavigationBackgroundColor;
    QColor mSensorBackgroundColor;
    QColor mBookmarkDefault;
    QColor mBookmarkOrange;
    QColor mBookmarkRed;
    QColor mBookmarkBlue;
    QColor mVertexColor;
    QColor mVertexColorSemiOpaque;
    QColor mVertexSelectedColor;
    QColor mVertexSelectedColorSemiOpaque;
    QColor mVertexNewColor;
    QColor mVertexNewColorSemiOpaque;
    QColor mProcessingPreview;

    bool mDarkTheme = false;
    qreal mFontScale = 1.0;
    qreal mSystemFontPointSize = 14.0;
    qreal mScreenPpi = 160.0;
};

#endif // THEME_H
