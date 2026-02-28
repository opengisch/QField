

# File ProcessingAlgorithmForm.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**ProcessingAlgorithmForm.qml**](ProcessingAlgorithmForm_8qml.md)

[Go to the documentation of this file](ProcessingAlgorithmForm_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield
import Theme

Item {
  id: processingAlgorithmForm

  property ProcessingAlgorithmParametersModel algorithmParametersModel: processingAlgorithmParametersModel
  property alias inPlaceLayer: processingAlgorithmParametersModel.inPlaceLayer
  property alias algorithmId: processingAlgorithmParametersModel.algorithmId
  property alias algorithmDisplayName: processingAlgorithmParametersModel.algorithmDisplayName
  property alias algorithmShortHelp: processingAlgorithmParametersModel.algorithmShortHelp
  property real bottomMargin: 0

  ProcessingAlgorithmParametersModel {
    id: processingAlgorithmParametersModel
    filters: ProcessingAlgorithmParametersModel.GeneralParameterFilter

    onAlgorithmIdChanged: {
      filters = ProcessingAlgorithmParametersModel.GeneralParameterFilter;
    }
  }

  ColumnLayout {
    id: container
    anchors.fill: parent

    QfTabBar {
      id: tabRow
      Layout.fillWidth: true
      Layout.preferredHeight: defaultHeight

      model: processingAlgorithmParametersModel.hasAdvancedParameters ? [qsTr("General Parameters"), qsTr("Advanced Parameters"), qsTr("Help")] : [qsTr("General Parameters"), qsTr("Help")]

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
          tabRow.currentIndex = index;
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
        processingAlgorithmParametersModel.filters = currentIndex == 0 ? ProcessingAlgorithmParametersModel.GeneralParameterFilter : ProcessingAlgorithmParametersModel.AdvancedParameterFilter;
      }
    }

    Flickable {
      id: contentView
      Layout.topMargin: 5
      Layout.fillWidth: true
      Layout.fillHeight: true
      visible: tabRow.currentIndex !== tabRow.model.length - 1
      contentWidth: content.width
      contentHeight: content.height
      bottomMargin: processingAlgorithmForm.bottomMargin
      clip: true
      ScrollBar.vertical: QfScrollBar {}

      Rectangle {
        anchors.fill: parent
        color: Theme.mainBackgroundColor
      }

      Flow {
        id: content
        width: processingAlgorithmForm.width

        Repeater {
          id: contentRepeater

          // Note: digitizing a child geometry will temporarily hide the feature form,
          // we need to preserve items so signal connections are kept alive
          model: processingAlgorithmParametersModel
          delegate: parameterItem
        }
      }

      Text {
        width: processingAlgorithmForm.width
        height: processingAlgorithmForm.height - tabRow.height - 100
        visible: contentRepeater.count == 0
        padding: 10

        font: Theme.tipFont
        color: Theme.secondaryTextColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap

        text: qsTr("There are no general parameters attached to this algorithm.")
      }
    }

    Flickable {
      id: descriptionView
      Layout.topMargin: 5
      Layout.fillWidth: true
      Layout.fillHeight: true
      visible: tabRow.currentIndex === tabRow.model.length - 1

      contentWidth: descriptionContent.width
      contentHeight: descriptionContent.height
      bottomMargin: processingAlgorithmForm.bottomMargin
      clip: true

      ScrollBar.vertical: ScrollBar {
        policy: descriptionContent.height > descriptionView.height ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
        width: 6
        contentItem: Rectangle {
          implicitWidth: 6
          implicitHeight: 25
          color: Theme.mainColor
        }
      }

      Column {
        id: descriptionContent
        width: processingAlgorithmForm.width
        leftPadding: 10
        rightPadding: 10
        spacing: 10

        Text {
          width: parent.width - parent.leftPadding - parent.rightPadding
          visible: tabRow.currentIndex === tabRow.model.length - 1

          font: Theme.strongFont
          color: Theme.mainTextColor
          wrapMode: Text.WordWrap

          text: qsTr('Algorithm description')
        }

        Text {
          width: parent.width - parent.leftPadding - parent.rightPadding
          visible: tabRow.currentIndex === tabRow.model.length - 1

          font: Theme.tipFont
          color: Theme.mainTextColor
          wrapMode: Text.WordWrap

          text: processingAlgorithmParametersModel.algorithmShortHelp
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

            property var value: ParameterValue
            property string widget: ParameterType
            property var configuration: ParameterConfiguration

            anchors {
              left: parent.left
              right: parent.right
            }

            active: true
            source: {
              return 'processingparameterwidgets/' + widget + '.qml';
            }
          }

          Connections {
            target: parameterWidgetLoader.item

            function onValueChangeRequested(value) {
              ParameterValue = value;
            }
          }
        }
      }
    }
  }
}
```


