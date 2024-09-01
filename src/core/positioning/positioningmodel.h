#pragma once

#include "positioning.h"

#include <QObject>
#include <QStandardItemModel>

class PositioningModel : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY( Positioning *positioningSource READ positioningSource WRITE setPositioningSource NOTIFY positioningSourceChanged )
    Q_PROPERTY( double antennaHeight READ antennaHeight WRITE setAntennaHeight NOTIFY antennaHeightChanged )
    Q_PROPERTY( Qgis::DistanceUnit distanceUnits READ distanceUnits WRITE setDistanceUnits NOTIFY distanceUnitsChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem coordinateDisplayCrs READ coordinateDisplayCrs WRITE setCoordinateDisplayCrs NOTIFY coordinateDisplayCrsChanged )


  public:
    enum Roles
    {
      VariableNameRole = Qt::UserRole,
      VariableValueRole = Qt::UserRole + 1,
    };

    PositioningModel( QObject *parent = nullptr );

    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setupConnections();
    void refreshData();
    void updateInfo( const QString &name, const QVariant &value );


    Positioning *positioningSource() const;
    void setPositioningSource( Positioning *newPositioningSource );


    double antennaHeight() const;
    void setAntennaHeight( double newAntennaHeight );

    Qgis::DistanceUnit distanceUnits() const;
    void setDistanceUnits( Qgis::DistanceUnit newDistanceUnits );

    QgsCoordinateReferenceSystem coordinateDisplayCrs() const;
    void setCoordinateDisplayCrs( const QgsCoordinateReferenceSystem &newCoordinateDisplayCrs );

  signals:
    void positioningSourceChanged();
    void antennaHeightChanged();
    void distanceUnitsChanged();
    void coordinateDisplayCrsChanged();

  private slots:
    void onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles );

  private:
    void getCoordinateLabels( QString &coord1Label, QString &coord2Label, bool coordinatesIsXY, bool isGeographic );
    void getCoordinateValues( QString &coord1Value, QString &coord2Value, const QgsPoint &coordinates, bool coordinatesIsXY, bool isGeographic );
    QString getAltitude( double distanceUnitFactor, const QString &distanceUnitAbbreviation );
    QString getSpeed();
    QString getHorizontalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation );
    QString getVerticalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation );

  private:
    Positioning *mPositioningSource = nullptr;
    double mAntennaHeight = -1;
    Qgis::DistanceUnit mDistanceUnits;
    QgsCoordinateReferenceSystem mCoordinateDisplayCrs;
};
