import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
QfVisibilityFadingRow {
  id: markupToolbar

  property QfMarkupCollection markupCollection
  property QfRubberbandModel rubberbandModel
  property MapSettings mapSettings

  property var pickedColor: undefined

  readonly property bool isMarking: rubberbandModel && markupToolbar.pickedColor != undefined

  spacing: 4

  Item {
    id: markupColorItem

    width: (markupColorContent.contentWidth / markupColorContainer.contentModel.count * 3.5) + markupColorContainer.spacing * 2
    height: QfTheme.toolButtonSize

    Container {
      id: markupColorContainer
      anchors.fill: parent
      spacing: 4
      clip: true
      focusPolicy: Qt.NoFocus

      contentItem: Rectangle {
        id: markupColorContentItem
        radius: QfTheme.toolButtonSize / 2
        color: QfTheme.toolButtonBackgroundSemiOpaqueColor
        clip: true

        ListView {
          id: markupColorContent
          width: markupColorContainer.width - 10
          height: markupColorContainer.height - 2
          x: 4
          y: 4
          model: markupColorContainer.contentModel
          snapMode: ListView.SnapToItem
          orientation: ListView.Horizontal
          spacing: 4
          clip: true
        }
      }

      Repeater {
        model: ["#e41a1c", "#377eb8", "#4daf4a", "#984ea3", "#ff7f00"] // Colobrewer 5-class qualitative set
        delegate: QfToolButton {
          width: 40
          height: 40
          padding: 2
          round: true
          bgcolor: "transparent"

          Rectangle {
            anchors.centerIn: parent
            width: markupToolbar.pickedColor == modelData ? 40 : 28
            height: width
            radius: width / 2
            color: modelData
          }

          onClicked: {
            markupToolbar.pickedColor = modelData;
          }
        }
      }
    }
  }

  QfToolButton {
    id: newCollectionButton
    round: true
    //iconSource: QfTheme.getThemeVectorIcon("ic_clear_white_24dp")
    bgcolor: QfTheme.toolButtonBackgroundColor

    onClicked: {
      //TODO
    }
  }

  function processMarkup() {
    if (!rubberbandModel) {
      return;
    }

    if (markupCollection) {
      rubberbandModel.smoothSegment(0, rubberbandModel.vertexCount - 2, mapSettings.mapUnitsPerPoint * 1.5);
      const geometry = QfGeometryUtils.geometryFromRubberband(rubberbandModel, QfCoordinateReferenceSystemUtils.wgs84Crs(), mapSettings.mapUnitsPerPoint);
      const item = markupCollection.createItem("", "", geometry, pickedColor);
      const uuid = markupCollection.addItem(item);
    }

    pickedColor = undefined;
    rubberbandModel.reset();
  }
}
