

# File qfpositioninginformationmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfpositioninginformationmodel.h**](qfpositioninginformationmodel_8h.md)

[Go to the documentation of this file](qfpositioninginformationmodel_8h.md)


```C++
#ifndef QFPOSITIONINGINFORMATIONMODEL_H
#define QFPOSITIONINGINFORMATIONMODEL_H

#include "qfpositioning.h"

#include <QObject>
#include <QStandardItemModel>

class QfPositioningInformationModel : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY( QfPositioning *positioningSource READ positioningSource WRITE setPositioningSource NOTIFY positioningSourceChanged )
    Q_PROPERTY( double antennaHeight READ antennaHeight WRITE setAntennaHeight NOTIFY antennaHeightChanged )
    Q_PROPERTY( Qgis::DistanceUnit distanceUnits READ distanceUnits WRITE setDistanceUnits NOTIFY distanceUnitsChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem coordinateDisplayCrs READ coordinateDisplayCrs WRITE setCoordinateDisplayCrs NOTIFY coordinateDisplayCrsChanged )

  public:
    enum Roles
    {
      NameRole = Qt::UserRole,
      ValueRole = Qt::UserRole + 1,
    };

    QfPositioningInformationModel( QObject *parent = nullptr );

    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QHash<int, QByteArray> roleNames() const override;

    void refreshData();

    void updateInfo( const QString &name, const QVariant &value );

    QfPositioning *positioningSource() const;

    void setPositioningSource( QfPositioning *positioningSource );

    double antennaHeight() const;

    void setAntennaHeight( double antennaHeight );

    Qgis::DistanceUnit distanceUnits() const;

    void setDistanceUnits( Qgis::DistanceUnit distanceUnits );

    QgsCoordinateReferenceSystem coordinateDisplayCrs() const;

    void setCoordinateDisplayCrs( const QgsCoordinateReferenceSystem &coordinateDisplayCrs );

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
    void softReset();
    void updateCoordinates();
    QString getAltitude( double distanceUnitFactor, const QString &distanceUnitAbbreviation );
    QString getSpeed( double distanceUnitFactor, const QString &distanceUnitAbbreviation );
    QString getHorizontalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation );
    QString getVerticalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation );

  private:
    QfPositioning *mPositioningSource = nullptr;
    double mAntennaHeight = -1;
    Qgis::DistanceUnit mDistanceUnits;
    QgsCoordinateReferenceSystem mCoordinateDisplayCrs;
    QMetaObject::Connection positioningSourceConnection;
};

#endif // QFPOSITIONINGINFORMATIONMODEL_H
```


