

# File QfProcessingParameterBoolean.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**processingparameterwidgets**](dir_0b91122ea4e9f423064291e4771a420d.md) **>** [**QfProcessingParameterBoolean.qml**](QfProcessingParameterBoolean_8qml.md)

[Go to the documentation of this file](QfProcessingParameterBoolean_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield.gui
import org.qfield.core
import org.qgis

QfProcessingParameterWidgetBase {
  id: booleanItem

  height: childrenRect.height

  Row {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    spacing: 5

    Label {
      id: labelValue

      anchors.verticalCenter: parent.verticalCenter
      width: parent.width - checkBox.width
      font.pointSize: QfTheme.defaultFont.pointSize
      font.bold: QfTheme.defaultFont.bold

      text: value ? qsTr("True") : qsTr("False")
    }

    QfSwitch {
      id: checkBox

      anchors.verticalCenter: parent.verticalCenter
      width: implicitContentWidth

      checked: value
    }
  }

  MouseArea {
    id: checkArea
    anchors.fill: parent

    onClicked: {
      valueChangeRequested(!value);
    }
  }
}
```


