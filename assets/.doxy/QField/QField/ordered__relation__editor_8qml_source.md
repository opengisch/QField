

# File ordered\_relation\_editor.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**relationeditors**](dir_755943273b0ca8aa2c962f9336c80f06.md) **>** [**ordered\_relation\_editor.qml**](ordered__relation__editor_8qml.md)

[Go to the documentation of this file](ordered__relation__editor_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQml.Models
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield
import org.qgis
import Theme
import "../.."
import ".."

RelationEditorBase {
  id: relationEditor

  showAllItems: true
  showSortButton: false

  relationEditorModel: OrderedRelationModel {
    //containing the current (parent) feature, the relation to the children
    //and the relation from the children to the other parent (if it's nm and cardinality is set)
    id: orderedRelationModel
    currentRelationId: relationId
    feature: currentFeature
    orderingField: relationEditorWidgetConfig['ordering_field'] || ""
    imagePath: relationEditorWidgetConfig['image_path'] || ""
    description: relationEditorWidgetConfig['description'] || ""

    property int featureFocus: -1

    onFailedReorder: {
      displayToast("Failed to reorder features.", "error");
    }
  }

  listView.model: DelegateModel {
    model: orderedRelationModel
    delegate: dragDelegate
  }

  Component {
    id: dragDelegate

    MouseArea {
      id: dragArea
      pressAndHoldInterval: 130

      property bool held: false

      property int indexFrom: -1
      property int indexTo: -1

      anchors.left: parent ? parent.left : undefined
      anchors.right: parent ? parent.right : undefined
      height: listitem.height

      drag.target: held ? listitem : undefined
      drag.axis: Drag.YAxis

      onPressAndHold: {
        if (isEnabled) {
          held = true;
        }
      }
      onReleased: {
        if (held === true) {
          held = false;
          orderedRelationModel.moveItems(indexFrom, indexTo);
        } else if (listView.currentIndex !== dragArea.DelegateModel.itemsIndex) {
          listView.currentIndex = dragArea.DelegateModel.itemsIndex;
          orderedRelationModel.triggerViewCurrentFeatureChange(listView.currentIndex);
        }
      }

      onClicked: {
        if (orderedRelationModel.relation.referencingLayer !== undefined) {
          if (orderedRelationModel.relation.referencingLayer.geometryType() !== Qgis.GeometryType.Null && orderedRelationModel.relation.referencingLayer.geometryType() !== Qgis.GeometryType.Unknown) {
            geometryHighlighter.geometryWrapper.qgsGeometry = nmRelationId ? model.nmReferencingFeature.geometry : model.referencingFeature.geometry;
            geometryHighlighter.geometryWrapper.crs = orderedRelationModel.relation.referencingLayer.crs;
            mapCanvas.mapSettings.extent = FeatureUtils.extent(mapCanvas.mapSettings, orderedRelationModel.relation.referencingLayer, nmRelationId ? model.nmReferencingFeature : model.referencingFeature);
          } else {
            viewButton.click();
          }
        }
      }

      Rectangle {
        id: listitem
        anchors {
          horizontalCenter: parent.horizontalCenter
          verticalCenter: parent.verticalCenter
        }
        width: dragArea.width
        height: row.implicitHeight

        Ripple {
          clip: true
          width: parent.width
          height: parent.height
          pressed: dragArea.pressed
          anchor: listitem
          active: dragArea.pressed
          color: Material.rippleColor
        }

        color: "transparent"

        radius: 2
        Drag.active: dragArea.held
        Drag.source: dragArea
        Drag.hotSpot.x: width / 2
        Drag.hotSpot.y: height / 2

        states: State {
          when: dragArea.held

          AnchorChanges {
            target: listitem
            anchors.horizontalCenter: undefined
            anchors.verticalCenter: undefined
          }
        }

        Row {
          id: row
          anchors.fill: parent
          anchors.rightMargin: 10
          anchors.leftMargin: 10
          height: listitem.height

          Image {
            id: featureImage
            source: ImagePath ? UrlUtils.fromString(ImagePath) : Theme.getThemeVectorIcon("ic_photo_notavailable_black_24dp")
            anchors.verticalCenter: parent.verticalCenter
            width: 48
            height: 48
            fillMode: Image.PreserveAspectFit
            mipmap: true
            visible: !!ImagePath
          }

          Text {
            id: featureText
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width - (featureImage.visible ? featureImage.width : 0) - viewButton.width - moveDownButton.width - moveUpButton.width - menuButton.width
            topPadding: 5
            bottomPadding: 5
            leftPadding: featureImage.visible ? 5 : 0
            font: Theme.defaultFont
            color: (!isEditable && isEditing) ? Theme.mainTextDisabledColor : Theme.mainTextColor
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
            text: Description || model.displayString
          }

          QfToolButton {
            id: viewButton
            anchors.verticalCenter: parent.verticalCenter
            width: 48
            height: 48

            round: false
            iconSource: isEnabled ? Theme.getThemeVectorIcon('ic_edit_attributes_white_24dp') : Theme.getThemeVectorIcon('ic_baseline-list_white_24dp')
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'

            onClicked: {
              ensureEmbeddedFormLoaded();
              embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly';
              embeddedPopup.currentLayer = orderedRelationModel.relation.referencingLayer;
              embeddedPopup.linkedRelation = orderedRelationModel.relation;
              embeddedPopup.linkedRelationOrderingField = orderedRelationModel.orderingField;
              embeddedPopup.linkedParentFeature = orderedRelationModel.feature;
              embeddedPopup.feature = model.referencingFeature;
              embeddedPopup.open();
            }
          }

          QfToolButton {
            id: moveDownButton
            anchors.verticalCenter: parent.verticalCenter
            visible: isEnabled
            width: visible ? 48 : 0
            height: 48
            opacity: (index === listView.count - 1) ? 0.3 : 1

            round: false
            iconSource: Theme.getThemeVectorIcon('ic_chevron_down')
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'

            onClicked: {
              if (index === listView.count - 1) {
                return;
              }
              orderedRelationModel.moveItems(index, index + 1);
            }
          }

          QfToolButton {
            id: moveUpButton
            anchors.verticalCenter: parent.verticalCenter
            visible: isEnabled
            width: visible ? 48 : 0
            height: 48
            opacity: (index === 0) ? 0.3 : 1

            round: false
            iconSource: Theme.getThemeVectorIcon('ic_chevron_up')
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'

            onClicked: {
              if (index === 0) {
                return;
              }
              orderedRelationModel.moveItems(index, index - 1);
            }
          }

          QfToolButton {
            id: menuButton
            anchors.verticalCenter: parent.verticalCenter
            width: 48
            height: 48

            round: false
            iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'

            onClicked: {
              childMenu.entryReferencingFeature = model.referencingFeature;
              childMenu.entryDisplayString = model.displayString;
              childMenu.entryNmReferencedFeature = nmRelationId ? model.model.nmReferencedFeature : undefined;
              childMenu.entryNmReferencedFeatureDisplayMessage = nmRelationId ? model.nmDisplayString : '';
              childMenu.popup(menuButton.x, menuButton.y);
            }
          }
        }
      }

      DropArea {
        anchors.fill: parent
        anchors.margins: 10

        onEntered: drag => {
          if (dragArea.indexFrom === -1) {
            dragArea.indexFrom = drag.source.DelegateModel.itemsIndex;
          }
          dragArea.indexTo = dragArea.DelegateModel.itemsIndex;
          listView.model.items.move(drag.source.DelegateModel.itemsIndex, dragArea.DelegateModel.itemsIndex);
        }
      }
    }
  }
}
```


