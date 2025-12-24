import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme
import org.qgis
import org.qfield

CogoParameterWidgetBase {
  id: angleParameter

  width: parent.width
  height: childrenRect.height

  GridLayout {
    anchors {
      left: parent.left
      right: parent.right
    }
    columns: 1

    Label {
      Layout.fillWidth: true
      color: Theme.secondaryTextColor
      font: Theme.tipFont
      text: label ? label : qsTr("Angle")
    }

    QfTextField {
      id: angleField
      Layout.fillWidth: true
      font: Theme.tipFont

      onTextEdited: {
        processValue();
      }
    }
  }

  function processValue() {
    const angle = parseFloat(angleField.text);
    if (!isNaN(angle)) {
      valueChangeRequested(angle);
    } else {
      valueChangeRequested(undefined);
    }
  }
}
