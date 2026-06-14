

# File EditorWidgetBase.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**EditorWidgetBase.qml**](EditorWidgetBase_8qml.md)

[Go to the documentation of this file](EditorWidgetBase_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

Item {
  property bool isLoaded: false
  property bool hasMenu: false
  readonly property bool isNull: FeatureUtils.attributeIsNull(value)
  readonly property bool isEmpty: value === ''

  property Menu menu: QfMenu {
    id: itemMenu
    title: qsTr("Item Menu")
    z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin
  }

  signal valueChangeRequested(var value, bool isNull)

  signal requestGeometry(var item, var layer)

  signal requestBarcode(var item)

  signal requestJumpToPoint(var center, real scale, bool handleMargins)
}
```


