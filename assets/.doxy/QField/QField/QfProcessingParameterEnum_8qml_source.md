

# File QfProcessingParameterEnum.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**processingparameterwidgets**](dir_0b91122ea4e9f423064291e4771a420d.md) **>** [**QfProcessingParameterEnum.qml**](QfProcessingParameterEnum_8qml.md)

[Go to the documentation of this file](QfProcessingParameterEnum_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield.gui
import org.qgis

QfProcessingParameterWidgetBase {
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
```


