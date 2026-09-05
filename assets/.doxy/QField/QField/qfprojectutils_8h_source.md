

# File qfprojectutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfprojectutils.h**](qfprojectutils_8h.md)

[Go to the documentation of this file](qfprojectutils_8h.md)


```C++
/***************************************************************************
  qfprojectutils.h - QfProjectUtils

 ---------------------
 begin                : 19.04.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFPROJECTUTILS_H
#define QFPROJECTUTILS_H

#include "qfgnsspositioninformation.h"

#include <QObject>
#include <qgsproject.h>

class QfProjectUtils : public QObject
{
    Q_OBJECT

  public:
    explicit QfProjectUtils( QObject *parent = nullptr );

    Q_INVOKABLE static QVariantMap mapLayers( QgsProject *project );

    Q_INVOKABLE static bool addMapLayer( QgsProject *project, QgsMapLayer *layer );

    Q_INVOKABLE static void removeMapLayer( QgsProject *project, QgsMapLayer *layer );

    Q_INVOKABLE static void removeMapLayer( QgsProject *project, const QString &layerId );

    Q_INVOKABLE static Qgis::TransactionMode transactionMode( QgsProject *project = nullptr );

    Q_INVOKABLE static QString title( QgsProject *project = nullptr );

    Q_INVOKABLE static QString createProject( const QVariantMap &options, const QfGnssPositionInformation &positionInformation = QfGnssPositionInformation() );
};

#endif // QFPROJECTUTILS_H
```


