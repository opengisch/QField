/***************************************************************************
                            appinterface.h
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <QObject>
#include <QPointF>
#include <QQmlComponent>
#include <QStandardItemModel>

class QgisMobileapp;
class QgsRectangle;
class QgsFeature;
class QQuickItem;

/**
 * \ingroup core
 */
class AppInterface : public QObject
{
    Q_OBJECT

  public:
    explicit AppInterface( QgisMobileapp *app );
    AppInterface()
    {
      // You shouldn't get here, this constructor only exists that we can register it as a QML type
      Q_ASSERT( false );
    }

    Q_INVOKABLE void importUrl( const QString &url, bool loadOnImport = false );

    Q_INVOKABLE bool hasProjectOnLaunch() const;
    Q_INVOKABLE bool loadFile( const QString &path, const QString &name = QString() );
    Q_INVOKABLE void reloadProject();
    Q_INVOKABLE void readProject();
    Q_INVOKABLE void removeRecentProject( const QString &path );

    Q_INVOKABLE QString readProjectEntry( const QString &scope, const QString &key, const QString &def = QString() ) const;
    Q_INVOKABLE int readProjectNumEntry( const QString &scope, const QString &key, int def = 0 ) const;
    Q_INVOKABLE double readProjectDoubleEntry( const QString &scope, const QString &key, double def = 0.0 ) const;
    Q_INVOKABLE bool readProjectBoolEntry( const QString &scope, const QString &key, bool def = false ) const;

    Q_INVOKABLE bool print( const QString &layoutName );
    Q_INVOKABLE bool printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds );

    Q_INVOKABLE void setScreenDimmerTimeout( int timeoutSeconds );

    Q_INVOKABLE QVariantMap availableLanguages() const;

    Q_INVOKABLE bool isFileExtensionSupported( const QString &filename ) const;

    /**
     * Returns TRUE if the authentication configuration \a id is available.
     */
    Q_INVOKABLE bool isAuthenticationConfigurationAvailable( const QString &id ) const;

    /**
     * Adds a log \a message that will be visible to the user through the
     * message log panel, as well as added into the device's system logs
     * which will be captured by the sentry's reporting framework when enabled.
     */
    Q_INVOKABLE void logMessage( const QString &message );

    /**
     * Outputs the current runtime profiler model content into the message log
     * panel, as well as added into the device's system logs
     * which will be captured by the sentry's reporting framework when enabled.
     */
    Q_INVOKABLE void logRuntimeProfiler();

    /**
     * Sends a logs reporting through to sentry when enabled.
     */
    Q_INVOKABLE void sendLog( const QString &message, const QString &cloudUser );

    /**
     * Initalizes sentry connection.
     */
    Q_INVOKABLE void initiateSentry() const;

    /**
     * Closes active sentry connection.
     */
    Q_INVOKABLE void closeSentry() const;

    /**
     * Clears the currently opened project
     */
    Q_INVOKABLE void clearProject() const;

    /**
     * Returns the item matching the provided object \a name
     */
    Q_INVOKABLE QObject *findItemByObjectName( const QString &name ) const;

    /**
     * Adds an \a item in the plugins toolbar container
     */
    Q_INVOKABLE void addItemToPluginsToolbar( QQuickItem *item ) const;

    /**
     * Adds an \a item in the map canvas menu's action toolbar container
     */
    Q_INVOKABLE void addItemToCanvasActionsToolbar( QQuickItem *item ) const;

    /**
     * Adds an \a item in the dashboard's action toolbar container
     */
    Q_INVOKABLE void addItemToDashboardActionsToolbar( QQuickItem *item ) const;

    /**
     * Adds an \a item in the dashboard's action toolbar container
     * \note This function is deprecated and will be removed in the future, use
     * the addItemToDashboardActionsToolbar function instead
     */
    Q_INVOKABLE void addItemToMainMenuActionsToolbar( QQuickItem *item ) const;

    /**
     * Returns the main window.
     */
    Q_INVOKABLE QObject *mainWindow() const;

    /**
     * Returns the main map canvas.
     */
    Q_INVOKABLE QObject *mapCanvas() const;

    /**
     * Returns the main map canvas.
     */
    Q_INVOKABLE QObject *positioning() const;

    static void setInstance( AppInterface *instance ) { sAppInterface = instance; }
    static AppInterface *instance() { return sAppInterface; }

  public slots:
    void openFeatureForm();

  signals:
    void openFeatureFormRequested();

    /**
     * Emitted when a dataset or project import has been triggered.
     * \param name a indentifier-friendly string (e.g. a file being imported)
     */
    void importTriggered( const QString &name );

    /**
     * Emitted when an ongoing import reports its \a progress.
     * \note when an import is started, its progress will be indefinite by default
     */
    void importProgress( double progress );

    /**
     * Emitted when an import has ended.
     * \param path the path within which the imported dataset or project has been copied into
     * \note if the import was not successful, the path value will be an empty string
     */
    void importEnded( const QString &path = QString() );

    /**
     * Emitted when a project has begin loading.
     */
    void loadProjectTriggered( const QString &path, const QString &name );

    /**
     * Emitted when a project loading has ended.
     */
    void loadProjectEnded( const QString &path, const QString &name );

    //! Requests QField to set its map to the provided \a extent.
    void setMapExtent( const QgsRectangle &extent );

    //! Requests QField to open its local data picker screen to show the \a path content.
    void openPath( const QString &path );

    //! Requests QField to execute a given \a action.
    void executeAction( const QString &action );

    //! Emitted when a volume key is pressed while QField is set to handle those keys.
    void volumeKeyDown( int volumeKeyCode );

    //! Emitted when a volume key is pressed while QField is set to handle those keys.
    void volumeKeyUp( int volumeKeyCode );

  private:
    static AppInterface *sAppInterface;

    QgisMobileapp *mApp = nullptr;
};

#endif // APPINTERFACE_H
