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
  property string activeCollectionName: ""

  model: markupManager.collectionNames
  flickableDirection: Flickable.VerticalFlick
  boundsBehavior: Flickable.StopAtBounds
  clip: true
  spacing: 0

  delegate: Rectangle {
    id: rectangle

    property string collectionName: modelData
    property bool isSelectedCollection: collectionName == activeCollectionName

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
        activeCollectionName = collectionName;
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
            opacity: true ? 1 : 0.25
            anchors.centerIn: parent
            iconSource: true ? QfTheme.getThemeVectorIcon('ic_hide_green_48dp') : QfTheme.getThemeVectorIcon('ic_show_green_48dp')
            iconColor: isSelectedCollection ? QfTheme.mainOverlayColor : QfTheme.mainTextColor
            bgcolor: "transparent"
            enabled: true
            onClicked: {
              //TODO: markup collection visibility
            }
          }
        }

        Text {
          id: markupCollectionName
          width: rectangle.width - collectionVisibility.width - line.leftPadding - 5
          padding: 3
          leftPadding: 0
          text: collectionName
          horizontalAlignment: Text.AlignLeft
          font: QfTheme.tipFont
          elide: Text.ElideRight
          opacity: true ? 1 : 0.25
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
