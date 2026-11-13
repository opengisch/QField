import QtQuick
import QtQuick.Controls
import org.qfield.core
import org.qfield.gui
import org.qgis

QfEditorWidgetBase {
  id: uniqueValuesWidget

  readonly property bool allowNewValues: Number(config['Editable']) === 1
  readonly property VectorLayer editedLayer: currentLayer
  readonly property string valueText: isNull ? '' : String(value)

  height: (allowNewValues ? editableComboBox.height : uniqueValuesComboBox.height) + 4

  onValueTextChanged: {
    editableComboBox.editText = valueText;
  }

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

    featureListModel: allowNewValues ? null : listModel
    enabled: isEnabled
    visible: !allowNewValues
    relation: undefined
  }

  QfComboBox {
    id: editableComboBox
    objectName: "uniqueValuesEditableComboBox"

    property bool isTextEdited: false

    anchors {
      left: parent.left
      right: parent.right
    }

    enabled: isEnabled
    visible: allowNewValues
    editable: true
    currentIndex: -1
    model: allowNewValues ? listModel : null
    textRole: "display"
    font: QfTheme.defaultFont

    Component.onCompleted: {
      editText = valueText;
      popup.z = 10000; // 1000s are embedded feature forms, use a higher value to insure popups always show above embedded feature formes
    }

    Connections {
      target: editableComboBox.contentItem

      function onTextEdited() {
        editableComboBox.isTextEdited = true;
      }
    }

    onEditTextChanged: {
      const isUserEdit = isTextEdited;
      isTextEdited = false;
      if (editText === valueText) {
        return;
      }
      if (isUserEdit) {
        valueChangeRequested(editText, editText === '');
      } else {
        editText = valueText;
      }
    }

    onActivated: index => {
      valueChangeRequested(textAt(index), false);
    }
  }
}
