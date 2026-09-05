

# File QfDialog.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfDialog.qml**](QfDialog_8qml.md)

[Go to the documentation of this file](QfDialog_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield.core
import org.qfield.gui

Dialog {
  visible: false
  modal: true
  font: QfTheme.defaultFont
  standardButtons: Dialog.Ok | Dialog.Cancel

  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2

  onAboutToShow: {
    const okBtn = standardButton(Dialog.Ok);
    if (okBtn)
      okBtn.text = qsTr("OK");
    const cancelBtn = standardButton(Dialog.Cancel);
    if (cancelBtn)
      cancelBtn.text = qsTr("Cancel");
    const yesBtn = standardButton(Dialog.Yes);
    if (yesBtn)
      yesBtn.text = qsTr("Yes");
    const noBtn = standardButton(Dialog.No);
    if (noBtn)
      noBtn.text = qsTr("No");
    const closeBtn = standardButton(Dialog.Close);
    if (closeBtn)
      closeBtn.text = qsTr("Close");
  }

  onClosed: {
    focusstack.forceActiveFocusOnLastTaker();
  }
}
```


