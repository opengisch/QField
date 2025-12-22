import QtQuick
import QtQuick.Controls

Item {
  /**
   * This signal is emmited when a processing parameter widget has changed the value.
   */
  signal valueChangeRequested(var value)

  /**
   * This signal is emitted when a parameter requests a position from the internal/external GNSS.
   */
  signal requestPosition(var item)
}
