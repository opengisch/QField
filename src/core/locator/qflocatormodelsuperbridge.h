/***************************************************************************
  qflocatormodelsuperbridge.h

 ---------------------
 begin                : 01.12.2018
 copyright            : (C) 2018 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFLOCATORMODELSUPERBRIDGE_H
#define QFLOCATORMODELSUPERBRIDGE_H

#include "qfbookmarkmodel.h"
#include "qfnavigation.h"

#include <QStandardItemModel>
#include <qgslocatorfilter.h>
#include <qgslocatormodelbridge.h>

class QgsQuickMapSettings;
class QfFeatureListExtentController;
class QfPeliasGeocoder;
class QfGnssPositionInformation;
class QfLocatorFilter;
class QgsLocator;

/**
 * QfLocatorActionsModel is a model used to dislay
 * additional actions from the result of a filter.
 * These are displayed as contextual menu in QGIS
 * while they are shown in a row of buttons in QField.
 */
class QfLocatorActionsModel : public QStandardItemModel
{
    Q_OBJECT
  public:
    enum ActionRoles
    {
      IdRole = Qt::UserRole + 1,
      IconPathRole
    };
    explicit QfLocatorActionsModel( QObject *parent = nullptr );
    QfLocatorActionsModel( int rows, int columns, QObject *parent = nullptr );
    QHash<int, QByteArray> roleNames() const override;
};


/**
 * QfLocatorModelSuperBridge reimplements QgsLocatorModelBridge
 * for specific needs of QField / QML implementation.
 */
class QfLocatorModelSuperBridge : public QgsLocatorModelBridge
{
    Q_OBJECT

    // moc needs the full definition to build the metaobject.
    Q_MOC_INCLUDE( "qffeaturelistextentcontroller.h" )

    //! The current project's map settings
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    //! The locator highlight geometry object through which locator actions can highhlight features
    Q_PROPERTY( QObject *geometryHighlighter READ geometryHighlighter WRITE setGeometryHighlighter NOTIFY geometryHighlighterChanged )
    //! The feature list extent controller
    Q_PROPERTY( QfFeatureListExtentController *featureListController READ featureListController WRITE setFeatureListController NOTIFY featureListControllerChanged )
    //! The current project's active layer
    Q_PROPERTY( QgsMapLayer *activeLayer READ activeLayer WRITE setActiveLayer NOTIFY activeLayerChanged )
    //! The bookmark manager containing user and current project bookmarks
    Q_PROPERTY( QfBookmarkModel *bookmarks READ bookmarks WRITE setBookmarks NOTIFY bookmarksChanged )
    //! The navigation object from which destination can be set or modified
    Q_PROPERTY( QfNavigation *navigation READ navigation WRITE setNavigation NOTIFY navigationChanged )
    //! The keep scale flag. When turned on, locator actions should not result in changed scale
    Q_PROPERTY( bool keepScale READ keepScale WRITE setKeepScale NOTIFY keepScaleChanged )

  public:
    explicit QfLocatorModelSuperBridge( QObject *parent = nullptr );
    ~QfLocatorModelSuperBridge() = default;

    //! \copydoc QfLocatorModelSuperBridge::mapSettings
    QgsQuickMapSettings *mapSettings() const;
    //! \copydoc QfLocatorModelSuperBridge::mapSettings
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! \copydoc QfLocatorModelSuperBridge::bookmarks
    QfBookmarkModel *bookmarks() const;
    //! \copydoc QfLocatorModelSuperBridge::bookmarks
    void setBookmarks( QfBookmarkModel *bookmarks );

    //! \copydoc QfLocatorModelSuperBridge::navigation
    QfNavigation *navigation() const;
    //! \copydoc QfLocatorModelSuperBridge::navigation
    void setNavigation( QfNavigation *navigation );

    //! \copydoc QfLocatorModelSuperBridge::geometryHighlighter
    QObject *geometryHighlighter() const;
    //! \copydoc QfLocatorModelSuperBridge::geometryHighlighter
    void setGeometryHighlighter( QObject *geometryHighlighter );

