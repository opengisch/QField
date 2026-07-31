import QtQuick
import QtQuick.Controls
import Theme
import org.qfield.gui.processingparameterwidgets
import org.qgis

QfProcessingParameterWidgetBase {
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
