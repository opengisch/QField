import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
ListView {
  id: markupLegend

  property bool isVisible: false
  property bool allowActiveCollectionChange: true
  property QfMarkupCollection activeCollection

  model: markupManager.collections
  flickableDirection: Flickable.VerticalFlick
  boundsBehavior: Flickable.StopAtBounds
  clip: true
  spacing: 0

  delegate: Rectangle {
    id: rectangle

    property QfMarkupCollection collection: modelData
    property string collectionName: modelData.name
    property string collectionUuid: modelData.uuid
    property bool isSelectedCollection: markupLegend.activeCollection == collection
    property bool isVisibleCollection: markupManager.hiddenCollectionUuids.indexOf(collectionUuid) === -1

    width: ListView.view.width
    height: line.height + 7
    color: isSelectedCollection ? QfTheme.mainColor : "transparent"
    radius: 2

    MouseArea {
      id: mouseArea
      anchors.fill: parent
      enabled: true
      acceptedButtons: Qt.LeftButton | Qt.RightButton
      onClicked: mouse => {
        if (allowActiveCollectionChange) {
          markupLegend.activeCollection = collection;
        }
      }
    }

    Ripple {
      clip: true
      width: parent.width
      height: parent.height
      pressed: mouseArea.pressed
      anchor: parent
      active: mouseArea.pressed
      color: Material.rippleColor
    }

    Row {
      id: line
      anchors.verticalCenter: parent.verticalCenter
      leftPadding: 29
      spacing: 0

      Row {
        anchors.verticalCenter: parent.verticalCenter
        spacing: 5

        Item {
          id: collectionVisibility

          height: 24
          width: parent.height
          anchors.verticalCenter: parent.verticalCenter
          visible: true

          QfToolButton {
            height: 35
            width: height
            opacity: isVisibleCollection ? 1 : 0.25
            anchors.centerIn: parent
            iconSource: isVisibleCollection ? QfTheme.getThemeVectorIcon('ic_show_green_48dp') : QfTheme.getThemeVectorIcon('ic_hide_green_48dp')
            iconColor: isSelectedCollection ? QfTheme.mainOverlayColor : QfTheme.mainTextColor
            bgcolor: "transparent"
            enabled: true
            onClicked: {
              let hiddenCollectionUuids = markupManager.hiddenCollectionUuids;
              const idx = hiddenCollectionUuids.indexOf(collectionUuid);
              if (idx === -1) {
                hiddenCollectionUuids.push(collectionUuid);
              } else {
                hiddenCollectionUuids.splice(idx, 1);
              }
              markupManager.hiddenCollectionUuids = hiddenCollectionUuids;
            }
          }
        }

        Item {
          anchors.verticalCenter: parent.verticalCenter
          height: 24
          width: 24

          Image {
            anchors.fill: parent
            anchors.margins: 4
            fillMode: Image.PreserveAspectFit
            cache: false
            smooth: true
            mipmap: true
            source: QfTheme.getThemeVectorIcon('ic_markuplayer_18dp')
            opacity: isVisibleCollection ? 1 : 0.25
          }
        }

        Text {
          id: markupCollectionName
          width: rectangle.width - collectionVisibility.width - line.leftPadding - 5
          padding: 3
          leftPadding: 3
          text: collectionName
          horizontalAlignment: Text.AlignLeft
          font: QfTheme.tipFont
          elide: Text.ElideRight
          opacity: isVisibleCollection ? 1 : 0.25
          color: {
            if (isSelectedCollection) {
              return QfTheme.mainOverlayColor;
            }
            return QfTheme.mainTextColor;
          }
        }
      }
    }
  }
}
