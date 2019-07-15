/***************************************************************************
  snappingutils.h

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

#ifndef SNAPPINGUTILS_H
#define SNAPPINGUTILS_H

class QgsQuickMapSettings;

#include <qgssnappingutils.h>

#include "snappingresult.h"

class SnappingUtils : public QgsSnappingUtils
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QgsVectorLayer *currentLayer READ currentLayer WRITE setCurrentLayer NOTIFY currentLayerChanged )
    Q_PROPERTY( SnappingResult snappingResult READ snappingResult NOTIFY snappingResultChanged )
    Q_PROPERTY( QPointF inputCoordinate READ inputCoordinate WRITE setInputCoordinate NOTIFY inputCoordinateChanged )

  public:
    SnappingUtils( QObject *parent = nullptr );

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *settings );

    QgsVectorLayer *currentLayer() const;
    void setCurrentLayer( QgsVectorLayer *currentLayer );

    QPointF inputCoordinate() const;
    void setInputCoordinate( const QPointF &inputCoordinate );

    SnappingResult snappingResult() const;

    static QgsPoint newPoint( const QgsPoint &snappedPoint, const QgsWkbTypes::Type wkbType );

  signals:
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

  private:
    void snap();

    QgsQuickMapSettings *mSettings;
    QgsVectorLayer *mCurrentLayer;

    int mIndexLayerCount;
    SnappingResult mSnappingResult;
    QPointF mInputCoordinate;
};


#endif // SNAPPINGUTILS_H
