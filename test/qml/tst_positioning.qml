import QtQuick 2.3
import QtTest 1.0

import org.qfield 1.0

import QFieldControls 1.0

TestCase {
    name: "Positioning"

    Positioning {
        id: positioning
        deviceId: 'tcp:localhost:1958'
        active: true
        ellipsoidalElevation: true
    }

    function test_positioning() {
        // wait a few seconds so positioning can catch some NMEA strings
        wait(5000)

        // the elevation in the NMEA stream goes between 320 to 322, and the ellispodal adjustment is -26.0 meters
        // we therefore simply check whether we are int the 200s value range, which indicates ellispodal elevation is
        // being returned
        compare(Math.floor(positioning.positionInformation.elevation / 100), 2)
    }
}
