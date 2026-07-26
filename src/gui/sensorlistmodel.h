/***************************************************************************
 sensorlistmodel.h
 ---------------------
 begin                : 25.03.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu at opengisch dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SENSORLISTMODEL_H
#define SENSORLISTMODEL_H

#include "qgssensormodel.h"

#include <QSortFilterProxyModel>
#include <qobjectuniqueptr.h>

class QgsProject;
class QgsVectorLayer;

/**
 * \ingroup core
 */
class SensorListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( bool showConnectedOnly READ showConnectedOnly WRITE setShowConnectedOnly NOTIFY showConnectedOnlyChanged )

  public:
    explicit SensorListModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    //! Returns the current project from which sensors are retrieved.
    QgsProject *project() const;

    //! Sets the project to be used to retrieve sensors.
    void setProject( QgsProject *project );

    //! Returns TRUE when the model only shows connected sensors.
    bool showConnectedOnly() const { return mShowConnectedOnly; }

    //! Sets whether the model should only show connected sensors.
    void setShowConnectedOnly( bool showConnectedOnly );

    /**
     * Connects a sensor.
     * \param id The unique ID string of the sensor
     */
    Q_INVOKABLE void connectSensorId( const QString &id ) const;

    /**
     * Disconnects a sensor.
     * \param id The unique ID string of the sensor
     */
    Q_INVOKABLE void disconnectSensorId( const QString &id ) const;

  protected:
    bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  signals:

    //! Emitted when the current project has changed.
    void projectChanged();

    //! Emitted when the show connected only filter has changed.
    void showConnectedOnlyChanged();

    //! Emitted when a sensor error has occurred. An \a errorString provides details on the error.
    void sensorErrorOccurred( const QString &errorString );

  private:
    void handleSensorError( const QString &id );

    QgsProject *mProject = nullptr;
    QObjectUniquePtr<QgsSensorModel> mSensorModel;

    bool mShowConnectedOnly = false;
};

#endif // SENSORLISTMODEL_H
