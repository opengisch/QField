import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.14
import QtQuick.Controls.Material.impl 2.14

import Theme 1.0
import org.qfield 1.0

Item {
  id: processingAlgorithmsList

  property alias filters: processingAlgorithmsModel.filters
  property alias inPlaceLayer: processingAlgorithmsModel.inPlaceLayer

  signal algorithmSelected(var id)

  Rectangle {
    anchors.fill: parent
    color: Theme.controlBackgroundColor
  }

  ListView {
    id: processingAlgorithmsListView
    anchors.fill: parent
    clip: true

    model: ProcessingAlgorithmsModel {
      id: processingAlgorithmsModel
      filters: ProcessingAlgorithmsModel.InPlaceFilter
    }

    section.property: "AlgorithmGroup"
    section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
    section.delegate: Component {
      Rectangle {
        width: parent.width
        height: 30
        color: Theme.controlBorderColor

        Text {
          anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
          font.bold: true
          font.pointSize: Theme.resultFont.pointSize
          color: Theme.mainTextColor
          text: section
        }
      }
    }

    delegate: Rectangle {
      id: itemBackground
      anchors {
        left: parent ? parent.left : undefined
        right: parent ? parent.right: undefined
      }
      focus: true
      height: Math.max( 48, itemText.height )
      color: "transparent"

      Ripple {
          clip: true
          width: parent.width
          height: parent.height
          pressed: mouseArea.pressed
          anchor: itemBackground
          active: mouseArea.pressed
          color: Material.rippleColor
      }

      Text {
        id: itemText
        anchors {
          leftMargin: 10
          left: parent.left
          verticalCenter: parent.verticalCenter
        }
        font.bold: true
        font.pointSize: Theme.resultFont.pointSize
        color: Theme.mainTextColor
        text: AlgorithmName
      }

      MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
          processingAlgorithmsList.algorithmSelected(AlgorithmId)
        }
      }

      Rectangle {
        anchors.bottom: parent.bottom
        height: 1
        color: Theme.controlBorderColor
        width: parent.width
      }
    }
  }
}
