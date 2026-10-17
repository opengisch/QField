

# File QfConfirmationToolbar.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfConfirmationToolbar.qml**](QfConfirmationToolbar_8qml.md)

[Go to the documentation of this file](QfConfirmationToolbar_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield.core
import org.qfield.gui

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
    iconSource: QfTheme.getThemeVectorIcon("ic_clear_white_24dp")
    visible: true
    round: true
    bgcolor: QfTheme.darkRed

    onClicked: cancel()
  }

  QfToolButton {
    id: confirmButton
    iconSource: QfTheme.getThemeVectorIcon("ic_check_white_24dp")
    visible: true
    round: true
    bgcolor: QfTheme.mainColor

    onClicked: confirm()
  }
}
```


