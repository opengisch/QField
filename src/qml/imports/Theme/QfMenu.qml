import QtQuick
import QtQuick.Controls

Menu {
  id: control

  property int paddingMultiplier: 1

  width: {
    let result = 50;
    let padding = 0;
    for (let i = 0; i < control.count; ++i) {
      const item = control.itemAt(i);
      if (item) {
        if (item.contentItem) {
          result = Math.max(item.contentItem.implicitWidth, result);
        }
        padding = Math.max(item.leftPadding + item.rightPadding, padding);
      }
    }
    return mainWindow.width > 0 ? Math.min(result + padding * paddingMultiplier, mainWindow.width - 20) : result + padding;
  }
}
