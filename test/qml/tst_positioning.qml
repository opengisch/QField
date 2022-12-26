import QtQuick 2.3
import QtTest 1.0

import org.qfield 1.0

import QFieldControls 1.0

TestCase {
    name: "Positioning"

    Positioning {
        id: positioning
        active: true
        deviceId: 'udp:localhost:1958'
        ellipsoidalElevation: true
    }

    function test_positioning() {
        compare('one', 'one')
    }
}
