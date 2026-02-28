

# File appinterface.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**appinterface.h**](appinterface_8h.md)

[Go to the documentation of this file](appinterface_8h.md)


```C++
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
    Q_INVOKABLE void removeRecentProject( const QString &path );

    Q_INVOKABLE QString readProjectEntry( const QString &scope, const QString &key, const QString &def = QString() ) const;

    Q_INVOKABLE int readProjectNumEntry( const QString &scope, const QString &key, int def = 0 ) const;

    Q_INVOKABLE double readProjectDoubleEntry( const QString &scope, const QString &key, double def = 0.0 ) const;

    Q_INVOKABLE bool readProjectBoolEntry( const QString &scope, const QString &key, bool def = false ) const;

    Q_INVOKABLE bool print( const QString &layoutName );

    Q_INVOKABLE bool printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds );

    Q_INVOKABLE void setScreenDimmerTimeout( int timeoutSeconds );

    Q_INVOKABLE QVariantMap availableLanguages() const;

    Q_INVOKABLE void changeLanguage( const QString &languageCode );

    Q_INVOKABLE bool isFileExtensionSupported( const QString &filename ) const;

    Q_INVOKABLE bool isAuthenticationConfigurationAvailable( const QString &id ) const;

    Q_INVOKABLE void logMessage( const QString &message );

    Q_INVOKABLE void logRuntimeProfiler();

    Q_INVOKABLE void sendLog( const QString &message, const QString &cloudUser );

    Q_INVOKABLE void initiateSentry() const;

    Q_INVOKABLE void closeSentry() const;

    Q_INVOKABLE void clearProject() const;

    Q_INVOKABLE QObject *findItemByObjectName( const QString &name ) const;

    Q_INVOKABLE void addItemToPluginsToolbar( QQuickItem *item ) const;

    Q_INVOKABLE void addItemToCanvasActionsToolbar( QQuickItem *item ) const;

    Q_INVOKABLE void addItemToDashboardActionsToolbar( QQuickItem *item ) const;

    Q_INVOKABLE void addItemToMainMenuActionsToolbar( QQuickItem *item ) const;

    Q_INVOKABLE QObject *mainWindow() const;

    Q_INVOKABLE QObject *mapCanvas() const;

    Q_INVOKABLE QObject *positioning() const;


    Q_INVOKABLE QObject *createHttpRequest() const;

    Q_INVOKABLE void readProject();

    static void setInstance( AppInterface *instance ) { sAppInterface = instance; }
    static AppInterface *instance() { return sAppInterface; }

  signals:
    void importTriggered( const QString &name );

    void importProgress( double progress );

    void importEnded( const QString &path = QString() );

    void loadProjectTriggered( const QString &path, const QString &name );

    void loadProjectEnded( const QString &path, const QString &name );

    void setMapExtent( const QgsRectangle &extent );

    void openPath( const QString &path );

    void executeAction( const QString &action );

    void volumeKeyDown( int volumeKeyCode );

    void volumeKeyUp( int volumeKeyCode );

  private:
    static AppInterface *sAppInterface;

    QgisMobileapp *mApp = nullptr;
};

#endif // APPINTERFACE_H
```


