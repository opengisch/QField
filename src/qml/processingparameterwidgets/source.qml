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
      id: sourceTypesComboBox

      width: parent.width
      model: configuration["options"]
      textRole: "key"
      valueRole: "value"

      onCurrentValueChanged: {
        if (currentValue != value) {
          valueChangeRequested(currentValue);
        }
      }
    }
  }
}
