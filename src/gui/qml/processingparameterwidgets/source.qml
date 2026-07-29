import QtQuick
import QtQuick.Controls
import Theme
import org.qfield
import org.qgis

ProcessingParameterWidgetBase {
  id: sourceItem

  height: childrenRect.height

  Row {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    spacing: 5

    QfComboBox {
      id: sourceComboBox

      width: parent.width

      model: configuration["layers"]
      textRole: "name"
      valueRole: "id"

      onCurrentValueChanged: {
        if (currentValue !== value) {
          valueChangeRequested(currentValue);
        }
      }
    }
  }
}
