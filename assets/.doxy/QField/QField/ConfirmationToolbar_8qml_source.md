

# File ConfirmationToolbar.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**ConfirmationToolbar.qml**](ConfirmationToolbar_8qml.md)

[Go to the documentation of this file](ConfirmationToolbar_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield
import Theme

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
```


