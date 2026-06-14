

# File source.qml

[**File List**](files.md) **>** [**processingparameterwidgets**](dir_845aa3fe93488bbf89a398aeca40ac68.md) **>** [**source.qml**](source_8qml.md)

[Go to the documentation of this file](source_8qml.md)


```C++
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
```


