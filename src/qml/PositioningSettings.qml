import QtQuick 2.12
import Qt.labs.settings 1.0 as LabSettings

LabSettings.Settings {
    property bool positioningActivated: false

    property string positioningDevice: ""
    property string positioningDeviceName: qsTr( "Internal device" );
    property bool ellipsoidalElevation: true

    property bool showPositionInformation: false

    property bool accuracyIndicator: false
    property real accuracyBad: 5.0
    property real accuracyExcellent: 1.0
    property bool accuracyRequirement: false

    property real antennaHeight: 0.0
    property bool antennaHeightActivated: false
    property bool skipAltitudeCorrection: false

    property string verticalGrid: ""
}
