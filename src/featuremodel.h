/***************************************************************************
                            featuremodel.h
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

#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include <QAbstractListModel>
#include <QGeoPositionInfoSource>
#include <memory>
#include "geometry.h"
#include "qgsquickfeaturemodel.h"

class FeatureModel : public QgsQuickFeatureModel
{
    Q_OBJECT
    Q_PROPERTY( Geometry* geometry MEMBER mGeometry NOTIFY geometryChanged )
    Q_PROPERTY( QString positionSourceName READ positionSourceName WRITE setPositionSourceName NOTIFY positionSourceChanged )

  public:
    explicit FeatureModel( QObject *parent = 0 );
    explicit FeatureModel( const QgsFeature& feat, QObject *parent = 0 );

    /**
     * The name of the position source to use.
     * A QGeoPositionInfoSource is created internally based on this name.
     */
    QString positionSourceName() const;

    /**
     * The name of the position source to use.
     * A QGeoPositionInfoSource is created internally based on this name.
     */
    void setPositionSourceName( const QString &positionSourceName );

    Q_INVOKABLE void resetAttributes() override;

  public slots:
    void applyGeometry();

  signals:
    void geometryChanged();
    void positionSourceChanged();

    void warning( const QString& text );

  private:
    Geometry* mGeometry;
    std::unique_ptr<QGeoPositionInfoSource> mPositionSource;
    QString mTempName;
};

#endif // FEATUREMODEL_H
