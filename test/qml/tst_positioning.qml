import QtQuick
import QtTest
import org.qfield

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

  Repeater {
    id: positioningInformations
    model: PositioningInformationModel {
      distanceUnits: qgisProject.distanceUnits
      positioningSource: positioning
    }
    delegate: Item {
      property string name: Name
      property string value: Value
    }
  }

  FeatureModel {
    id: featureModel
    project: qgisProject
    currentLayer: qgisProject.mapLayersByName('Apiary')[0]
    appExpressionContextScopesGenerator: AppExpressionContextScopesGenerator {
      positionInformation: positioning.positionInformation
      positionLocked: false
    }
  }

  /**
   * Test the positioning of the feature model.
   *
   * This function performs the following steps:
   * 1. Initially verifies that the positional attributes (x, y, z) are undefined,
   *    indicating no data has been received yet.
   * 2. Waits for 2500 milliseconds to allow the system to collect and process NMEA strings,
   *    which carry essential navigation data.
   * 3. Resets the feature model's attributes to ensure feature updated.
   * 4. Validates that the attribute values for source, quality, fix status, and satellite count
   *    are set correctly, confirming that the positioning data has been successfully acquired.
   * 5. Verifies that the position coordinates (x, y, z) and are now defined,
   *    signaling that the feature model has processed the positioning data correctly.
   */
  function test_00_featureModelPositioning() {
    // wait a few seconds so positioning can catch some NMEA strings
    verify(featureModel.feature.attribute("x") === undefined);
    verify(featureModel.feature.attribute("y") === undefined);
    verify(featureModel.feature.attribute("z") === undefined);
    wait(2500);
    featureModel.resetAttributes();
    compare(featureModel.feature.attribute("source"), "manual");
    compare(featureModel.feature.attribute("Quality"), "Autonomous");
    compare(featureModel.feature.attribute("Fix status"), "Fix3D");
    verify(featureModel.feature.attribute("Horizontal accuracy") !== undefined);
    verify(featureModel.feature.attribute("Nb. of satellites") !== undefined);
    verify(featureModel.feature.attribute("x") !== undefined);
    verify(featureModel.feature.attribute("y") !== undefined);
    verify(featureModel.feature.attribute("z") !== undefined);
    verify(featureModel.appExpressionContextScopesGenerator.positionInformation.latitude !== undefined);
    featureModel.appExpressionContextScopesGenerator.positionLocked = true;
    featureModel.resetAttributes();
    compare(featureModel.feature.attribute("source"), "nmea");
    compare(featureModel.feature.attribute("Quality"), "Autonomous");
    compare(featureModel.feature.attribute("Fix status"), "Fix3D");
    verify(featureModel.feature.attribute("Horizontal accuracy") !== undefined);
    verify(featureModel.feature.attribute("Nb. of satellites") !== undefined);
    verify(featureModel.feature.attribute("x") !== undefined);
    verify(featureModel.feature.attribute("y") !== undefined);
    verify(featureModel.feature.attribute("z") !== undefined);
    verify(featureModel.appExpressionContextScopesGenerator.positionInformation.latitude !== undefined);
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
    compare(positioning.positionInformation.imuRollValid, true);
    compare(positioning.positionInformation.imuPitchValid, true);
    compare(positioning.positionInformation.imuHeadingValid, true);
    compare(positioning.positionInformation.imuSteeringValid, true);
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
    compare(positioning.positionInformation.imuRollValid, true);
    compare(positioning.positionInformation.imuPitchValid, true);
    compare(positioning.positionInformation.imuHeadingValid, true);
    compare(positioning.positionInformation.imuSteeringValid, true);

    // Expected values for positioningInformations
    const expectedPositioningInformationsValues = [
      {
        "name": "PDOP",
        "value": "1.5"
      },
      {
        "name": "HDOP",
        "value": "0.9"
      },
      {
        "name": "VDOP",
        "value": "1.2"
      },
      {
        "name": "Valid",
        "value": "True"
      },
      {
        "name": "Fix",
        "value": "Fix3D"
      },
      {
        "name": "Quality",
        "value": "Fixed RTK + IMU"
      }
    ];

    // we are skipping "4" items ["X","Y","Altitude","Speed", "H. Accuracy", "V. Accuracy"] because they might move a bit.
    compare(positioningInformations.count, expectedPositioningInformationsValues.length + 6);
    expectedPositioningInformationsValues.forEach((expected, index) => {
      compare(positioningInformations.itemAt(index + 6).name, expected.name);
      compare(positioningInformations.itemAt(index + 6).value, expected.value);
    });
  }
}
