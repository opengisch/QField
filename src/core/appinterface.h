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
class QFieldXmlHttpRequest;

/**
 * \brief App interface made available in QML as `iface`.
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

    /**
     * Imports a compressed project from a given URL and place the content into the Imported Projects
     * folder.
     * \param url the URL where the compressed project ZIP file is
     * \param loadOnImport set to TRUE to load the project on successful import
     */
    Q_INVOKABLE void importUrl( const QString &url, bool loadOnImport = false );

    //! Returns TRUE is a project was passed on when launching QField.
    Q_INVOKABLE bool hasProjectOnLaunch() const;

    /**
     * Loads a project file or standalone dataset.
     *
     * \param path the project file (.qgs or .qgz) or standalone dataset path
     * \param name a project name (if left empty, the project file will be used instead)
     */
    Q_INVOKABLE bool loadFile( const QString &path, const QString &name = QString() );

    //! Reloads the currently opened project.
    Q_INVOKABLE void reloadProject();
    //! Removes a given project \a path from the recent projects list.
    Q_INVOKABLE void removeRecentProject( const QString &path );

    /**
     * Reads a string from the specified scope and key.
     * \param scope	entry scope (group) name
     * \param key	entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values
     * \param def	default value to return if the specified key does not exist within the scope
     */
    Q_INVOKABLE QString readProjectEntry( const QString &scope, const QString &key, const QString &def = QString() ) const;

    /**
     * Reads an integer from the specified scope and key.
     * \param scope	entry scope (group) name
     * \param key	entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values
     * \param def	default value to return if the specified key does not exist within the scope
     */
    Q_INVOKABLE int readProjectNumEntry( const QString &scope, const QString &key, int def = 0 ) const;

    /**
     * Reads a double from the specified scope and key.
     * \param scope	entry scope (group) name
     * \param key	entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values
     * \param def	default value to return if the specified key does not exist within the scope
     */
    Q_INVOKABLE double readProjectDoubleEntry( const QString &scope, const QString &key, double def = 0.0 ) const;

    /**
     * Reads a double from the specified scope and key.
     * \param scope	entry scope (group) name
     * \param key	entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values
     * \param def	default value to return if the specified key does not exist within the scope
     */
    Q_INVOKABLE bool readProjectBoolEntry( const QString &scope, const QString &key, bool def = false ) const;

    /**
     * Prints a project layout to PDF.
     * \param layoutName the layout name
     */
    Q_INVOKABLE bool print( const QString &layoutName );

    /**
     * Prints an atlas-driven project layout to PDF.
     * \param layoutName the layout name
     * \param featureIds the list of atlas feature IDs
     */
    Q_INVOKABLE bool printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds );

    /**
     * Sets the screen drimmer timeout. Dimming can be disabled by setting the timeout to zero.
     * \param timeoutSeconds timeout in seconds
     */
    Q_INVOKABLE void setScreenDimmerTimeout( int timeoutSeconds );

    //! Returns a list of available UI translation languages
    Q_INVOKABLE QVariantMap availableLanguages() const;

    /**
     * Changes the application language to the specified \a languageCode.
     * This will reload translators and refresh all QML translations without restarting the app.
     * \param languageCode The language code (e.g., "en", "de")
     * \see availableLanguages
     */
    Q_INVOKABLE void changeLanguage( const QString &languageCode );

    //! Returns TRUE if a given \a filename can be opened as a project or standalone dataset.
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
     * Returns the main map canvas item.
     * \see MapCanvas
     */
    Q_INVOKABLE QObject *mapCanvas() const;

    /**
     * Returns the positioning item.
     * \see Positioning
     */
    Q_INVOKABLE QObject *positioning() const;


    //! One-shot xmlhttp request. Defaults to autoDelete = true.
    Q_INVOKABLE QObject *createHttpRequest() const;

    /// @cond PRIVATE
    //! Reads the content of the loaded project, called on loadProjectTriggered()
    Q_INVOKABLE void readProject();

    static void setInstance( AppInterface *instance ) { sAppInterface = instance; }
    static AppInterface *instance() { return sAppInterface; }
    ///@endcond

  signals:
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
