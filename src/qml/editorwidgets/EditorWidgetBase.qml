import QtQuick 2.12

Item {
    signal valueChanged( var value, bool isNull )

    /* The requested geometry will be returned through calling a requestedGeometry(geometry)
     * function attached to an editor widget which signaled the request.
     */
    signal requestGeometry(var item, var layer)
}
