import QtQuick
import QtQuick.Controls

/**
 * \ingroup qml
 */
SwitchDelegate {
  id: control

  property bool small: false

  implicitWidth: small ? 34 : 48
  implicitHeight: 30
}
