import QtQuick
import QtQuick.Controls
import org.qfield

/**
 * \ingroup qml
 */
Dialog {
  visible: false
  modal: true
  font: Theme.defaultFont
  standardButtons: Dialog.Ok | Dialog.Cancel

  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2

  onClosed: {
    focusstack.forceActiveFocusOnLastTaker();
  }
}
