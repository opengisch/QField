

# File QfEditorWidgetUniqueValues.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_aa4aab3cdee284f0e217d9df55b13787.md) **>** [**QfEditorWidgetUniqueValues.qml**](QfEditorWidgetUniqueValues_8qml.md)

[Go to the documentation of this file](QfEditorWidgetUniqueValues_8qml.md)


```C++
import QtQuick
import org.qfield.core
import org.qfield.gui
import org.qgis

QfEditorWidgetBase {
  id: uniqueValuesWidget

  readonly property VectorLayer editedLayer: currentLayer

  height: uniqueValuesComboBox.height + 4

  QfFeatureCheckListModel {
    id: listModel

    currentLayer: editedLayer
    keyField: field.name
    displayValueField: field.name
    uniqueValues: true
    attributeField: field

    // attributeValue has to be the last one set to make sure the property’s value is handled properly
    attributeValue: value !== undefined ? value : ''

    onListUpdated: {
      valueChangeRequested(attributeValue, false);
    }
  }

  QfRelationCombobox {
    id: uniqueValuesComboBox
    objectName: "uniqueValuesComboBox"

    anchors {
      left: parent.left
      right: parent.right
    }

    featureListModel: listModel
    allowNewValues: Number(config['Editable']) === 1
    useCompleter: allowNewValues
    enabled: isEnabled
    relation: undefined
  }
}
```


