import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
    /* This property indicates whether the editor widget has been fully loaded by its Loader.
     * Note: prior to this property being true, signals emitted by the editor widget will not be
     * propagated.
     */
    property bool isLoaded: false
    property bool hasMenu: false
    property Menu menu: Menu {
      id: itemMenu
      title: qsTr( "Item Menu" )
      z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes

      width: {
          var result = 0;
          var padding = 0;
          for (var i = 0; i < count; ++i) {
              var item = itemAt(i);
              result = Math.max(item.contentItem.implicitWidth, result);
              padding = Math.max(item.padding, padding);
          }
          return result + padding * 2;
      }
    }

    /* This signal is emmited when an editor widget has changed the value.
     */
    signal valueChangeRequested(var value, bool isNull)

    /* This signal is emitted when an editor widget is in need of a digitized geometry. The
     * geometry will be returned through calling a requestedGeometryReceived(geometry) function
     * attached to editor widget which signaled the request. The corresponding
     * handler is \c onRequestGeometry.
     */
    signal requestGeometry(var item, var layer)

    /* This signal is emitted when an editor widget is requesting a barcode value. The
     * decoded barcode value will be returned as a string through calling a requestedBarcodeReceived(string) function
     * attached to editor widget which signaled the request. The corresponding
     * handler is \c onRequestBarcode.
     */
    signal requestBarcode(var item)
}
