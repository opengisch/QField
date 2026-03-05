

# File RelationCombobox.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**RelationCombobox.qml**](RelationCombobox_8qml.md)

[Go to the documentation of this file](RelationCombobox_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import org.qgis
import Theme

Item {
  id: relationCombobox

  property FeatureCheckListModel featureListModel

  property bool useCompleter: false
  property bool useSearch: false
  property bool allowAddFeature: false
  property var relation: undefined
  property var layerResolver: undefined
  property var currentKeyValue: value
  readonly property alias count: comboBox.count
  property EmbeddedFeatureForm embeddedFeatureForm: embeddedPopupLoader.item
  readonly property alias searchPopup: searchFeaturePopup
  property color displayedTextColor: FeatureUtils.attributeIsNull(value) || value === "" || (!isEditable && isEditing) ? Theme.mainTextDisabledColor : Theme.mainTextColor

  signal requestJumpToPoint(var center, real scale, bool handleMargins)

  Component.onCompleted: {
    if (featureListModel && !featureListModel.allowMulti) {
      comboBox.currentIndex = featureListModel.findKey(value);
      invalidWarning.visible = relation !== undefined ? !(relation.isValid) : false;
    }
  }

  onCurrentKeyValueChanged: {
    if (featureListModel && !featureListModel.allowMulti) {
      comboBox._cachedCurrentValue = currentKeyValue;
      comboBox.currentIndex = featureListModel.findKey(currentKeyValue);
    }
  }

  height: childrenRect.height

  QfPopup {
    id: searchFeaturePopup

    readonly property int minimumHeight: mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)

    parent: mainWindow.contentItem
    width: mainWindow.width - Theme.popupScreenEdgeHorizontalMargin * 2
    height: minimumHeight > 0 ? minimumHeight : 200
    x: Theme.popupScreenEdgeHorizontalMargin
    y: (mainWindow.height - height) / 2
    z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes
    closePolicy: Popup.CloseOnEscape
    focus: visible

    onOpened: {
      if (searchableText.typedFilter != '') {
        searchBar.setSearchTerm(searchableText.typedFilter);
      }
      if (searchableText.focus) {
        searchableText.text = '';
        searchableText.focus = false;
      }
      if (resultsList.contentHeight > resultsList.height) {
        searchBar.focusOnTextField();
      }
    }

    onClosed: {
      searchBar.clear();
      // Reset the current index based on non-filtered list content
      comboBox.currentIndex = featureListModel.findKey(currentKeyValue);
    }

    Page {
      anchors.fill: parent
      padding: 5

      header: QfPageHeader {
        title: fieldLabel
        showBackButton: false
        showApplyButton: false
        showCancelButton: true
        onCancel: searchFeaturePopup.close()
      }

      QfSearchBar {
        id: searchBar
        z: 1
        anchors.left: parent.left
        anchors.right: parent.right
        height: childrenRect.height

        onSearchTermChanged: {
          featureListModel.searchTerm = searchTerm;
        }

        onReturnPressed: {
          if (featureListModel.rowCount() === 1) {
            resultsList.itemAtIndex(0).performClick();
            searchFeaturePopup.close();
          }
        }
      }

      ListView {
        id: resultsList
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchBar.bottom
        model: featureListModel
        width: parent.width
        height: searchFeaturePopup.height - searchBar.height - 60
        clip: true
        ScrollBar.vertical: QfScrollBar {}
        section.property: featureListModel ? featureListModel.groupField != "" ? "groupFieldValue" : "" : ""
        section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
        section.delegate: Component {
          Rectangle {
            width: parent.width
            height: featureListModel.displayGroupName ? 30 : 5
            color: Theme.controlBackgroundAlternateColor

            Text {
              anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
              }
              font.bold: true
              font.pointSize: Theme.resultFont.pointSize
              color: Theme.mainTextColor
              text: section
              visible: featureListModel.displayGroupName
            }
          }
        }

        delegate: Rectangle {
          id: rectangle

          property int idx: index
          property string itemText: StringUtils.highlightText(displayString, featureListModel.searchTerm, Theme.mainTextColor)

          anchors.margins: 10
          width: parent ? parent.width : undefined
          height: line.height + 20
          color: model.checked ? Theme.mainColor : searchFeaturePopup.Material ? searchFeaturePopup.Material.dialogColor : Theme.mainBackgroundColor

          Row {
            id: line
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            RadioButton {
              id: radioButton

              visible: !featureListModel.allowMulti
              anchors.verticalCenter: parent.verticalCenter
              width: 0
              height: 48
              padding: 12

              font.pointSize: Theme.defaultFont.pointSize
              font.weight: model.checked ? Font.DemiBold : Font.Normal

              checked: model.checked
              indicator: Rectangle {}
            }

            CheckBox {
              id: checkBoxButton

              visible: !!featureListModel.allowMulti
              anchors.verticalCenter: parent.verticalCenter
              padding: 12

              font.pointSize: Theme.defaultFont.pointSize
              font.weight: model.checked ? Font.DemiBold : Font.Normal
            }

            Text {
              id: contentText
              width: rectangle.width - (checkBoxButton.visible ? checkBoxButton.width : radioButton.width) - 10
              anchors.verticalCenter: parent.verticalCenter
              leftPadding: 5
              font.pointSize: Theme.defaultFont.pointSize
              font.weight: model.checked ? Font.DemiBold : Font.Normal
              elide: Text.ElideRight
              wrapMode: Text.WordWrap
              color: featureListModel.searchTerm != '' ? Theme.secondaryTextColor : Theme.mainTextColor
              textFormat: Text.RichText
              text: itemText
            }
          }

          /* bottom border */
          Rectangle {
            anchors.bottom: parent.bottom
            height: 1
            color: Theme.controlBorderColor
            width: parent.width
          }

          function performClick() {
            model.checked = true;
          }
        }

        MouseArea {
          anchors.fill: parent
          propagateComposedEvents: true

          onClicked: mouse => {
            const item = resultsList.itemAt(resultsList.contentX + mouse.x, resultsList.contentY + mouse.y);
            if (!item)
              return;
            item.performClick();
            if (!resultsList.model.allowMulti) {
              searchFeaturePopup.close();
            }
          }
        }

        onMovementStarted: {
          Qt.inputMethod.hide();
        }
      }
    }
  }

  RowLayout {
    anchors {
      left: parent.left
      right: parent.right
    }

    QfComboBox {
      id: comboBox
      objectName: "RelationComboBox"
      visible: !enabled || (!useSearch && !useCompleter && (relation !== undefined ? relation.isValid : true))
      Layout.fillWidth: true

      property var _cachedCurrentValue

      model: featureListModel
      textRole: 'display'
      valueRole: 'keyFieldValue'

      onCurrentIndexChanged: {
        if (searchFeaturePopup.opened)
          return;
        const newValue = featureListModel.dataFromRowIndex(currentIndex, FeatureListModel.KeyFieldRole);
        if (newValue !== currentKeyValue) {
          valueChangeRequested(newValue, false);
        }
      }

      Connections {
        target: featureListModel
        enabled: featureListModel ? !featureListModel.allowMulti : false

        function onModelReset() {
          comboBox.currentIndex = featureListModel.findKey(comboBox._cachedCurrentValue);
        }
      }

      MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true

        onClicked: mouse => {
          mouse.accepted = false;
        }
        onPressed: mouse => {
          forceActiveFocus();
          mouse.accepted = false;
        }
        onReleased: mouse => {
          mouse.accepted = false;
        }
        onDoubleClicked: mouse => {
          mouse.accepted = false;
        }
        onPositionChanged: mouse => {
          mouse.accepted = false;
        }
        onPressAndHold: mouse => {
          mouse.accepted = false;
        }
      }

      Component.onCompleted: {
        comboBox.popup.z = 10000; // 1000s are embedded feature forms, use a higher value to insure popups always show above embedded feature formes
      }

      font: Theme.defaultFont
      text.color: displayedTextColor
      displayText: {
        if (!isEditing && value === "") {
          return qsTr("Empty");
        } else if (!isEditing && FeatureUtils.attributeIsNull(value)) {
          return qsTr("NULL");
        }
        return comboBox.currentIndex === -1 && value !== undefined ? '(' + value + ')' : comboBox.currentText;
      }

      popup: Popup {
        y: comboBox.height - 1
        width: comboBox.width
        implicitHeight: contentItem.implicitHeight
        padding: 1
        font: Theme.defaultFont
        topMargin: mainWindow.sceneTopMargin
        bottomMargin: mainWindow.sceneTopMargin

        onAboutToShow: {
          contentItem.model = comboBox.delegateModel;
        }

        onAboutToHide: {
          contentItem.model = null;
        }

        contentItem: ListView {
          clip: true
          implicitHeight: Math.min(mainWindow.height - mainWindow.sceneTopMargin - mainWindow.sceneTopMargin, contentHeight)
          currentIndex: comboBox.highlightedIndex

          section.property: featureListModel ? featureListModel.groupField != "" ? "groupFieldValue" : "" : ""
          section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
          section.delegate: Component {
            Rectangle {
              width: parent.width
              height: featureListModel.displayGroupName ? 30 : 5
              color: Theme.mainBackgroundColor

              Text {
                anchors {
                  horizontalCenter: parent.horizontalCenter
                  verticalCenter: parent.verticalCenter
                }
                font.bold: true
                font.pointSize: Theme.resultFont.pointSize
                color: Theme.mainTextColor
                text: section
                visible: featureListModel.displayGroupName
              }
            }
          }

          ScrollIndicator.vertical: ScrollIndicator {}
        }
      }

      background.visible: relationCombobox.enabled || (!isEditable && isEditing)
      indicator.visible: relationCombobox.enabled || (!isEditable && isEditing)
    }

    FontMetrics {
      id: fontMetrics
      font: comboBox.font
    }

    Rectangle {
      id: searchable
      visible: !comboBox.visible
      height: searchableText.height + searchableText.topInset + searchableText.bottomInset
      Layout.fillWidth: true
      Layout.topMargin: 5
      Layout.bottomMargin: 5

      Text {
        id: searchableLabel

        property bool useCompleter: false
        property string completer: ''

        anchors.verticalCenter: parent.verticalCenter
        topPadding: searchableText.topPadding
        leftPadding: searchableText.leftPadding
        rightPadding: searchableText.rightPadding
        bottomPadding: searchableText.bottomPadding
        width: parent.width - dropDownArrowCanvas.width - dropDownArrowCanvas.anchors.rightMargin * 2
        height: parent.height
        text: useCompleter ? completer : comboBox.displayText
        font: comboBox.font
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        textFormat: Text.RichText
        clip: true
        elide: Text.ElideRight

        color: value === undefined || !enabled ? Theme.mainTextDisabledColor : searchableText.text === '' ? Theme.mainTextColor : Theme.mainTextDisabledColor
      }

      TextField {
        id: searchableText

        property string typedFilter: ''

        anchors.verticalCenter: parent.verticalCenter
        width: parent.width - dropDownArrowCanvas.width - dropDownArrowCanvas.anchors.rightMargin * 2
        text: ''
        font: comboBox.font
        horizontalAlignment: TextInput.AlignLeft
        verticalAlignment: TextInput.AlignVCenter

        color: Theme.mainTextColor
        background: Rectangle {
          color: "transparent"
          border.color: "transparent"
          border.width: 0
          implicitHeight: searchableText.Material.textFieldHeight
        }

        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhSensitiveData

        onDisplayTextChanged: {
          if (activeFocus) {
            if (text != comboBox.displayText) {
              const trimmedText = text.trim();
              const matches = featureListModel.findDisplayValueMatches(trimmedText);
              if (matches.length > 0) {
                const remainder = featureListModel.dataFromRowIndex(matches[0], featureListModel.DisplayStringRole).substring(trimmedText.length);
                searchableLabel.completer = '<span style="color:rgba(0,0,0,0);">' + trimmedText + '</span><span style="font-weight:' + (matches.length === 1 ? 'bold' : 'normal') + ';">' + remainder + '</span>';
                color = Theme.mainTextColor;
              } else {
                searchableLabel.completer = '';
                color = Theme.warningColor;
              }
            } else {
              searchableLabel.completer = '';
            }
          }
        }

        onActiveFocusChanged: {
          searchableLabel.useCompleter = activeFocus;
          if (activeFocus) {
            if (text === '') {
              if (!featureListModel.addNull || comboBox.currentIndex !== 0) {
                text = comboBox.displayText;
                color = Theme.mainTextColor;
              }
              searchableLabel.completer = '';
            }
          } else {
            if (!isLastKeyPressedReturn) {
              if (text === '' && featureListModel.addNull) {
                comboBox.currentIndex = 0;
                searchableLabel.completer = comboBox.displayText;
              } else {
                applyAutoCompletion(true);
              }
            } else if (text !== '') {
              // the last return-triggered auto completion didn't match anything, we have to reset the item state
              text = '';
              searchableLabel.completer = comboBox.displayText;
            }
          }
          isLastKeyPressedReturn = false;
        }

        property bool isLastKeyPressedReturn: false
        Keys.onPressed: event => {
          if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
            if (!isLastKeyPressedReturn) {
              applyAutoCompletion();
            }
            isLastKeyPressedReturn = true;
          } else {
            isLastKeyPressedReturn = false;
          }
        }

        function applyAutoCompletion(resetIfNone = false) {
          const trimmedText = text.trim();
          const matches = featureListModel.findDisplayValueMatches(trimmedText);
          if (matches.length > 0) {
            text = '';
            comboBox.currentIndex = matches[0];
            searchableLabel.completer = comboBox.displayText;
            if (matches.length > 1) {
              // remember the typed filter in case users want to see the multiple hits by clicking on the search button
              typedFilter = trimmedText;
              searchableTimer.restart();
            }
          } else if (resetIfNone) {
            text = '';
            searchableLabel.completer = comboBox.displayText;
          }
        }

        Timer {
          id: searchableTimer
          interval: 500
          onTriggered: {
            searchableText.typedFilter = '';
          }
        }
      }

      Canvas {
        id: dropDownArrowCanvas
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        width: 30
        height: 15
        contextType: "2d"
        visible: !useCompleter

        onPaint: {
          if (!context) {
            return;
          }
          context.reset();
          context.moveTo(10, 5);
          context.lineTo(20, 5);
          context.lineTo(15, 10);
          context.closePath();
          context.fillStyle = !enabled ? Theme.mainTextDisabledColor : Theme.mainTextColor;
          context.fill();
        }

        onEnabledChanged: requestPaint()
      }

      border.color: searchableText.activeFocus ? Theme.mainColor : searchableText.hovered ? searchableText.Material.primaryTextColor : searchableText.Material.hintTextColor
      border.width: searchableText.activeFocus ? 2 : 1
      color: "transparent"
      radius: 2

      MouseArea {
        enabled: !useCompleter
        anchors.fill: parent

        onClicked: mouse => {
          mouse.accepted = true;
          searchFeaturePopup.open();
        }
      }
    }

    QfToolButton {
      id: searchButton
      objectName: "OpenSearchFeaturePopupButton"

      Layout.preferredWidth: enabled ? 48 : 0
      Layout.preferredHeight: 48

      bgcolor: "transparent"
      iconSource: Theme.getThemeVectorIcon("ic_baseline_search_white")
      iconColor: Theme.mainTextColor

      visible: enabled

      onClicked: {
        searchFeaturePopup.open();
      }
    }

    QfToolButton {
      id: addFeatureButton
      objectName: "AddFeatureButton"

      Layout.preferredWidth: comboBox.enabled ? 48 : 0
      Layout.preferredHeight: 48

      bgcolor: "transparent"
      opacity: enabled ? 1 : 0.3
      iconSource: Theme.getThemeVectorIcon("ic_add_white_24dp")
      iconColor: Theme.mainTextColor

      visible: enabled && allowAddFeature && (layerResolver !== undefined || (relation !== undefined && relation.isValid))

      onClicked: {
        if (relationCombobox.relation !== undefined && relationCombobox.relation.referencedLayer.geometryType() !== Qgis.GeometryType.Null) {
          requestGeometry(relationCombobox, relationCombobox.relation.referencedLayer);
          return;
        } else if (relationCombobox.layerResolver !== undefined && relationCombobox.layerResolver.currentLayer.geometryType() !== Qgis.GeometryType.Null) {
          requestGeometry(relationCombobox, relationCombobox.layerResolver.currentLayer);
          return;
        }
        showAddFeaturePopup();
      }
    }

    Text {
      id: invalidWarning
      visible: false
      text: qsTr("Invalid relation")
      color: Theme.errorColor
    }
  }

  Loader {
    id: embeddedPopupLoader
    active: false

    sourceComponent: EmbeddedFeatureForm {
      id: embeddedPopup

      embeddedLevel: form.embeddedLevel + 1
      digitizingToolbar: form.digitizingToolbar
      codeReader: form.codeReader

      onFeatureSaved: {
        // model not yet reloaded - keep the value in _cachedCurrentValue and set it onModelReset
        if (relationCombobox.relation !== undefined) {
          const referencedValue = embeddedPopup.attributeFormModel.attribute(relationCombobox.relation.resolveReferencedField(field.name));
          const index = featureListModel.findKey(referencedValue);
          if ((featureListModel.addNull && index < 1) || index < 0) {
            comboBox._cachedCurrentValue = referencedValue;
          }
        } else {
          const keyValue = embeddedPopup.attributeFormModel.attribute(relationCombobox.featureListModel.keyField);
          const index = featureListModel.findKey(keyValue);
          if ((featureListModel.addNull && index < 1) || index < 0) {
            comboBox._cachedCurrentValue = keyValue;
          }
        }
      }

      onRequestJumpToPoint: function (center, scale, handleMargins) {
        relationCombobox.requestJumpToPoint(center, scale, handleMargins);
      }
    }
  }

  function requestedGeometryReceived(geometry) {
    showAddFeaturePopup(geometry);
  }

  function ensureEmbeddedFormLoaded() {
    if (!embeddedPopupLoader.active) {
      embeddedPopupLoader.active = true;
    }
  }

  function showAddFeaturePopup(geometry) {
    ensureEmbeddedFormLoaded();
    embeddedFeatureForm.state = 'Add';
    embeddedFeatureForm.currentLayer = null;
    if (relationCombobox.relation !== undefined) {
      embeddedFeatureForm.currentLayer = relationCombobox.relation.referencedLayer;
    } else if (relationCombobox.layerResolver !== undefined) {
      embeddedFeatureForm.currentLayer = relationCombobox.layerResolver.currentLayer;
    }
    if (geometry !== undefined) {
      embeddedFeatureForm.applyGeometry(geometry);
    }
    embeddedFeatureForm.open();
  }
}
```


