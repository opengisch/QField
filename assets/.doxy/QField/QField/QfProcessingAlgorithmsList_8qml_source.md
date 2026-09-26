

# File QfProcessingAlgorithmsList.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfProcessingAlgorithmsList.qml**](QfProcessingAlgorithmsList_8qml.md)

[Go to the documentation of this file](QfProcessingAlgorithmsList_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield.core
import org.qfield.gui

Item {
  id: processingAlgorithmsList

  property alias filters: processingAlgorithmsModel.filters
  property alias inPlaceLayer: processingAlgorithmsModel.inPlaceLayer

  signal algorithmSelected(var id)

  Rectangle {
    anchors.fill: parent
    color: QfTheme.controlBackgroundColor
  }

  ColumnLayout {
    id: container
    anchors.fill: parent

    QfTabBar {
      id: tabRow
      Layout.fillWidth: true
      Layout.preferredHeight: defaultHeight

      model: [qsTr("Favorites"), qsTr("All Algorithms")]

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
          color: !tabButton.enabled ? QfTheme.darkGray : tabButton.down ? Qt.darker(QfTheme.mainColor, 1.5) : QfTheme.mainColor
          font.pointSize: QfTheme.tipFont.pointSize
          font.weight: isCurrentIndex ? Font.DemiBold : Font.Normal

          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
        }
      }

      onCurrentIndexChanged: {
        processingAlgorithmsModel.filters = currentIndex == 0 ? QfProcessingAlgorithmsModel.InPlaceFilter | QfProcessingAlgorithmsModel.FavoriteFilter : QfProcessingAlgorithmsModel.InPlaceFilter;
      }
    }

    ListView {
      id: processingAlgorithmsListView
      Layout.fillWidth: true
      Layout.fillHeight: true
      clip: true

      model: QfProcessingAlgorithmsModel {
        id: processingAlgorithmsModel
        filters: QfProcessingAlgorithmsModel.InPlaceFilter | QfProcessingAlgorithmsModel.FavoriteFilter
      }

      section.property: "AlgorithmGroup"
      section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
      section.delegate: Component {
        Rectangle {
          width: parent.width
          height: 30
          color: QfTheme.controlBorderColor

          Text {
            anchors {
              horizontalCenter: parent.horizontalCenter
              verticalCenter: parent.verticalCenter
            }
            font: QfTheme.strongResultFont
            color: QfTheme.mainTextColor
            text: section
          }
        }
      }

      delegate: Rectangle {
        id: itemBackground
        anchors {
          left: parent ? parent.left : undefined
          right: parent ? parent.right : undefined
        }
        focus: true
        height: Math.max(48, itemText.height)
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
          width: parent.width - favoriteButton.width - 10
          font: QfTheme.resultFont
          color: QfTheme.mainTextColor
          text: AlgorithmName
        }

        MouseArea {
          id: mouseArea
          anchors.fill: parent

          onClicked: {
            processingAlgorithmsList.algorithmSelected(AlgorithmId);
          }
        }

        QfToolButton {
          id: favoriteButton
          anchors {
            rightMargin: 5
            right: parent.right
            verticalCenter: parent.verticalCenter
          }
          width: QfTheme.toolButtonSize

          iconSource: QfTheme.getThemeVectorIcon("ic_star_white_24dp")
          iconColor: AlgorithmFavorite ? QfTheme.mainColor : QfTheme.mainTextColor
          opacity: AlgorithmFavorite ? 1.0 : 0.75

          onClicked: {
            AlgorithmFavorite = !AlgorithmFavorite;
          }
        }

        Rectangle {
          anchors.bottom: parent.bottom
          height: 1
          color: QfTheme.controlBorderColor
          width: parent.width
        }
      }
    }
  }
}
```


