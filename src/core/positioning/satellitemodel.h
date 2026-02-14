/***************************************************************************
  satellitemodel.h - SatelliteModel

 ---------------------
 begin                : 13.02.2026
 copyright            : (C) 2026 by Pascal
 email                :
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SATELLITEMODEL_H
#define SATELLITEMODEL_H

#include <QAbstractListModel>
#include <qgssatelliteinformation.h>

/**
 * \ingroup core
 *
 * Model exposing satellite data from GNSS positioning for use in QML (Skyplot, SNR bars).
 * Wraps a QList<QgsSatelliteInfo> and provides constellation-aware roles.
 */
class SatelliteModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum Role
    {
      SatId = Qt::UserRole,
      Elevation,
      Azimuth,
      Snr,
      InUse,
      Constellation,
    };
    Q_ENUM( Role )

    explicit SatelliteModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    /**
     * Updates the model with a new list of satellite information.
     */
    void updateSatellites( const QList<QgsSatelliteInfo> &satellites );

  private:
    /**
     * Returns a string constellation name from a QgsSatelliteInfo.
     * Uses satType (Qgis::SatelliteSystem) when available, otherwise derives from PRN range.
     */
    static QString constellationName( const QgsSatelliteInfo &info );

    QList<QgsSatelliteInfo> mSatellites;
};

#endif // SATELLITEMODEL_H
