#ifndef POSITIONINGINFORMATIONMODEL_H
#define POSITIONINGINFORMATIONMODEL_H

#include "positioning.h"

#include <QObject>
#include <QStandardItemModel>

/**
 * \ingroup core
 */
class PositioningInformationModel : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY( Positioning *positioningSource READ positioningSource WRITE setPositioningSource NOTIFY positioningSourceChanged )
    Q_PROPERTY( double antennaHeight READ antennaHeight WRITE setAntennaHeight NOTIFY antennaHeightChanged )
    Q_PROPERTY( Qgis::DistanceUnit distanceUnits READ distanceUnits WRITE setDistanceUnits NOTIFY distanceUnitsChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem coordinateDisplayCrs READ coordinateDisplayCrs WRITE setCoordinateDisplayCrs NOTIFY coordinateDisplayCrsChanged )

  public:
    enum Roles
    {
      NameRole = Qt::UserRole,
      ValueRole = Qt::UserRole + 1,
    };

    PositioningInformationModel( QObject *parent = nullptr );

    /**
     * @brief Sets the data for the given index
     * @param index The index to set the data for
     * @param value The new value
     * @param role The role of the data
     */
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    /**
     * @brief Returns the names of the roles used by the model
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Refreshes the data in the model
     */
    void refreshData();

    /**
     * @brief Updates the information in the model with the given name and value
     * @param name The name of the information to update
     * @param value The new value
     */
    void updateInfo( const QString &name, const QVariant &value );

    /**
     * @brief Returns the positioning source
     */
    Positioning *positioningSource() const;

    /**
     * @brief Updates the positioning source and reconnects the positionInformationChanged signal to refreshData, while disconnecting the previous connection.
     * @param positioningSource The new positioning source to update with
     */
    void setPositioningSource( Positioning *positioningSource );

    /**
     * @brief Returns the antenna height
     */
    double antennaHeight() const;

    /**
     * @brief Sets the antenna height
     * @param antennaHeight The new antenna height
     */
    void setAntennaHeight( double antennaHeight );

    /**
     * @brief Returns the distance units
     */
    Qgis::DistanceUnit distanceUnits() const;

    /**
     * @brief Sets the distance units
     * @param distanceUnits The new distance units
     */
    void setDistanceUnits( Qgis::DistanceUnit distanceUnits );

    /**
     * @brief Returns the coordinate display CRS
     */
    QgsCoordinateReferenceSystem coordinateDisplayCrs() const;

    /**
     * @brief Sets the coordinate display CRS
     * @param coordinateDisplayCrs The new CRS
     */
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
    QString getSpeed();
    QString getHorizontalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation );
    QString getVerticalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation );

  private:
    Positioning *mPositioningSource = nullptr;
    double mAntennaHeight = -1;
    Qgis::DistanceUnit mDistanceUnits;
    QgsCoordinateReferenceSystem mCoordinateDisplayCrs;
    QMetaObject::Connection positioningSourceConnection;
};

#endif // POSITIONINGINFORMATIONMODEL_H
