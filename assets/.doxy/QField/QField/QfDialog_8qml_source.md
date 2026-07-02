

# File QfDialog.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfDialog.qml**](QfDialog_8qml.md)

[Go to the documentation of this file](QfDialog_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield

Dialog {
  visible: false
  modal: true
  font: Theme.defaultFont
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


