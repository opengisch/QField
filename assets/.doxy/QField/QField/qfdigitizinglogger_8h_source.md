

# File qfdigitizinglogger.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfdigitizinglogger.h**](qfdigitizinglogger_8h.md)

[Go to the documentation of this file](qfdigitizinglogger_8h.md)


```C++
/***************************************************************************
 qfdigitizinglogger.h - QfDigitizingLogger
  ---------------------
 begin                : 7.6.2021
 copyright            : (C) 2021 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFDIGITIZINGLOGGER_H
#define QFDIGITIZINGLOGGER_H

#include "qfcloudconnection.h"
#include "qfgnsspositioninformation.h"
#include "qfsnappingresult.h"
#include "qgsquickmapsettings.h"

#include <QObject>
#include <qgspoint.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

class QfDigitizingLogger : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString type READ type WRITE setType NOTIFY typeChanged )
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QgsVectorLayer *digitizingLayer READ digitizingLayer WRITE setDigitizingLayer NOTIFY digitizingLayerChanged )
    Q_PROPERTY( QfGnssPositionInformation positionInformation READ positionInformation WRITE setPositionInformation NOTIFY positionInformationChanged )
    Q_PROPERTY( bool positionLocked READ positionLocked WRITE setPositionLocked NOTIFY positionLockedChanged )
    Q_PROPERTY( QfSnappingResult topSnappingResult READ topSnappingResult WRITE setTopSnappingResult NOTIFY topSnappingResultChanged )
    Q_PROPERTY( QfCloudUserInformation cloudUserInformation READ cloudUserInformation WRITE setCloudUserInformation NOTIFY cloudUserInformationChanged );

  public:
    explicit QfDigitizingLogger();

    QString type() const { return mType; }

    /*
     * Sets the digitizing logs \a type
     * \note if the type is an empty string, digitized vertices will not be logged
     */
    void setType( const QString &type );

    QgsProject *project() const { return mProject; }

    void setProject( QgsProject *project );

    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QgsVectorLayer *digitizingLayer() const { return mDigitizingLayer; }

    void setDigitizingLayer( QgsVectorLayer *layer );

    QfGnssPositionInformation positionInformation() const { return mPositionInformation; }

    void setPositionInformation( const QfGnssPositionInformation &positionInformation );

    bool positionLocked() const { return mPositionLocked; }

    void setPositionLocked( bool positionLocked );

    QfSnappingResult topSnappingResult() const { return mTopSnappingResult; }

    void setTopSnappingResult( const QfSnappingResult &topSnappingResult );

    QfCloudUserInformation cloudUserInformation() const { return mCloudUserInformation; }

    void setCloudUserInformation( const QfCloudUserInformation &cloudUserInformation );

    Q_INVOKABLE void addCoordinate( const QgsPoint &point );

    Q_INVOKABLE void removeLastCoordinate();

    Q_INVOKABLE void writeCoordinates();

    Q_INVOKABLE void clearCoordinates();

  signals:

    void typeChanged();
    void projectChanged();
    void mapSettingsChanged();
    void digitizingLayerChanged();
    void positionInformationChanged();
    void positionLockedChanged();
    void topSnappingResultChanged();
    void currentCoordinateChanged();
    void cloudUserInformationChanged();

  private:
    void findLogsLayer();

    QString mType;

    QgsProject *mProject = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    QgsVectorLayer *mLogsLayer = nullptr;
    QgsVectorLayer *mDigitizingLayer = nullptr;

    QfGnssPositionInformation mPositionInformation;
    bool mPositionLocked = false;
    QfSnappingResult mTopSnappingResult;
    QfCloudUserInformation mCloudUserInformation;

    QList<QgsFeature> mPointFeatures;
};

#endif // QFDIGITIZINGLOGGER_H
```


