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
#include <QStandardItemModel>

class QgisMobileapp;
class QgsRectangle;
class QgsFeature;

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

    Q_INVOKABLE void loadLastProject();
    Q_INVOKABLE void loadFile( const QString &path, const QString &name = QString() );
    Q_INVOKABLE void reloadProject();
    Q_INVOKABLE void readProject();
    Q_INVOKABLE void removeRecentProject( const QString &path );

    Q_INVOKABLE QString readProjectEntry( const QString &scope, const QString &key, const QString &def = QString() ) const;
    Q_INVOKABLE int readProjectNumEntry( const QString &scope, const QString &key, int def = 0 ) const;
    Q_INVOKABLE double readProjectDoubleEntry( const QString &scope, const QString &key, double def = 0.0 ) const;

    Q_INVOKABLE bool print( const QString &layoutName );
    Q_INVOKABLE bool printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds );

    Q_INVOKABLE void setScreenDimmerTimeout( int timeoutSeconds );

    Q_INVOKABLE QVariantMap availableLanguages() const;

    Q_INVOKABLE bool isFileExtensionSupported( const QString &filename ) const;

    /**
     * Adds a log \a message that will be visible to the user through the
     * message log panel, as well as added into the device's system logs
     * which will be captured by the sentry's reporting framework when enabled.
     */
    Q_INVOKABLE void logMessage( const QString &message );

    /**
     * Sends a logs reporting through to sentry when enabled.
     */
    Q_INVOKABLE void sendLog( const QString &message );

    /**
     * Initalizes sentry connection.
     */
    Q_INVOKABLE void initiateSentry() const;

    /**
     * Closes active sentry connection.
     */
    Q_INVOKABLE void closeSentry() const;

    Q_INVOKABLE void restrictImageSize( const QString &imagePath, int maximumWidthHeight );

    static void setInstance( AppInterface *instance ) { sAppInterface = instance; }
    static AppInterface *instance() { return sAppInterface; }

  public slots:
    void openFeatureForm();

  signals:
    void openFeatureFormRequested();

    void loadProjectTriggered( const QString &path, const QString &name );

    void loadProjectEnded( const QString &path, const QString &name );

    void setMapExtent( const QgsRectangle &extent );

    //! Signal emitted requesting QField to open its local data picker screen to show the \a path content
    void openPath( const QString &path );

  private:
    static AppInterface *sAppInterface;

    QgisMobileapp *mApp = nullptr;
};

#endif // APPINTERFACE_H
