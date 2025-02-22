import QtQuick
import QtQuick.Controls
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

  relationEditorModel: ReferencingFeatureListModel {
    //containing the current (parent) feature, the relation to the children
    //and the relation from the children to the other parent (if it's nm and cardinality is set)
    //if cardinality is not set, the nmRelationId is empty
    id: relationEditorModel
    currentRelationId: relationId
    currentNmRelationId: nmRelationId
    feature: currentFeature

    property int featureFocus: -1
    onModelUpdated: {
      if (featureFocus > -1) {
        referencingFeatureListView.currentIndex = relationEditorModel.getFeatureIdRow(featureFocus);
        featureFocus = -1;
      }
    }
  }

  listView.model: DelegateModel {
    model: relationEditorModel
    delegate: referencingFeatureDelegate
  }

  Component {
    id: referencingFeatureDelegate

    Item {
      id: listitem
      anchors.left: parent ? parent.left : undefined
      anchors.right: parent ? parent.right : undefined

      focus: true

      height: Math.max(itemHeight, featureText.height)

      Ripple {
        clip: true
        width: parent.width
        height: parent.height
        pressed: mouseArea.pressed
        anchor: listitem
        active: mouseArea.pressed
        color: Material.rippleColor
      }

      MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
          if (relationEditorModel.relation.referencingLayer !== undefined) {
            geometryHighlighter.geometryWrapper.qgsGeometry = nmRelationId ? model.nmReferencingFeature.geometry : model.referencingFeature.geometry;
            geometryHighlighter.geometryWrapper.crs = relationEditorModel.relation.referencingLayer.crs;
            mapCanvas.mapSettings.extent = FeatureUtils.extent(mapCanvas.mapSettings, relationEditorModel.relation.referencingLayer, nmRelationId ? model.nmReferencingFeature : model.referencingFeature);
          }
        }
      }

      Row {
        id: itemRow
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        height: listitem.height

        Text {
          id: featureText
          anchors.verticalCenter: parent.verticalCenter
          width: parent.width - viewButton.width - menuButton.width
          topPadding: 5
          bottomPadding: 5
          font: Theme.defaultFont
          color: !isEnabled ? Theme.mainTextDisabledColor : Theme.mainTextColor
          elide: Text.ElideRight
          wrapMode: Text.WordWrap
          text: nmRelationId ? model.nmDisplayString : model.displayString
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
            embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly';
            embeddedPopup.currentLayer = nmRelationId ? relationEditorModel.nmRelation.referencedLayer : relationEditorModel.relation.referencingLayer;
            embeddedPopup.linkedRelation = relationEditorModel.relation;
            embeddedPopup.linkedParentFeature = relationEditorModel.feature;
            embeddedPopup.feature = nmRelationId ? model.nmReferencedFeature : model.referencingFeature;
            embeddedPopup.open();
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
            //var gc = mapToItem(mainWindow, 0, 0);
            childMenu.entryReferencingFeature = model.referencingFeature;
            childMenu.entryDisplayString = model.displayString;
            childMenu.entryNmReferencedFeature = nmRelationId ? model.model.nmReferencedFeature : undefined;
            childMenu.entryNmReferencedFeatureDisplayMessage = nmRelationId ? model.nmDisplayString : '';
            childMenu.popup(menuButton.x, menuButton.y);
          }
        }
      }

      //bottom line
      Rectangle {
        id: bottomLine
        anchors.bottom: parent.bottom
        height: 1
        color: Theme.controlBorderColor
        width: parent.width
      }
    }
  }
}
