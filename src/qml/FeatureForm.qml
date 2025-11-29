import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQml.Models
import QtQml
import QtCharts
import QtWebView
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Page {
  id: form

  signal confirmed
  signal cancelled
  signal temporaryStored
  signal valueChanged(var field, var oldValue, var newValue)
  signal aboutToSave

  signal toolbarDragged(var deltaX, var deltaY)
  signal toolbarDragAcquired
  signal toolbarDragReleased

  signal requestGeometry(var item, var layer)
  signal requestBarcode(var item)
  signal requestJumpToPoint(var center, real scale, bool handleMargins)

  property DigitizingToolbar digitizingToolbar
  property CodeReader codeReader

  property AttributeFormModel model
  property alias currentTab: swipeView.currentIndex
  property alias toolbarVisible: toolbar.visible
  //! if embedded form called by RelationEditor or RelationReferenceWidget
  property bool embedded: false
  property int embeddedLevel: 0
  //setupOnly means data would be neither saved nor cleared (feature creation is handled elsewhere like e.g. in the tracking)
  property bool setupOnly: false
  property bool featureCreated: false
  property bool isVertical: false
  property bool isDraggable: false

  property double topMargin: 0.0
  property double leftMargin: 0.0
  property double rightMargin: 0.0
  property double bottomMargin: 0.0

  function requestCancel() {
    if (!qfieldSettings.autoSave) {
      cancelDialog.open();
    } else {
      cancel();
    }
  }

  function resetTabs() {
    tabRow.currentIndex = 0;
  }

  clip: true
  states: [
    State {
      name: 'ReadOnly'
    },
    State {
      name: 'Edit'
    },
    State {
      name: 'Add'
    }
  ]

  /**
   * This is a relay to forward private signals to internal components.
   */
  QtObject {
    id: master

    /**
     * When set to true, changed value signals are ignored to avoid double feature creation / save when in fast editing mode
     */
    property bool ignoreChanges: false
  }

  ColumnLayout {
    id: container
    anchors.fill: parent
    anchors.leftMargin: form.leftMargin
    anchors.rightMargin: form.rightMargin

    QfTabBar {
      id: tabRow
      objectName: "tabRow"
      visible: form.model.hasTabs
      model: form.model.hasTabs ? form.model : 0
      Layout.fillWidth: true
      Layout.preferredHeight: defaultHeight

      delegate: TabButton {
        id: tabButton

        property bool isCurrentIndex: index == tabRow.currentIndex

        objectName: "tabRowdDelegate_" + index
        text: Name
        topPadding: 0
        bottomPadding: 0
        leftPadding: !ConstraintHardValid || !ConstraintSoftValid ? 22 : 8
        rightPadding: 8
        width: contentItem.width + leftPadding + rightPadding
        height: 48

        onClicked: {
          tabRow.currentIndex = index;
        }

        background: Rectangle {
          implicitWidth: parent.width
          implicitHeight: parent.height
          color: "transparent"

          Rectangle {
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            width: 10
            height: 10
            radius: 5
            color: !ConstraintHardValid ? Theme.errorColor : Theme.warningColor
            visible: !ConstraintHardValid || !ConstraintSoftValid
          }
        }

        contentItem: Text {
          // Make sure the width is derived from the text so we can get wider
          // than the parent item and the Flickable is useful
          width: paintedWidth
          height: parent.height
          text: tabButton.text
          color: !tabButton.enabled ? Theme.mainTextDisabledColor : tabButton.down ? Qt.darker(Theme.mainColor, 1.5) : isCurrentIndex ? Theme.mainColor : Theme.mainTextColor
          font.pointSize: Theme.tipFont.pointSize
          font.weight: isCurrentIndex ? Font.DemiBold : Font.Normal

          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
        }
      }
    }

    /**
     * The main form content area
     */
    SwipeView {
      id: swipeView
      Layout.fillWidth: true
      Layout.fillHeight: true
      currentIndex: tabRow.currentIndex
      onCurrentIndexChanged: tabRow.currentIndex = swipeView.currentIndex
      clip: true

      Repeater {
        // One page per tab in tabbed forms, 1 page in auto forms
        model: form.model.hasTabs ? tabRow.count : 1

        Flickable {
          id: contentView

          property int contentIndex: index

          width: form.width - form.leftMargin - form.rightMargin
          contentWidth: content.width
          contentHeight: content.height
          bottomMargin: form.bottomMargin
          clip: true
          ScrollBar.vertical: QfScrollBar {
          }
          boundsBehavior: Flickable.StopAtBounds

          Rectangle {
            anchors.fill: parent
            color: Theme.mainBackgroundColor
          }

          Flow {
            id: content
            width: form.width - form.leftMargin - form.rightMargin
            bottomPadding: 10

            SubModel {
              id: contentModel
              model: form.model
              rootIndex: form.model.index(form.model.hasTabs ? contentIndex : -1, 0)
            }

            Repeater {
              // Note: digitizing a child geometry will temporarily hide the feature form,
              // we need to preserve items so signal connections are kept alive
              model: form.model.hasTabs ? contentModel : form.model
              objectName: "fieldRepeater"
              delegate: fieldItem
            }
          }
        }
      }
    }
  }

  Component {
    id: spacerContainer

    Item {
      height: childrenRect.height
      anchors {
        left: parent.left
        right: parent.right
        leftMargin: 12
        rightMargin: 12
      }

      Item {
        width: parent.width
        height: 36

        Rectangle {
          anchors.centerIn: parent
          width: parent.width * 0.66
          height: 3
          radius: 1
          color: Theme.controlBackgroundAlternateColor
          visible: containerName !== ""
        }
      }
    }
  }

  Component {
    id: textContainer

    Item {
      height: childrenRect.height
      anchors {
        left: parent.left
        right: parent.right
        leftMargin: 12
        rightMargin: 12
      }

      Label {
        id: textLabel
        width: parent.width
        height: containerName !== '' ? undefined : topPadding + bottomPadding
        text: containerName
        wrapMode: Text.WordWrap
        font.pointSize: Theme.tinyFont.pointSize
        font.bold: true
        topPadding: 10
        bottomPadding: 5
        opacity: form.state === 'ReadOnly' || embedded && EditorWidget === 'RelationEditor' ? 0.45 : 1
        color: labelOverrideColor !== undefined && labelOverrideColor ? labelColor : Theme.mainTextColor
      }

      Text {
        id: textContent
        text: containerCode
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        anchors {
          left: parent.left
          right: parent.right
          top: textLabel.bottom
        }
        bottomPadding: 10
        opacity: textLabel.opacity
        color: Theme.mainTextColor
        linkColor: Theme.mainColor
        onLinkActivated: link => {
          Qt.openUrlExternally(link);
        }
      }
    }
  }

  Component {
    id: qmlContainer

    Item {
      height: childrenRect.height
      anchors {
        left: parent.left
        right: parent.right
        leftMargin: 12
      }

      Label {
        id: qmlLabel
        width: parent.width
        height: containerName !== '' ? undefined : topPadding + bottomPadding
        text: containerName
        wrapMode: Text.WordWrap
        font.pointSize: Theme.tinyFont.pointSize
        font.bold: true
        topPadding: 10
        bottomPadding: 5
        opacity: form.state === 'ReadOnly' || embedded && EditorWidget === 'RelationEditor' ? 0.45 : 1
        color: labelOverrideColor !== undefined && labelOverrideColor ? labelColor : Theme.mainTextColor
      }

      Item {
        id: qmlItem

        property string code: containerCode
        onCodeChanged: {
          var obj = Qt.createQmlObject(code, qmlItem, 'qmlContent');
        }

        height: childrenRect.height
        anchors {
          left: parent.left
          rightMargin: 12
          right: parent.right
          top: qmlLabel.bottom
        }
      }
    }
  }

  Component {
    id: htmlContainer

    Item {
      property string htmlCode: containerCode
      property var htmlItem: undefined

      height: childrenRect.height
      anchors {
        left: parent.left
        right: parent.right
        leftMargin: 12
      }

      Label {
        id: htmlLabel
        width: parent.width
        height: containerName !== '' ? undefined : topPadding + bottomPadding
        text: containerName
        wrapMode: Text.WordWrap
        font.pointSize: Theme.tinyFont.pointSize
        font.bold: true
        topPadding: 10
        bottomPadding: 5
        opacity: form.state === 'ReadOnly' || embedded && EditorWidget === 'RelationEditor' ? 0.45 : 1
        color: labelOverrideColor !== undefined && labelOverrideColor ? labelColor : Theme.mainTextColor
      }

      Item {
        id: htmlContent
        height: childrenRect.height
        anchors {
          left: parent.left
          rightMargin: 12
          right: parent.right
          top: htmlLabel.bottom
        }
      }

      onHtmlCodeChanged: {
        if (htmlItem === undefined) {
          htmlItem = Qt.createQmlObject('import QtWebView;
            WebView {
              id: htmlItem;
              height: 0;
              opacity: 0;
              anchors { top: parent.top; left: parent.left; right: parent.right; }
              onLoadingChanged: {
                if (!loading) {
                  runJavaScript("document.body.offsetHeight", function(result) {
                    anchors.left = parent.left;
                    width = parent.width;
                    height = (result + 18);
                    opacity = 1.0;
                  });
                }
              }
            }', htmlContent);
        }
        htmlItem.loadHtml(htmlCode);
      }
    }
  }

  Component {
    id: innerContainer

    Item {
      height: childrenRect.height
      anchors {
        left: parent.left
        right: parent.right
      }

      Flow {
        id: innerContainerContent
        anchors {
          left: parent.left
          right: parent.right
        }

        Repeater {
          model: SubModel {
            id: innerSubModel
            model: form.model
            rootIndex: form.model.mapFromSource(containerGroupIndex)
          }
          delegate: fieldItem
        }

        Connections {
          target: form.model

          function onModelReset() {
            if (containerGroupIndex !== undefined && innerContainer.visible) {
              innerSubModel.rootIndex = form.model.mapFromSource(containerGroupIndex);
            }
          }
        }
      }
    }
  }

  Component {
    id: dummyContainer

    Item {
    }
  }

  /**
   * A field editor
   */
  Component {
    id: fieldItem

    Item {
      width: parent && parent.width > 0 ? parent.width / ColumnCount > 190 ? parent.width / ColumnCount : parent.width : form.width
      height: fieldGroupTitle.height + fieldContent.childrenRect.height

      Rectangle {
        id: fieldGroupBackground

        anchors.fill: parent
        visible: GroupColor ? true : false
        color: GroupColor ? Qt.hsla(GroupColor.hslHue, GroupColor.hslSaturation, GroupColor.hslLightness, 0.05) : "transparent"
      }

      Rectangle {
        id: fieldGroupTitle

        width: parent.width
        height: GroupName !== '' ? childrenRect.height : 0
        color: GroupColor ? Qt.hsla(GroupColor.hslHue, GroupColor.hslSaturation, GroupColor.hslLightness, 0.5) : Theme.controlBorderColor

        Rectangle {
          width: 5
          height: parent.height
          anchors.top: parent.top
          anchors.left: parent.left
          color: GroupColor ? GroupColor : "transparent"
        }

        Text {
          leftPadding: 10
          rightPadding: 10
          topPadding: 5
          bottomPadding: 5
          width: parent.width
          font.pointSize: Theme.tipFont.pointSize
          font.bold: true
          color: Theme.mainTextColor
          text: GroupName || ''
          wrapMode: Text.WordWrap
        }
      }

      Item {
        id: fieldContent

        anchors {
          top: fieldGroupTitle.bottom
          left: parent.left
          right: parent.right
        }

        Loader {
          property string containerName: Name || ''
          property string containerCode: EditorWidgetCode || ''
          property var containerGroupIndex: GroupIndex
          property var labelOverrideColor: LabelOverrideColor
          property var labelColor: LabelColor
          property string itemType: Type

          active: (Type === 'container' && GroupIndex !== undefined && GroupIndex.valid) || ((Type === 'text' || Type === 'html' || Type === 'qml' || Type === 'spacer') && form.model.featureModel.modelMode != FeatureModel.MultiFeatureModel)
          height: status == Loader.Ready ? item.childrenRect.height : 0
          anchors {
            left: parent.left
            right: parent.right
          }

          sourceComponent: {
            if (Type === 'container') {
              if (GroupIndex !== undefined && GroupIndex.valid) {
                return innerContainer;
              }
            } else if (Type === 'qml') {
              return qmlContainer;
            } else if (Type === 'html') {
              return htmlContainer;
            } else if (Type === 'text') {
              return textContainer;
            } else if (Type === 'spacer') {
              return spacerContainer;
            }
            return dummyContainer;
          }
        }

        Item {
          id: fieldContainer

          property bool isVisible: (Type === 'field' && EditorWidget !== "Hidden") || Type === 'relation'

          visible: isVisible
          height: isVisible ? childrenRect.height : 0
          anchors {
            left: parent.left
            right: parent.right
            leftMargin: 12
          }

          Label {
            id: fieldLabel
            width: parent.width
            height: Name !== '' ? undefined : topPadding + bottomPadding
            text: Name || ''
            wrapMode: Text.WordWrap
            font.family: LabelOverrideFont ? LabelFont.family : Theme.tinyFont.family
            font.pointSize: Theme.tinyFont.pointSize
            font.bold: LabelOverrideFont ? LabelFont.bold : true
            font.italic: LabelOverrideFont ? LabelFont.italic : false
            font.underline: LabelOverrideFont ? LabelFont.underline : false
            font.strikeout: LabelOverrideFont ? LabelFont.strikeout : false
            topPadding: 10
            bottomPadding: 5
            opacity: !AttributeEditable && form.state === "Edit" ? (LabelOverrideColor ? 0.5 : 1.0) : 1.0
            color: LabelOverrideColor ? LabelColor : (!AttributeEditable && form.state === "Edit" ? Theme.mainTextDisabledColor : Theme.mainTextColor)
          }

          Label {
            id: constraintDescriptionLabel
            anchors {
              left: parent.left
              right: parent.right
              top: fieldLabel.bottom
              rightMargin: 10
            }

            font.pointSize: fieldLabel.font.pointSize * 0.8
            text: {
              if (ConstraintHardValid && ConstraintSoftValid)
                return '';
              return ConstraintDescription || '';
            }
            height: !ConstraintHardValid || !ConstraintSoftValid ? undefined : 0
            visible: !ConstraintHardValid || !ConstraintSoftValid
            opacity: fieldLabel.opacity
            color: !ConstraintHardValid ? Theme.errorColor : Theme.warningColor
            wrapMode: Text.WordWrap
          }

          Item {
            id: placeholder
            height: attributeEditorLoader.childrenRect.height
            anchors {
              left: parent.left
              right: fieldMenuButton.left
              top: constraintDescriptionLabel.bottom
              rightMargin: fieldMenuButton.visible ? 5 : 0
            }

            Loader {
              id: attributeEditorLoader
              objectName: "attributeEditorLoader" + Name

              anchors {
                left: parent.left
                right: parent.right
              }

              //disable widget if it's:
              // - not activated in multi edit mode
              // - not set to editable in the widget configuration
              // - not in edit mode (ReadOnly)
              // - a relation in multi edit mode
              property bool isAdding: form.state === 'Add'
              property bool isEditing: form.state !== 'ReadOnly'
              property bool isEnabled: !!AttributeEditable && form.state !== 'ReadOnly' && !(Type === 'relation' && form.model.featureModel.modelMode == FeatureModel.MultiFeatureModel)
              property bool isEditable: !!AttributeEditable && !(Type === 'relation' && form.model.featureModel.modelMode == FeatureModel.MultiFeatureModel)

              property var value: AttributeValue
              property var config: (EditorWidgetConfig || {})
              property var widget: EditorWidget
              property var relationEditorWidget: RelationEditorWidget
              property var relationEditorWidgetConfig: RelationEditorWidgetConfig
              property var field: Field
              property var fieldLabel: Name
              property var relationId: RelationId
              property var nmRelationId: NmRelationId
              property var constraintHardValid: ConstraintHardValid
              property var constraintSoftValid: ConstraintSoftValid
              property bool constraintsHardValid: form.model.constraintsHardValid
              property bool constraintsSoftValid: form.model.constraintsSoftValid
              property var currentFeature: form.model.featureModel.feature
              property var currentLayer: form.model.featureModel.currentLayer
              property bool autoSave: qfieldSettings.autoSave

              active: widget !== undefined && widget !== "Hidden"
              source: {
                if (widget === 'RelationEditor') {
                  return 'editorwidgets/relationeditors/' + (RelationEditorWidget || 'relation_editor') + '.qml';
                }
                return 'editorwidgets/' + (widget || 'TextEdit') + '.qml';
              }

              onLoaded: {
                item.isLoaded = true;
              }

              onStatusChanged: {
                if (attributeEditorLoader.status === Loader.Error) {
                  source = (widget === 'RelationEditor') ? 'editorwidgets/relationeditors/relation_editor.qml' : 'editorwidgets/TextEdit.qml';
                }
              }
            }

            Connections {
              target: form

              function onAboutToSave() {
                // it may not be implemented
                if (attributeEditorLoader.item.pushChanges) {
                  attributeEditorLoader.item.pushChanges(form.model.featureModel.feature);
                }
              }

              function onValueChanged(field, oldValue, newValue) {
                // it may not be implemented
                if (attributeEditorLoader.item && attributeEditorLoader.item.siblingValueChanged) {
                  attributeEditorLoader.item.siblingValueChanged(field, form.model.featureModel.feature);
                }
              }
            }

            Connections {
              target: attributeEditorLoader.item

              function onValueChangeRequested(value, isNull) {
                //do not compare AttributeValue and value with strict comparison operators
                if ((AttributeValue != value || (AttributeValue !== undefined && isNull)) && !(AttributeValue === undefined && isNull)) {
                  let oldValue = AttributeValue;
                  AttributeValue = isNull ? undefined : value;
                  valueChanged(Field, oldValue, AttributeValue);
                  if (!AttributeAllowEdit && form.model.featureModel.modelMode == FeatureModel.MultiFeatureModel) {
                    AttributeAllowEdit = true;
                  }
                  if (qfieldSettings.autoSave && !setupOnly && !master.ignoreChanges) {
                    // indirect action, no need to check for success and display a toast, the log is enough
                    save();
                  }
                }
              }
              function onRequestGeometry(item, layer) {
                form.digitizingToolbar.geometryRequested = true;
                form.digitizingToolbar.geometryRequestedItem = item;
                form.digitizingToolbar.geometryRequestedLayer = layer;
              }

              function onRequestBarcode(item) {
                form.codeReader.barcodeRequestedItem = item;
                form.codeReader.open();
              }

              function onRequestJumpToPoint(center, scale, handleMargins) {
                form.requestJumpToPoint(center, scale, handleMargins);
              }
            }
          }

          QfToolButton {
            id: fieldMenuButton
            anchors {
              right: rememberButton.left
              top: constraintDescriptionLabel.bottom
              rightMargin: 5
            }

            visible: attributeEditorLoader.isEnabled && attributeEditorLoader.item && attributeEditorLoader.item.hasMenu
            enabled: visible
            width: visible ? 48 : 0

            iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: "transparent"

            onClicked: {
              attributeEditorLoader.item.menu.popup(fieldMenuButton.x, fieldMenuButton.y);
            }
          }

          QfToolButton {
            id: rememberButton
            visible: !!CanRememberValue && form.state === "Add" && EditorWidget !== "Hidden" && EditorWidget !== 'RelationEditor'
            width: visible ? 48 : 0

            iconSource: Theme.getThemeVectorIcon("ic_pin_black_24dp")
            iconColor: RememberValue ? Theme.mainColor : Theme.mainTextDisabledColor
            bgcolor: "transparent"

            anchors {
              right: parent.right
              top: constraintDescriptionLabel.bottom
              verticalCenter: fieldMenuButton.verticalCenter
              rightMargin: visible ? 0 : 10
            }

            onClicked: {
              RememberValue = !RememberValue;
              if (RememberValue) {
                displayToast(qsTr("The last entered value for this field will be remembered and reused when creating new features"));
              } else {
                displayToast(qsTr("The last entered value for this field will not be reused when creating new features"));
              }
              projectInfo.saveLayerRememberedFields(form.model.featureModel.currentLayer);
            }
          }

          Label {
            id: multiEditAttributeLabel
            text: (AttributeAllowEdit ? qsTr("Value applied") : qsTr("Value skipped")) + qsTr(" (click to toggle)")
            visible: form.model.featureModel.modelMode == FeatureModel.MultiFeatureModel && Type !== 'relation'
            height: form.model.featureModel.modelMode == FeatureModel.MultiFeatureModel ? undefined : 0
            bottomPadding: form.model.featureModel.modelMode == FeatureModel.MultiFeatureModel ? 15 : 0
            anchors {
              left: parent.left
              top: placeholder.bottom
              rightMargin: 10
            }
            font: Theme.tipFont
            color: AttributeAllowEdit ? Theme.mainColor : Theme.secondaryTextColor

            MouseArea {
              anchors.fill: parent
              onClicked: {
                AttributeAllowEdit = !AttributeAllowEdit;
              }
            }
          }
        }
      }
    }
  }

  function confirm() {
    if (form.state === "ReadOnly") {
      return;
    }

    //if this is not handled before (e.g. when this is called because the drawer is closed by tipping on the map)
    if (!model.constraintsHardValid) {
      displayToast(qsTr('Hard constraints not satisfied'), 'error');
      cancel();
      return;
    } else if (!model.constraintsSoftValid) {
      displayToast(qsTr('Soft constraints were not satisified'));
    }
    parent.focus = true;
    if (setupOnly) {
      temporaryStored();
      return;
    }
    if (!save()) {
      featureCreated = false;
      return;
    }
    state = 'Edit';
    featureCreated = false;
    confirmed();
  }

  function save() {
    if (!model.constraintsHardValid) {
      return false;
    }
    aboutToSave();
    master.ignoreChanges = true;
    let isSuccess = false;
    let errorPushed = false;
    if (form.state === 'Add' && !featureCreated) {
      if (!model.featureModel.featureAdditionLocked) {
        isSuccess = model.create();
        featureCreated = isSuccess;
      } else {
        isSuccess = false;
        errorPushed = true;
        displayToast(qsTr('Feature addition disabled'), 'warning');
      }
    } else {
      isSuccess = model.save();
    }
    master.ignoreChanges = false;
    if (!isSuccess && !errorPushed) {
      displayToast(qsTr('Unable to save changes'), 'error');
    }
    return isSuccess;
  }

  function cancel() {
    if (form.state === 'Add' && featureCreated) {
      // indirect action, no need to check for success and display a toast, the log is enough
      model.deleteFeature();
    }
    cancelled();
    featureCreated = false;
  }

  Connections {
    target: Qt.inputMethod

    function onVisibleChanged() {
      Qt.inputMethod.commit();
    }
  }

  /** The title toolbar **/
  header: ToolBar {
    id: toolbar

    topPadding: 0
    leftPadding: 0
    rightPadding: 0
    bottomPadding: 0

    height: visible ? form.topMargin + 48 : 0
    visible: form.state === 'Add'
    objectName: "toolbar"
    background: Rectangle {
      color: "transparent"
    }

    Rectangle {
      width: parent.width * 0.3
      height: 5
      radius: 10

      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.top
      anchors.topMargin: form.topMargin + 6

      color: Theme.controlBorderColor
      visible: isDraggable
    }

    RowLayout {
      anchors.fill: parent
      anchors.topMargin: form.topMargin + 10
      anchors.leftMargin: form.leftMargin
      anchors.rightMargin: form.rightMargin

      QfToolButton {
        id: saveButton

        property bool isVisible: form.state === 'Add' || form.state === 'Edit'

        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        visible: isVisible
        width: 48
        height: 48
        clip: true

        iconSource: Theme.getThemeVectorIcon("ic_check_white_24dp")
        iconColor: (form.state === 'Add' && model.featureModel.featureAdditionLocked) || !model.constraintsHardValid ? Theme.mainOverlayColor : Theme.mainTextColor
        bgcolor: (form.state === 'Add' && model.featureModel.featureAdditionLocked) || !model.constraintsHardValid ? Theme.errorColor : !model.constraintsSoftValid ? Theme.warningColor : "transparent"
        borderColor: Theme.mainBackgroundColor
        roundborder: true
        round: true

        onClicked: {
          if (model.constraintsHardValid) {
            if (!model.constraintsSoftValid) {
              displayToast(qsTr('Soft constraints were not satisified'));
            }
            confirm();
          } else {
            displayToast(qsTr('Hard constraints not satisfied'), 'error');
          }
        }
      }

      Text {
        id: titleLabel

        Layout.fillWidth: true
        Layout.preferredHeight: parent.height
        Layout.leftMargin: form.state === "ReadOnly" || (!setupOnly && form.model.hasRemembrance) ? 48 : 0
        Layout.rightMargin: !setupOnly ? 0 : 48
        objectName: "titleLabel"

        font: Theme.strongFont
        color: Theme.mainTextColor

        text: {
          const featureModel = model.featureModel;
          var currentLayer = featureModel ? featureModel.currentLayer : null;
          var layerName = 'N/A';
          if (currentLayer !== null)
            layerName = currentLayer.name;
          if (form.state === 'Add')
            qsTr('Add feature on %1').arg(layerName);
          else if (form.state === 'Edit')
            qsTr('Edit feature on %1').arg(layerName);
          else
            qsTr('View feature on %1').arg(layerName);
        }

        fontSizeMode: Text.Fit
        wrapMode: Text.Wrap
        elide: Label.ElideRight
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        DragHandler {
          enabled: true
          target: null
          acceptedButtons: Qt.LeftButton
          grabPermissions: PointerHandler.CanTakeOverFromAnything
          dragThreshold: 5

          property var oldPos

          onActiveChanged: {
            if (active) {
              form.toolbarDragAcquired();
              oldPos = centroid.scenePosition;
            } else {
              form.toolbarDragReleased();
            }
          }

          onCentroidChanged: {
            if (active) {
              var dx = centroid.scenePosition.x - oldPos.x;
              var dy = centroid.scenePosition.y - oldPos.y;
              if (dx !== 0 || dy !== 0) {
                form.toolbarDragged(dx, dy);
                oldPos = centroid.scenePosition;
              }
            }
          }
        }
      }

      QfToolButton {
        id: closeButton

        property bool isVisible: !setupOnly

        Layout.alignment: Qt.AlignTop | Qt.AlignRight
        width: 48
        height: 48
        clip: true
        visible: isVisible

        iconSource: form.state === 'Add' ? Theme.getThemeVectorIcon('ic_delete_forever_white_24dp') : Theme.getThemeVectorIcon('ic_close_white_24dp')
        iconColor: Theme.mainTextColor

        onClicked: {
          Qt.inputMethod.hide();
          if ((form.state === 'Add' || form.state === 'Edit')) {
            form.requestCancel();
          } else {
            cancel();
          }
        }
      }

      QfToolButton {
        id: featureFormMenuButton

        property bool isVisible: !setupOnly && form.model.hasRemembrance

        Layout.alignment: Qt.AlignTop | Qt.AlignRight

        width: 48
        height: 48
        clip: true
        visible: isVisible

        iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
        iconColor: Theme.mainTextColor

        onClicked: {
          featureFormMenu.popup(featureFormMenuButton.x + featureFormMenuButton.width - featureFormMenuButton.width, featureFormMenuButton.y);
        }
      }
    }
  }

  QfMenu {
    id: featureFormMenu
    title: qsTr("Feature Form Menu")

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin

    MenuItem {
      text: qsTr('Remember All Reusable Values')

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding

      onTriggered: form.model.activateAllRememberValues()
    }

    MenuItem {
      text: qsTr('Forget All Reusable Values')

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding

      onTriggered: form.model.deactivateAllRememberValues()
    }
  }

  QfDialog {
    id: cancelDialog
    parent: mainWindow.contentItem
    z: 10000 // 1000s are embedded feature forms, user a higher value to insure the dialog will always show above embedded feature forms
    title: qsTr("Cancel editing")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: form.state === 'Add' ? qsTr("You are about to dismiss the new feature, proceed?") : qsTr("You are about to leave editing state, any changes will be lost. Proceed?")
    }
    onAccepted: {
      form.cancel();
    }
  }
}
