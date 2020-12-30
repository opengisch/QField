import QtQuick 2.12

Item {
    signal valueChanged( var value, bool isNull )
    signal requestGeometry(var item, var layer)
}
