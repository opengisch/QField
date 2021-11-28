/***************************************************************************
  scalebarmeasurement.h - ScaleBarMeasurement

 ---------------------
 begin                : 9.10.2021
 copyright            : (C) 2021 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SCALEBARMEASUREMENT_H
#define SCALEBARMEASUREMENT_H

#include <QObject>
#include <qgsdistancearea.h>
#include <qgsquickmapsettings.h>

class QgsProject;

class ScaleBarMeasurement : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    Q_PROPERTY( double screenLength READ screenLength NOTIFY screenLengthChanged )
    Q_PROPERTY( QString label READ label NOTIFY labelChanged )

    Q_PROPERTY( double referenceScreenLength READ referenceScreenLength WRITE setReferenceScreenLength NOTIFY referenceScreenLengthChanged )

  public:
    explicit ScaleBarMeasurement( QObject *parent = nullptr );

    double referenceScreenLength() const;
    void setReferenceScreenLength( double referenceScreenLength );

    double screenLength() const;
    QString label() const;

    QgsProject *project() const;
    void setProject( QgsProject *project );

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

  signals:
    void projectChanged();
    void mapSettingsChanged();

    void screenLengthChanged();
    void labelChanged();

    void referenceScreenLengthChanged();

  private slots:

    void resetDistanceArea();
    void measure();

  private:
    QgsProject *mProject = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;

    QgsDistanceArea mDistanceArea;

    double mScreenLength = 0.0;

    QString mLabel;

    double mReferenceScreenLength = 300.0;
};

#endif // SCALEBARMEASUREMENT_H
