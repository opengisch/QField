import QtQuick
import QtQuick.Controls
import QtQml.Models
import org.qgis
import org.qfield
import Theme

Item {
  id: cogoTools

  property FeatureModel featureModel //<! the feature which has its geometry being edited
  /// type:QgsQuickMapSettings
  property MapSettings mapSettings
  property RubberbandModel rubberbandModel //<! the rubberband model attached to the ferature model to build its geometry

  property var cogoContainer: undefined

  signal requestJumpToPoint(var center, real scale, bool handleMargins)
  signal requestPosition(var item)

  // returns true if handled
  function canvasClicked(point, type) {
    if (cogoTool.item) {
      return cogoTool.item.canvasClicked(point, type);
    } else {
      return false;
    }
  }

  // returns true if handled
  function canvasLongPressed(point, type) {
    if (cogoTool.item) {
      return cogoTool.item.canvasLongPressed(point, type);
    } else {
      return false;
    }
  }

  width: content.contentWidth + cogoToolsContainer.spacing * 2
  height: 48
  enabled: false
  visible: enabled

  onEnabledChanged: {
    if (enabled && toolsModel.currentIndex == -1) {
      toolsModel.currentIndex = 0;
      cogoTool.load(toolsModel.get(0).parameters, toolsModel.get(0).name);
    }
    if (cogoContainer !== undefined) {
      cogoContainer.enabled = enabled;
    }
  }

  Container {
    id: cogoToolsContainer
    anchors.fill: parent
    spacing: 4
    clip: true
    focusPolicy: Qt.NoFocus

    contentItem: Rectangle {
      radius: 48 / 2
      color: Theme.toolButtonBackgroundSemiOpaqueColor
      clip: true

      ListView {
        id: content
        width: content.contentWidth
        height: 46
        x: 4
        y: 4
        model: cogoToolsContainer.contentModel
        snapMode: ListView.SnapToItem
        orientation: ListView.Horizontal
        spacing: 4
      }
    }

    ListModel {
      id: toolsModel

      property int currentIndex: -1

      ListElement {
        name: qsTr("Point at XY")
        iconPath: "ic_cogo_xy_white_24dp"
        parameters: [
          ListElement {
            parameterName: "point"
            parameterType: "point"
          }
        ]
      }
      ListElement {
        name: qsTr("Point at Angle/Distance")
        iconPath: "ic_cogo_angle_distance_white_24dp"
        parameters: [
          ListElement {
            parameterName: "point"
            parameterType: "point"
          },
          ListElement {
            parameterName: "distance"
            parameterType: "distance"
          },
          ListElement {
            parameterName: "angle"
            parameterType: "angle"
          }
        ]
      }
      ListElement {
        name: qsTr("Point at Circle's Intersection")
        iconPath: "ic_cogo_intersection_circles_white_24dp"
        parameters: [
          ListElement {
            parameterName: "point1"
            parameterType: "point"
            parameterLabel: "Circle #1"
          },
          ListElement {
            parameterName: "distance1"
            parameterType: "distance"
            parameterLabel: "Circle #1: Radius"
          },
          ListElement {
            parameterName: "point2"
            parameterType: "point"
            parameterLabel: "Circle #2"
          },
          ListElement {
            parameterName: "distance2"
            parameterType: "distance"
            parameterLabel: "Circle #2: Radius"
          }
        ]
      }
    }

    Repeater {
      model: toolsModel
      delegate: QfToolButton {
        width: 40
        height: 40
        padding: 2
        round: true
        bgcolor: toolsModel.currentIndex === index ? Theme.toolButtonBackgroundColor : Theme.toolButtonBackgroundSemiOpaqueColor
        iconSource: Theme.getThemeVectorIcon(iconPath)
        iconColor: toolsModel.currentIndex === index ? Theme.mainColor : Theme.toolButtonColor

        onClicked: {
          toolsModel.currentIndex = index;
          cogoTool.load(parameters, name);
        }
      }
    }
  }

  Loader {
    id: cogoTool

    property string cogoName: [{}]
    property var cogoParameters: [{}]

    active: cogoTools.enabled
    parent: cogoContainer
    width: parent.width

    sourceComponent: Component {
      id: cogoSettings

      CogoToolSettings {
        title: cogoName
        parameters: cogoParameters
      }
    }

    Connections {
      target: cogoTool.item

      function onRequestJumpToPoint(center, scale, handleMargins) {
        cogoTools.requestJumpToPoint(center, scale, handleMargins);
      }

      function onRequestPosition(item) {
        cogoTools.requestPosition(item);
      }
    }

    function load(parameters, name) {
      cogoName = name;
      cogoParameters = parameters;
      displayToast(name);
    }
  }
}
