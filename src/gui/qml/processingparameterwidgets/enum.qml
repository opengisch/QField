import QtQuick
import QtQuick.Controls
import Theme
import org.qfield
import org.qgis

ProcessingParameterWidgetBase {
  id: distanceItem

  height: childrenRect.height

  Row {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    spacing: 5

    QfComboBox {
      id: unitTypesComboBox

      width: parent.width
      model: configuration["options"]

      onCurrentIndexChanged: {
        if (currentIndex != value) {
          valueChangeRequested(currentIndex);
        }
      }

      Component.onCompleted: {
        currentIndex = value;
      }
    }
  }
}
