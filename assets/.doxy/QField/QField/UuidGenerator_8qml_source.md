

# File UuidGenerator.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**UuidGenerator.qml**](UuidGenerator_8qml.md)

[Go to the documentation of this file](UuidGenerator_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield

EditorWidgetBase {
  height: childrenRect.height

  anchors {
    right: parent.right
    left: parent.left
  }

  Label {
    id: uuidLabel
    height: fontMetrics.height + 20
    anchors {
      left: parent.left
      right: parent.right
    }

    topPadding: 10
    bottomPadding: 10
    font: Theme.defaultFont
    color: Theme.mainTextDisabledColor
    text: {
      var displayValue = value !== undefined ? value : '';
      if (isLoaded && isAdding && (value == undefined || value === '')) {
        displayValue = StringUtils.createUuid();
        valueChangeRequested(displayValue, false);
      }
      return displayValue;
    }
    elide: Text.ElideMiddle
  }

  FontMetrics {
    id: fontMetrics
    font: uuidLabel.font
  }
}
```


