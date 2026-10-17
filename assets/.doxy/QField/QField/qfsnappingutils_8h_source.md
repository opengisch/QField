

# File qfsnappingutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfsnappingutils.h**](qfsnappingutils_8h.md)

[Go to the documentation of this file](qfsnappingutils_8h.md)


```C++
/***************************************************************************
  qfsnappingutils.h

 ---------------------
 begin                : 8.10.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFSNAPPINGUTILS_H
#define QFSNAPPINGUTILS_H

class QgsQuickMapSettings;

#include "qfsnappingresult.h"

#include <qgssnappingutils.h>

class QfSnappingUtils : public QgsSnappingUtils
{
    Q_OBJECT

    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QgsVectorLayer *currentLayer READ currentLayer WRITE setCurrentLayer NOTIFY currentLayerChanged )
    Q_PROPERTY( QfSnappingResult snappingResult READ snappingResult NOTIFY snappingResultChanged )
    Q_PROPERTY( QPointF inputCoordinate READ inputCoordinate WRITE setInputCoordinate NOTIFY inputCoordinateChanged )

  public:
    explicit QfSnappingUtils( QObject *parent = nullptr );

    bool enabled() const;
    void setEnabled( bool enabled );

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *settings );

    QgsVectorLayer *currentLayer() const;
    void setCurrentLayer( QgsVectorLayer *currentLayer );

    QPointF inputCoordinate() const;
    void setInputCoordinate( const QPointF &inputCoordinate );

    QfSnappingResult snappingResult() const;

    static QgsPoint newPoint( const QgsPoint &snappedPoint, const Qgis::WkbType wkbType );

    static Q_INVOKABLE QgsSnappingConfig emptySnappingConfig() { return QgsSnappingConfig(); }

  signals:
    void enabledChanged();
    void mapSettingsChanged();
    void currentLayerChanged();
    void snappingResultChanged();
    void inputCoordinateChanged();

    void indexingStarted( int count );
    void indexingProgress( int index );
    void indexingFinished();

  protected:
    virtual void prepareIndexStarting( int count ) override;
    virtual void prepareIndexProgress( int index ) override;

  private slots:
    void onMapSettingsUpdated();
    void removeOutdatedLocators();

  private:
    void snap();

    bool mEnabled = false;
    QgsQuickMapSettings *mSettings = nullptr;
    QgsVectorLayer *mCurrentLayer = nullptr;

    int mIndexLayerCount;
    QfSnappingResult mSnappingResult;
    QPointF mInputCoordinate;
};


#endif // QFSNAPPINGUTILS_H
```