    //! \copydoc QfLocatorModelSuperBridge::featureListController
    QfFeatureListExtentController *featureListController() const;
    //! \copydoc QfLocatorModelSuperBridge::featureListController
    void setFeatureListController( QfFeatureListExtentController *featureListController );

    //! \copydoc QfLocatorModelSuperBridge::activeLayer
    QgsMapLayer *activeLayer() const;
    //! \copydoc QfLocatorModelSuperBridge::activeLayer
    void setActiveLayer( QgsMapLayer *layer );

    //! \copydoc QfLocatorModelSuperBridge::keepScale
    bool keepScale() const;
    //! \copydoc QfLocatorModelSuperBridge::keepScale
    void setKeepScale( bool keepScale );

    /**
     * Requests a \a text query against the search bar.
     */
    Q_INVOKABLE void requestSearch( const QString &text );

    /**
     * Requests for the current text in the search bar to be changed to the
     * provided \a text string.
     */
    void requestSearchTextChange( const QString &text );

    /**
     * Returns the actions model for a given locator search result list item.
     */
    Q_INVOKABLE QfLocatorActionsModel *contextMenuActionsModel( const int row );

    /**
     * Returns the description for a given locator search result list item.
     */
    Q_INVOKABLE QString getLocatorModelDescription( const int row );

    /**
     * Looks for and if present returns the locator filter prefix from
     * a given search string. If not prefix is detected, an empty
     * string will be returned.
     */
    Q_INVOKABLE QString getPrefixFromSearchString( const QString &string );

    /**
     * Registers a given \a filter with the locator.
     */
    Q_INVOKABLE void registerQFieldLocatorFilter( QfLocatorFilter *filter );

    /**
     * Deregisters a given \a filter with the locator.
     */
    Q_INVOKABLE void deregisterQFieldLocatorFilter( QfLocatorFilter *filter );

    void emitMessage( const QString &text );

  signals:
    void mapSettingsChanged();
    void bookmarksChanged();
    void navigationChanged();
    void geometryHighlighterChanged();
    void featureListControllerChanged();
    void activeLayerChanged();
    void messageEmitted( const QString &text );
    void keepScaleChanged();
    void searchRequested( const QString &text );
    void searchTextChangeRequested( const QString &text );
    void locatorFiltersChanged();
    void requestJumpToPoint( const QgsPoint &center, const double &scale = -1.0, bool handleMargins = false );

  public slots:
    Q_INVOKABLE void triggerResultAtRow( const int row, const int id = -1 );

  private:
    QgsQuickMapSettings *mMapSettings = nullptr;
    QObject *mGeometryHighlighter = nullptr;
    QfFeatureListExtentController *mFeatureListController = nullptr;
    QPointer<QgsMapLayer> mActiveLayer;
    bool mKeepScale = false;

    QfPeliasGeocoder *mFinlandGeocoder = nullptr;
    QfBookmarkModel *mBookmarks = nullptr;
    QfNavigation *mNavigation = nullptr;
};

class QfLocatorFiltersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( QfLocatorModelSuperBridge *locatorModelSuperBridge READ locatorModelSuperBridge WRITE setLocatorModelSuperBridge NOTIFY locatorModelSuperBridgeChanged )

  public:
    //! Custom model roles
    enum Role
    {
      NameRole = Qt::UserRole + 1,
      DescriptionRole,
      PrefixRole,
      ActiveRole,
      DefaultRole,
    };
    Q_ENUM( Role )

    /**
     * Constructor for QgsLocatorFiltersModel.
     */
    QfLocatorFiltersModel();

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    QgsLocatorFilter *filterForIndex( const QModelIndex &index ) const;

    QfLocatorModelSuperBridge *locatorModelSuperBridge() const;
    void setLocatorModelSuperBridge( QfLocatorModelSuperBridge *locatorModelSuperBridge );

    Q_INVOKABLE void setGeocoderLocatorFiltersDefaulByPosition( const QfGnssPositionInformation &position );

  signals:
    void locatorModelSuperBridgeChanged();

  private slots:
    void locatorFiltersChanged();

  private:
    QfLocatorModelSuperBridge *mLocatorModelSuperBridge = nullptr;
};
#endif // QFLOCATORMODELSUPERBRIDGE_H
