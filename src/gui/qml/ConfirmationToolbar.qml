import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
QfVisibilityFadingRow {
  id: confirmationToolbar

  spacing: 4

  /* This signal is emitted when the digitized geometry has been confirmed.
     * The correspoding handler is \c onConfirmed.
     */
  signal confirm
  /* This signal is emitted when the user cancels geometry digitizing.
     * The correspoding handler is \c onCancel.
     */
  signal cancel

  QfToolButton {
    id: cancelButton
    iconSource: Theme.getThemeVectorIcon("ic_clear_white_24dp")
    visible: true
    round: true
    bgcolor: Theme.darkRed

    onClicked: cancel()
  }

  QfToolButton {
    id: confirmButton
    iconSource: Theme.getThemeVectorIcon("ic_check_white_24dp")
    visible: true
    round: true
    bgcolor: Theme.mainColor

    onClicked: confirm()
  }
}
