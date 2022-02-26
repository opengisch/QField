/***************************************************************************
 navigation.h - Navigation

 ---------------------
 begin                : 22.02.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "qgsquickmapsettings.h"
#include "navigationmodel.h"

#include <QObject>

class Navigation : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( NavigationModel *model READ model() NOTIFY modelChanged )

    Q_PROPERTY( QgsPoint location READ location WRITE setLocation NOTIFY locationChanged )
    Q_PROPERTY( QgsPoint destination READ destination WRITE setDestination NOTIFY destinationChanged )
    Q_PROPERTY( QgsGeometry path READ path NOTIFY pathChanged )

  public:
    Navigation();

    ~Navigation();

    void setMapSettings( QgsQuickMapSettings *mapSettings );
    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

    NavigationModel *model() const { return mModel.get(); }

    QgsPoint location() const;
    void setLocation( const QgsPoint &point );

    QgsPoint destination() const;
    void setDestination( const QgsPoint &point );

    QgsGeometry path() const { return mPath; }

  signals:
    void mapSettingsChanged();
    void modelChanged();

    void locationChanged();
    void destinationChanged();

    void pathChanged();

  private slots:
    void crsChanged();

  private:
    void updatePath();

    std::unique_ptr<NavigationModel> mModel = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    QgsPoint mLocation;
    QgsGeometry mPath;
};

#endif // NAVIGATION_H
