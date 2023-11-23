import QtQuick 2.14
import QtQuick.Controls 2.14

import Theme 1.0

Container {
  id: container

  property string name: ''
  property real size: 48
  property bool downward: true
  property bool vertical: true
  property bool collapsed: true
  property alias bgcolor: toggleButton.bgcolor
  property alias iconSource: toggleButton.iconSource
  property alias iconColor: toggleButton.iconColor

  onNameChanged: {
    collapsed = settings.valueBool("QField/QfToolButtonContainer/"+name+"/collapsed", true)
  }

  width: vertical
         ? size
         : collapsed
           ? size
           : size + content.contentWidth + container.spacing * 2
  height: vertical
          ? collapsed
            ? size
            : size + content.contentHeight + container.spacing * 2
          : size
  spacing: 4
  clip: true

  Behavior on width {
    enabled: true
    NumberAnimation { duration: 200 }
  }

  Behavior on height {
    enabled: true
    NumberAnimation { duration: 200 }
  }

  contentItem: Rectangle {
    radius: size / 2
    color: Qt.hsla(toggleButton.bgcolor.hslHue, toggleButton.bgcolor.hslSaturation, toggleButton.bgcolor.hslLightness, 0.4)
    clip: true

    ListView {
      id: content
      width: content.contentWidth
      height: content.contentHeight
      x: container.downward ? container.vertical ? container.spacing : container.size + container.spacing : container.spacing
      y: container.downward ? container.vertical ? container.size + container.spacing : container.spacing : container.spacing
      model: container.contentModel
      snapMode: ListView.SnapToItem
      orientation: container.vertical ? ListView.Vertical : ListView.Horizontal
      spacing: container.spacing
    }

    QfToolButton {
      id: toggleButton
      width: container.size
      height: container.size
      x: container.downward ? 0 : container.width - container.size
      y: container.downward ? 0 : container.height - container.size
      round: true

      onClicked: {
        container.collapsed = !container.collapsed
        if (name != "") {
          settings.setValue("QField/QfToolButtonContainer/"+name+"/collapsed", container.collapsed)
        }
      }
    }
  }
}
