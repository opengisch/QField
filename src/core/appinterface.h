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

    Q_INVOKABLE void print( int layoutIndex );

  public slots:
    void openFeatureForm();

  signals:
    void openFeatureFormRequested();

    void loadProjectTriggered( const QString &path, const QString &name );

    void loadProjectEnded( const QString &path, const QString &name );

    void setMapExtent( const QgsRectangle &extent );

  private:
    QgisMobileapp *mApp = nullptr;
};

#endif // APPINTERFACE_H
