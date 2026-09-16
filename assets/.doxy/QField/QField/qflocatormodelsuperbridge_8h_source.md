

# File qflocatormodelsuperbridge.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**qflocatormodelsuperbridge.h**](qflocatormodelsuperbridge_8h.md)

[Go to the documentation of this file](qflocatormodelsuperbridge_8h.md)


```C++
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


class QfLocatorModelSuperBridge : public QgsLocatorModelBridge
{
    Q_OBJECT

    // moc needs the full definition to build the metaobject.
    Q_MOC_INCLUDE( "qffeaturelistextentcontroller.h" )

    
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QObject *geometryHighlighter READ geometryHighlighter WRITE setGeometryHighlighter NOTIFY geometryHighlighterChanged )
    Q_PROPERTY( QfFeatureListExtentController *featureListController READ featureListController WRITE setFeatureListController NOTIFY featureListControllerChanged )
    Q_PROPERTY( QgsMapLayer *activeLayer READ activeLayer WRITE setActiveLayer NOTIFY activeLayerChanged )
    Q_PROPERTY( QfBookmarkModel *bookmarks READ bookmarks WRITE setBookmarks NOTIFY bookmarksChanged )
    Q_PROPERTY( QfNavigation *navigation READ navigation WRITE setNavigation NOTIFY navigationChanged )
    Q_PROPERTY( bool keepScale READ keepScale WRITE setKeepScale NOTIFY keepScaleChanged )

  public:
    explicit QfLocatorModelSuperBridge( QObject *parent = nullptr );
    ~QfLocatorModelSuperBridge() = default;

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QfBookmarkModel *bookmarks() const;
    void setBookmarks( QfBookmarkModel *bookmarks );

    QfNavigation *navigation() const;
    void setNavigation( QfNavigation *navigation );

    QObject *geometryHighlighter() const;
    void setGeometryHighlighter( QObject *geometryHighlighter );

    QfFeatureListExtentController *featureListController() const;
    void setFeatureListController( QfFeatureListExtentController *featureListController );

    QgsMapLayer *activeLayer() const;
    void setActiveLayer( QgsMapLayer *layer );

    bool keepScale() const;
    void setKeepScale( bool keepScale );

    Q_INVOKABLE void requestSearch( const QString &text );

    void requestSearchTextChange( const QString &text );

    Q_INVOKABLE QfLocatorActionsModel *contextMenuActionsModel( const int row );

    Q_INVOKABLE QString getLocatorModelDescription( const int row );

    Q_INVOKABLE QString getPrefixFromSearchString( const QString &string );

    Q_INVOKABLE void registerQFieldLocatorFilter( QfLocatorFilter *filter );

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
    enum Role
    {
      NameRole = Qt::UserRole + 1,
      DescriptionRole,
      PrefixRole,
      ActiveRole,
      DefaultRole,
    };
    Q_ENUM( Role )

    
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
```


