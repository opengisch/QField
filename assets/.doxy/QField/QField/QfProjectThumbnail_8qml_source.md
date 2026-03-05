

# File QfProjectThumbnail.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfProjectThumbnail.qml**](QfProjectThumbnail_8qml.md)

[Go to the documentation of this file](QfProjectThumbnail_8qml.md)


```C++
import QtQuick
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import org.qfield

Rectangle {
  id: thumbnail

  property alias previewImageSource: previewImage.source
  property alias showType: type.visible
  property alias projectTypeSource: type.source
  property alias projectNote: projectNote.text
  property alias projectTitle: projectTitle
  property alias bgColor: thumbnailContentRectangle.color
  property alias showCustomizeIcon: customizeIcon.active
  property alias selected: selectedIndicator.active
  property alias primaryBadge: primaryStatusBadge

  property bool isPressed: false
  property bool fillHeight: false

  width: parent ? parent.width : undefined
  height: thumbnailContentRectangle.height + 8
  implicitHeight: 120
  color: "transparent"

  Rectangle {
    id: lineMask
    width: thumbnailContentRectangle.width
    height: thumbnailContentRectangle.height
    radius: 10
    color: "white"
    visible: false
    layer.enabled: true
  }

  Rectangle {
    id: thumbnailContentRectangle
    width: parent.width
    height: (previewImage.status === Image.Ready || fillHeight) ? parent.implicitHeight : detailsContainer.height
    anchors.verticalCenter: parent.verticalCenter
    color: "transparent"
    clip: true

    layer.enabled: true
    layer.effect: QfOpacityMask {
      maskSource: lineMask
    }

    Image {
      id: previewImage
      width: parent.width
      height: parent.height
      fillMode: Image.PreserveAspectCrop
    }

    Loader {
      id: customizeIcon
      active: false
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.verticalCenter: parent.verticalCenter
      anchors.verticalCenterOffset: -8
      sourceComponent: QfToolButton {
        iconSource: Theme.getThemeVectorIcon('ic_tune_white_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"
        round: true
      }
    }

    Ripple {
      clip: true
      width: thumbnailContentRectangle.width
      height: thumbnailContentRectangle.height
      pressed: thumbnail.isPressed
      active: thumbnail.isPressed
      color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.15)
    }

    Rectangle {
      id: detailsContainer
      color: Qt.hsla(Theme.mainBackgroundColor.hslHue, Theme.mainBackgroundColor.hslSaturation, Theme.mainBackgroundColor.hslLightness, Theme.darkTheme ? 0.75 : 0.95)
      width: parent.width
      height: details.childrenRect.height + details.topPadding + details.bottomPadding
      anchors.bottom: parent.bottom

      Row {
        id: details
        width: parent.width
        topPadding: 3
        bottomPadding: 3
        spacing: 0

        ParametizedImage {
          id: type
          anchors.verticalCenter: parent.verticalCenter
          width: 40
          height: visible ? 40 : 0
          strokeColor: Theme.mainColor
          parameters: {
            "cloud": Theme.cloudColor
          }
          visible: source !== ""

          QfBadge {
            id: primaryStatusBadge
            z: 2
            alignment: QfBadge.Alignment.BottomRight
            visible: false
            width: type.width / 2.5
            height: width
            border.width: 2
            bottomMargin: -2
          }
        }

        ColumnLayout {
          id: inner
          anchors.verticalCenter: parent.verticalCenter
          width: thumbnail.width - type.width - 20
          spacing: 2
          clip: true

          Text {
            id: projectTitle
            topPadding: 4
            leftPadding: type.visible ? 3 : 10
            bottomPadding: projectNote.visible ? 0 : 5
            font.pointSize: Theme.tipFont.pointSize
            color: Theme.mainTextColor
            opacity: thumbnail.isPressed ? 0.8 : 1
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          Text {
            id: projectNote
            leftPadding: 3
            bottomPadding: 4
            visible: text !== ""
            font.pointSize: Theme.tipFont.pointSize - 2
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }
        }
      }
    }

    Rectangle {
      anchors.fill: parent
      color: "transparent"
      border.color: Theme.controlBorderColor
      radius: 10
    }

    Loader {
      id: selectedIndicator
      active: false
      anchors.fill: parent
      sourceComponent: Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: Theme.mainColor
        radius: 10

        Rectangle {
          width: 25
          height: width
          radius: width / 2
          color: Theme.mainColor
          anchors.top: parent.top
          anchors.right: parent.right
          anchors.margins: 8
          opacity: visible

          Behavior on opacity {
            NumberAnimation {
              duration: 200
            }
          }

          Image {
            anchors.fill: parent
            anchors.margins: 4
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/themes/qfield/nodpi/ic_check_white_24dp.svg"
            sourceSize.width: width * Screen.devicePixelRatio
            sourceSize.height: height * Screen.devicePixelRatio
          }
        }
      }
    }
  }
}
```


