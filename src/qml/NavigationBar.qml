/***************************************************************************
                            NavigationBar.qml
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


import QtQuick 2.12
import QtQuick.Controls 2.12

import org.qgis 1.0
import Theme 1.0
import org.qfield 1.0

Rectangle {
  id: toolBar

  property bool allowDelete
  property MultiFeatureListModel model
  property FeatureListModelSelection selection
  property FeaturelistExtentController extentController

  signal statusIndicatorClicked
  signal statusIndicatorSwiped(var direction)
  signal editAttributesButtonClicked
  signal editGeometryButtonClicked
  signal save
  signal cancel

  signal toggleMultiSelection
  signal multiEditClicked
  signal multiMergeClicked
  signal multiDeleteClicked

  anchors.top:parent.top
  anchors.left: parent.left
  anchors.right: parent.right
  height: 48

  clip: true

  states: [
    State {
      name: "Navigation"
    },
    State {
      name: "Indication"
    },
    State {
      name: "Edit"
    }
  ]

  state: "Indication"

  Rectangle {
    id: navigationStatusIndicator
    anchors.fill: parent
    height: 48

    color: ( featureFormList.model.constraintsHardValid && featureFormList.model.constraintsSoftValid ) || parent.state !== "Edit" ? Theme.mainColor : !featureFormList.model.constraintsHardValid ? Theme.errorColor : Theme.warningColor

    clip: true
    focus: true

    Text {
      font: Theme.strongFont
      color: Theme.light
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.leftMargin: 0
          + (saveButton.visible ? saveButton.width : 0)
          + (previousButton.visible ? previousButton.width : 0)
          + (nextButton.visible ? nextButton.width : 0)
      anchors.rightMargin: 0
          + (cancelButton.visible ? cancelButton.width : 0)
          + (editButton.visible ? editButton.width : 0)
          + (editGeomButton.visible ? editGeomButton.width : 0)
          + (multiClearButton.visible ? multiClearButton.width : 0)
          + (multiEditButton.visible ? multiEditButton.width : 0)
          + (menuButton.visible ? menuButton.width : 0)
      height: parent.height

      text: {
        if ( model && selection && selection.focusedItem > -1 && (toolBar.state === 'Navigation' || toolBar.state === 'Edit') ) {
          var featurePosition = model.count > 1
              ? ( ( selection.focusedItem + 1 ) + '/' + model.count + ': ' )
              : '';

          return featurePosition + FeatureUtils.displayName(selection.focusedLayer, selection.focusedFeature)
        }
        else {
          return qsTr('Features')
        }
      }
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      fontSizeMode: Text.Fit
      wrapMode: Text.Wrap
      elide: Text.ElideRight
    }

    MouseArea {
      anchors.fill: parent

      property real velocity: 0.0
      property int startY: 0
      property int lastY: 0
      property int distance: 0
      property bool isTracing: false

      preventStealing: true

      onPressed: {
        startY = mouse.y
        lastY = mouse.y
        velocity = 0
        distance = 0
        isTracing = true
      }
      onPositionChanged: {
        if ( !isTracing )
          return

        var currentVelocity = Math.abs(mouse.y - lastY)
        lastY = mouse.y
        velocity = (velocity + currentVelocity) / 2.0
        distance = Math.abs(mouse.y - startY)
        isTracing = velocity > 15 && distance > parent.height
      }
      onReleased: {
        if ( !isTracing ) {
          toolBar.statusIndicatorSwiped(getDirection())
        } else {
          toolBar.statusIndicatorClicked()
        }
      }

      function getDirection() {
        return lastY < startY ? 'up' : 'down'
      }
    }
  }

  QfToolButton {
    id: nextButton

    anchors.left: previousButton.right

    width: ( parent.state == "Navigation" ? 48: 0 )
    height: 48
    clip: true

    iconSource: Theme.getThemeIcon( "ic_chevron_right_white_24dp" )

    enabled: ( toolBar.model && ( selection.focusedItem + 1 ) < toolBar.model.count )

    onClicked: {
      selection.focusedItem = selection.focusedItem + 1
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  QfToolButton {
    id: previousButton

    anchors.left: parent.left

    width: ( parent.state == "Navigation" ? 48: 0 )
    height: 48
    clip: true

    iconSource: Theme.getThemeIcon( "ic_chevron_left_white_24dp" )

    enabled: ( selection.focusedItem > 0 )

    onClicked: {
      selection.focusedItem = selection.focusedItem - 1
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  QfToolButton {
    id: saveButton
    anchors.left: parent.left
    width: ( parent.state == "Edit" ? 48: 0 )
    height: 48
    clip: true
    visible: !qfieldSettings.autoSave

    iconSource: featureFormList.model.constraintsHardValid ? Theme.getThemeIcon( "ic_check_white_48dp" ) : Theme.getThemeIcon( "ic_check_gray_48dp" )
    onClicked: {
     if( featureFormList.model.constraintsHardValid ) {
       toolBar.save()
     } else {
       displayToast( "Constraints not valid" )
     }
    }
    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  QfToolButton {
    id: cancelButton
    visible: !qfieldSettings.autoSave

    anchors.right: parent.right

    width: ( parent.state == "Edit" ? 48: 0 )
    height: 48
    clip: true

    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )

    onClicked: {
      selection.focusedItemChanged()
      toolBar.cancel()
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  QfToolButton {
    id: editGeomButton

    property bool readOnly: false

    visible: stateMachine.state === "digitize"
             && ! selection.focusedGeometry.isNull
             && ! selection.focusedLayer.customProperty( "QFieldSync/is_geometry_locked", false )

    anchors.right: editButton.left

    iconSource: Theme.getThemeIcon( "ic_edit_geometry_white" )

    width: ( parent.state == "Navigation" && !readOnly ? 48: 0 )
    height: 48
    clip: true

    onClicked: {
      extentController.zoomToSelected(true)
      toolBar.editGeometryButtonClicked()
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }

    Connections {
      target: selection

      function onFocusedItemChanged() {
        editGeomButton.readOnly = selection.focusedLayer && selection.focusedLayer.readOnly
      }
    }
  }

  QfToolButton {
    id: editButton

    property bool supportsEditing: false

    anchors.right: menuButton.left

    width: ( parent.state == "Navigation" && supportsEditing ? 48: 0 )
    height: 48
    clip: true

    iconSource: Theme.getThemeIcon( "ic_edit_attributes_white" )

    onClicked: {
      toolBar.editAttributesButtonClicked()
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }

    Connections {
      target: selection

      function onFocusedItemChanged() {
        editButton.supportsEditing = selection.focusedLayer && selection.focusedLayer.supportsEditing
      }
    }
  }

  QfToolButton {
    id: menuButton

    anchors.right: parent.right

    visible: parent.state != "Edit"
    width: visible ? 48 : 0
    height: 48
    clip: true

    iconSource: Theme.getThemeIcon( "ic_dot_menu_white_24dp" )

    onClicked: {
        console.log(parent.state);
        if ( parent.state == "Indication" ) {
            featureListMenu.popup(menuButton.x + menuButton.width - featureListMenu.width, menuButton.y);
        } else if ( parent.state == "Navigation" ) {
            featureMenu.popup(menuButton.x + menuButton.width - featureMenu.width, menuButton.y);
        }
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  QfToolButton {
    id: multiClearButton

    anchors.left: parent.left

    width: ( parent.state == "Indication" && toolBar.model && toolBar.model.selectedCount > 0  ? 48: 0 )
    height: 48
    clip: true

    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )

    enabled: ( toolBar.model && toolBar.model.selectedCount > 0 )

    onClicked: {
      toolBar.model.clearSelection();
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  QfToolButton {
    id: multiEditButton

    anchors.right: menuButton.left

    width: ( parent.state == "Indication" && toolBar.model && toolBar.model.canEditAttributesSelection && toolBar.model.selectedCount > 1 ? 48: 0 )
    height: 48
    clip: true

    iconSource: Theme.getThemeIcon( "ic_edit_attributes_white" )

    enabled: ( toolBar.model && toolBar.model.canEditAttributesSelection && toolBar.model.selectedCount > 1 )

    onClicked: {
      multiEditClicked();
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  Menu {
    id: featureListMenu
    title: qsTr( "Feature List Menu" )

    width: {
        var result = 0;
        var padding = 0;
        for (var i = 0; i < count; ++i) {
            var item = itemAt(i);
            result = Math.max(item.contentItem.implicitWidth, result);
            padding = Math.max(item.padding, padding);
        }
        return result + padding * 2;
    }

    MenuItem {
      text: qsTr( 'Toggle Feature Selection' )

      font: Theme.defaultFont
      height: 48
      leftPadding: 10

      onTriggered: toggleMultiSelection();
    }

    MenuSeparator { width: parent.width }

    MenuItem {
      text: qsTr( 'Merge Selected Features' )
      icon.source: Theme.getThemeIcon( "ic_merge_features_white_24dp" )
      enabled: toolBar.model && toolBar.model.canMergeSelection && toolBar.model.selectedCount > 1

      font: Theme.defaultFont
      height: 48
      leftPadding: 10

      onTriggered: multiMergeClicked();
    }

    MenuItem {
      text: qsTr( 'Delete Selected Feature(s)' )
      icon.source: Theme.getThemeIcon( "ic_delete_forever_white_24dp" )
      enabled: toolBar.model && toolBar.model.canDeleteSelection

      font: Theme.defaultFont
      height: 48
      leftPadding: 10

      onTriggered: multiDeleteClicked();
    }
  }

  Menu {
    id: featureMenu
    title: qsTr( "Feature Menu" )

    width: {
        var result = 0;
        var padding = 0;
        for (var i = 0; i < count; ++i) {
            var item = itemAt(i);
            result = Math.max(item.contentItem.implicitWidth, result);
            padding = Math.max(item.padding, padding);
        }
        return result + padding * 2;
    }

    MenuItem {
      text: qsTr( 'Zoom to Feature' )
      icon.source: Theme.getThemeIcon( "ic_fullscreen_white_24dp" )

      font: Theme.defaultFont
      height: 48
      leftPadding: 10

      onTriggered: extentController.zoomToSelected();
    }

    MenuItem {
      text: qsTr( 'Auto-Zoom to Feature' )

      font: Theme.defaultFont
      height: 48
      leftPadding: 10

      checked: extentController.autoZoom

      onTriggered: extentController.autoZoom = !extentController.autoZoom
    }
  }
}
