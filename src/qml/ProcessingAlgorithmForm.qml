import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.14
import QtQuick.Controls.Material.impl 2.14

import Theme 1.0
import org.qfield 1.0

Item {
  id: processingAlgorithmForm

  property alias algorithmId: processingAlgorithmParametersModel.algorithmId
  property alias algorithmDisplayName: processingAlgorithmParametersModel.algorithmDisplayName
  property alias algorithmShortHelp: processingAlgorithmParametersModel.algorithmShortHelp
  property real bottomMargin: 0

  ProcessingAlgorithmParametersModel {
    id: processingAlgorithmParametersModel
    filters: ProcessingAlgorithmParametersModel.GeneralParameterFilter

    onAlgorithmIdChanged: {
      filters = ProcessingAlgorithmParametersModel.GeneralParameterFilter
    }
  }

  ColumnLayout {
    id: container
    anchors.fill: parent

    QfTabBar {
      id: tabRow
      Layout.fillWidth: true
      Layout.preferredHeight: defaultHeight

      visible: processingAlgorithmParametersModel.hasAdvancedParameters
      model: [qsTr("General Parameters"), qsTr("Advanced Parameters")]

      delegate: TabButton {
        id: tabButton

        property bool isCurrentIndex: index == tabRow.currentIndex

        text: modelData
        topPadding: 0
        bottomPadding: 0
        leftPadding: 8
        rightPadding: 8
        width: contentItem.width + leftPadding + rightPadding
        height: 48

        onClicked: {
          tabRow.currentIndex = index
        }

        background: Rectangle {
          implicitWidth: parent.width
          implicitHeight: parent.height
          color: "transparent"
        }

        contentItem: Text {
          width: paintedWidth
          height: parent.height
          text: tabButton.text
          color: !tabButton.enabled ? Theme.darkGray : tabButton.down ? Qt.darker(Theme.mainColor, 1.5) : Theme.mainColor
          font.pointSize: Theme.tipFont.pointSize
          font.weight: isCurrentIndex ? Font.DemiBold : Font.Normal

          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
        }
      }

      onCurrentIndexChanged: {
        processingAlgorithmParametersModel.filters = currentIndex == 0 ? ProcessingAlgorithmParametersModel.GeneralParameterFilter : ProcessingAlgorithmParametersModel.AdvancedParameterFilter
      }
    }

    Flickable {
      id: contentView
      Layout.topMargin: 5
      Layout.fillWidth: true
      Layout.fillHeight: true

      contentWidth: content.width
      contentHeight: content.height
      bottomMargin: processingAlgorithmForm.bottomMargin
      clip: true

      ScrollBar.vertical: ScrollBar {
        policy: content.height > contentView.height ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
        width: 6
        contentItem: Rectangle {
          implicitWidth: 6
          implicitHeight: 25
          color: Theme.mainColor
        }
      }

      Rectangle {
        anchors.fill: parent
        color: Theme.mainBackgroundColor
      }

      Flow {
        id: content
        width: processingAlgorithmForm.width

        Repeater {
          // Note: digitizing a child geometry will temporarily hide the feature form,
          // we need to preserve items so signal connections are kept alive
          model: processingAlgorithmParametersModel
          delegate: parameterItem
        }
      }
    }
  }

  Component {
    id: parameterItem

    Item {
      width: processingAlgorithmForm.width
      height: parameterDescription.height + parameterContent.childrenRect.height

      Label {
        id: parameterDescription
        leftPadding: 10
        rightPadding: 10
        topPadding: 5
        bottomPadding: 5
        width: parent.width
        wrapMode: Text.WordWrap
        text: ParameterDescription
        font: Theme.tipFont
      }

      Item {
        id: parameterContent

        anchors {
          top: parameterDescription.bottom
          left: parent.left
          right: parent.right
          leftMargin: 10
          rightMargin: 10
        }

        Item {
          id: parameterWidgetPlaceholder
          visible: true
          height: parameterWidgetLoader.childrenRect.height
          anchors {
            left: parent.left
            right: parent.right
          }

          Loader {
            id: parameterWidgetLoader
            anchors { left: parent.left; right: parent.right }

            property var value: ParameterValue
            property string widget: ParameterType

            active: true
            source: {
              return 'processingparameterwidgets/' + widget + '.qml'
            }
          }

          Connections {
            target: parameterWidgetLoader.item

            function onValueChangeRequested(value) {
              ParameterValue = value
            }
          }
        }
      }
    }
  }
}
