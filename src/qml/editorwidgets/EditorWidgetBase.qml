import QtQuick 2.12

Item {
    property bool isLoaded: false

    /* This signal is emmited when an editor widget has changed the value.
     */
    signal valueChangeRequested(var value, bool isNull)

    /* This signal is emitted when an editor widget is in need of a digitized geometry. The
     * geometry will be returned through calling a requestedGeometry(geometry) function
     * attached to editor widget which signaled the request. The corresponding
     * handler is \c onRequestGeometry.
     */
    signal requestGeometry(var item, var layer)
}
