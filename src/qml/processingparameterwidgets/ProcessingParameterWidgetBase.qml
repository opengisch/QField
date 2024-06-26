import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
  /* This signal is emmited when a processing parameter widget has changed the value.
   */
  signal valueChangeRequested(var value)
}
