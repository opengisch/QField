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

  onAboutToShow: {
    var okBtn = standardButton(Dialog.Ok);
    if (okBtn)
      okBtn.text = qsTr("OK");
    var cancelBtn = standardButton(Dialog.Cancel);
    if (cancelBtn)
      cancelBtn.text = qsTr("Cancel");
    var yesBtn = standardButton(Dialog.Yes);
    if (yesBtn)
      yesBtn.text = qsTr("Yes");
    var noBtn = standardButton(Dialog.No);
    if (noBtn)
      noBtn.text = qsTr("No");
    var closeBtn = standardButton(Dialog.Close);
    if (closeBtn)
      closeBtn.text = qsTr("Close");
  }

  onClosed: {
    focusstack.forceActiveFocusOnLastTaker();
  }
}
