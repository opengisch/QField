import QtQuick 2.3
import QtTest 1.0
import org.qfield 1.0

TestCase {
  name: "Positioning"

  Positioning {
    id: positioning
    deviceId: 'udp:localhost:1958'
    active: true
    elevationCorrectionMode: Positioning.ElevationCorrectionMode.None

    coordinateTransformer: CoordinateTransformer {
      id: coordinateTransformer
      destinationCrs: CoordinateReferenceSystemUtils.wgs84Crs()
      transformContext: CoordinateReferenceSystemUtils.emptyTransformContext()
      deltaZ: 0
      skipAltitudeTransformation: false
      verticalGrid: ''
    }
  }

  FeatureModel {
    id: featureModel
    project: qgisProject
    positionInformation: positioning.positionInformation
    positionLocked: false
  }

  function test_00_featureModelPositioning() {
    // wait a few seconds so positioning can catch some NMEA strings
    wait(2500);
    verify(featureModel.positionInformation !== undefined);
    verify(featureModel.positionInformation.sourceName === "nmea");
    verify(featureModel.positionInformation.latitude !== undefined);
    verify(featureModel.positionInformation.longitude !== undefined);
    verify(featureModel.positionInformation.elevation !== undefined);
    verify(featureModel.positionInformation.speed !== undefined);
    verify(featureModel.positionInformation.direction !== undefined);
    verify(featureModel.positionInformation.satellitesInView !== undefined);
    verify(featureModel.positionInformation.pdop !== undefined);
    verify(featureModel.positionInformation.hdop !== undefined);
    verify(featureModel.positionInformation.vdop !== undefined);
    verify(featureModel.positionInformation.hacc !== undefined);
    verify(featureModel.positionInformation.vacc !== undefined);
    verify(featureModel.positionInformation.hvacc !== undefined);
    verify(featureModel.positionInformation.utcDateTime !== undefined);
    verify(featureModel.positionInformation.fixMode !== undefined);
    verify(featureModel.positionInformation.fixType !== undefined);
    verify(featureModel.positionInformation.quality !== undefined);
    verify(featureModel.positionInformation.satellitesUsed !== undefined);
    verify(featureModel.positionInformation.status !== undefined);
    verify(featureModel.positionInformation.satPrn !== undefined);
    verify(featureModel.positionInformation.satInfoComplete !== undefined);
    verify(featureModel.positionInformation.isValid !== undefined);
    verify(featureModel.positionInformation.fixStatus !== undefined);
    verify(featureModel.positionInformation.qualityDescription !== undefined);
    verify(featureModel.positionInformation.fixStatusDescription !== undefined);
    verify(featureModel.positionInformation.averagedCount !== undefined);
    verify(featureModel.positionInformation.imuCorrection !== undefined);
    verify(featureModel.positionInformation.orientationValid !== undefined);

  // These are nan:
  // verify(featureModel.positionInformation.verticalSpeed !== undefined)
  // verify(featureModel.positionInformation.magneticVariation !== undefined)
  // verify(featureModel.positionInformation.orientation !== undefined)
  }

  function test_01_ellipsoidalElevation() {
    positioning.elevationCorrectionMode = Positioning.ElevationCorrectionMode.None;
    coordinateTransformer.deltaZ = 0;
    coordinateTransformer.verticalGrid = '';
    // wait a few seconds so positioning can catch some NMEA strings
    wait(2500);

    // the elevation in the NMEA stream goes between 320 to 322, and the ellispodal adjustment is -26.0 meters
    // we therefore simply check whether we are int the 200s value range, which indicates ellispodal elevation is
    // being returned
    compare(Math.floor(positioning.positionInformation.elevation / 100), 2);
  }

  function test_02_orthometricElevation() {
    positioning.elevationCorrectionMode = Positioning.ElevationCorrectionMode.OrthometricFromDevice;
    coordinateTransformer.deltaZ = 0;
    coordinateTransformer.verticalGrid = '';
    // wait a few seconds so positioning can catch some NMEA strings
    wait(2500);

    // the orthmoetric elevation in the NMEA stream goes between 320 to 322,
    // we therefore simply check whether we are int the 300s value range, which indicates orthographic elevation is
    // being returned
    compare(Math.floor(positioning.positionInformation.elevation / 100), 3);
  }

  function test_03_deltaZ() {
    positioning.elevationCorrectionMode = Positioning.ElevationCorrectionMode.None;
    coordinateTransformer.deltaZ = -100;
    coordinateTransformer.verticalGrid = '';
    // wait a few seconds so positioning can catch some NMEA strings
    wait(2500);

    // the elevation in the NMEA stream's range is in the 290s
    // we therefore simply check whether we are in the 100s value range, which indicates
    // the delta Z value has been applied
    compare(Math.floor(positioning.projectedPosition.z / 100), 1);
  }

  function test_04_verticalGrid() {
    positioning.elevationCorrectionMode = Positioning.ElevationCorrectionMode.OrthometricFromGeoidFile;
    coordinateTransformer.deltaZ = 0;
    coordinateTransformer.verticalGrid = dataDir + '/testgrid.tif';
    // wait a few seconds so positioning can catch some NMEA strings
    wait(2500);

    // the elevation in the NMEA stream's range is in the 290s
    // we therefore simply check whether we have a negative value to see
    // if the grid value (~300) has been applied
    compare(Math.floor(positioning.projectedPosition.z / 100), -1);
  }

  function test_05_tcpReceiver() {
    positioning.deviceId = 'tcp:localhost:11111';
    positioning.elevationCorrectionMode = Positioning.ElevationCorrectionMode.None;
    coordinateTransformer.deltaZ = 0;
    coordinateTransformer.verticalGrid = '';
    // wait a few seconds so positioning can catch some NMEA strings
    wait(2500);
    compare(Math.floor(positioning.positionInformation.latitude), 46);
    compare(Math.floor(positioning.positionInformation.longitude), 9);
    compare(Math.floor(positioning.positionInformation.elevation / 10), 110);
    compare(Math.floor(positioning.positionInformation.hacc), 2);
    compare(Math.floor(positioning.positionInformation.vacc), 4);
  }

  function test_06_happyIMU() {
    positioning.deviceId = 'udp:localhost:1959';
    positioning.elevationCorrectionMode = Positioning.ElevationCorrectionMode.None;
    coordinateTransformer.deltaZ = 0;
    coordinateTransformer.verticalGrid = '';

    // wait a few seconds so positioning can catch some NMEA strings
    wait(2500);
    compare(positioning.positionInformation.qualityDescription, "Float RTK + IMU");
    compare(positioning.positionInformation.imuCorrection, true);
  }

  function test_07_happyMonch2IMU() {
    positioning.deviceId = 'udp:localhost:1960';
    positioning.elevationCorrectionMode = Positioning.ElevationCorrectionMode.None;
    coordinateTransformer.deltaZ = 0;
    coordinateTransformer.verticalGrid = '';

    // wait a few seconds so positioning can catch some NMEA strings
    wait(2500);
    compare(positioning.positionInformation.qualityDescription, "Fixed RTK + IMU");
    compare(positioning.positionInformation.imuCorrection, true);
  }
}
