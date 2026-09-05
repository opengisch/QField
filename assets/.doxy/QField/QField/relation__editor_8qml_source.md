

# File relation\_editor.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_aa4aab3cdee284f0e217d9df55b13787.md) **>** [**relationeditors**](dir_aa066cf6db9752ba8872dc4f8960076b.md) **>** [**relation\_editor.qml**](relation__editor_8qml.md)

[Go to the documentation of this file](relation__editor_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield.core
import org.qfield.gui
import org.qgis

QfRelationEditorBase {
  id: relationEditor

  relationEditorModel: QfReferencingFeatureListModel {
    //containing the current (parent) feature, the relation to the children
    //and the relation from the children to the other parent (if it's nm and cardinality is set)
    //if cardinality is not set, the nmRelationId is empty
    id: referencingFeatureListModel
    currentRelationId: relationId
    currentNmRelationId: nmRelationId ? nmRelationId : ""
    feature: currentFeature

    property int featureFocus: -1
    onModelUpdated: {
      if (featureFocus > -1) {
        listView.currentIndex = referencingFeatureListModel.getFeatureIdRow(featureFocus);
        featureFocus = -1;
      }
    }
  }

  onToggleSortAction: {
    referencingFeatureListModel.sortOrder = referencingFeatureListModel.sortOrder === Qt.AscendingOrder ? Qt.DescendingOrder : Qt.AscendingOrder;
  }

  listView.model: DelegateModel {
    model: referencingFeatureListModel
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
          if (referencingFeatureListModel.relation.referencingLayer !== undefined) {
            if (referencingFeatureListModel.relation.referencingLayer.geometryType() !== Qgis.GeometryType.Null && referencingFeatureListModel.relation.referencingLayer.geometryType() !== Qgis.GeometryType.Unknown) {
              geometryHighlighter.geometryWrapper.qgsGeometry = nmRelationId ? model.nmReferencingFeature.geometry : model.referencingFeature.geometry;
              geometryHighlighter.geometryWrapper.crs = referencingFeatureListModel.relation.referencingLayer.crs;
              const extentRect = QfFeatureUtils.extent(mapCanvas.mapSettings, referencingFeatureListModel.relation.referencingLayer, nmRelationId ? model.nmReferencingFeature : model.referencingFeature);
              const scale = mapCanvas.mapSettings.computeScaleForExtent(extentRect, true);
              relationEditor.requestJumpToPoint(extentRect.center, scale, true);
            } else {
              viewButton.click();
            }
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
          font: QfTheme.resultFont
          color: (!isEditable && isEditing) ? QfTheme.mainTextDisabledColor : QfTheme.mainTextColor
          elide: Text.ElideRight
          wrapMode: Text.WordWrap
          text: nmRelationId ? model.nmDisplayString : model.displayString
        }

        QfToolButton {
          id: viewButton
          anchors.verticalCenter: parent.verticalCenter
          width: QfTheme.toolButtonSize
          height: QfTheme.toolButtonSize

          round: false
          iconSource: isEnabled ? QfTheme.getThemeVectorIcon('ic_edit_attributes_white_24dp') : QfTheme.getThemeVectorIcon('ic_baseline-list_white_24dp')
          iconColor: QfTheme.mainTextColor
          bgcolor: 'transparent'

          onClicked: {
            showViewFeaturePopup(nmRelationId ? model.nmReferencedFeature : model.referencingFeature);
          }
        }

        QfToolButton {
          id: menuButton
          anchors.verticalCenter: parent.verticalCenter
          width: QfTheme.toolButtonSize
          height: QfTheme.toolButtonSize

          round: false
          iconSource: QfTheme.getThemeVectorIcon("ic_dot_menu_black_24dp")
          iconColor: QfTheme.mainTextColor
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
    }
  }
}
```


