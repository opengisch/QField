import QtQuick
import QtQuick.Controls
import QtQml.Models
import org.qgis
import org.qfield
import Theme

/**
 * This contains several COGO tools
 * A tool must subclass QfOverlayContainer and contains following functions:
 *  - function init(featureModel, mapSettings, editorRubberbandModel)
 *  - function cancel()
 * The following signal:
 *  - signal finished()
 * It can optionally implement the functions:
 *  - canvasClicked(point)
 *  - canvasLongPressed(point)
 * These functions must return true if they catch the event.
 *
 * \ingroup qml
 */
Item {
  property FeatureModel featureModel //<! the feature which has its geometry being edited
  /// type:QgsQuickMapSettings
  property MapSettings mapSettings
  property RubberbandModel rubberbandModel //<! the rubberband model attached to the ferature model to build its geometry

  property var cogoContainer: undefined

  signal toolChanged
  signal requestJumpToPoint(var center, real scale, bool handleMargins)

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

  width: content.contentWidth + cogoToolbar.spacing * 2
  height: 48

  Container {
    id: cogoToolbar

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
        model: cogoToolbar.contentModel
        snapMode: ListView.SnapToItem
        orientation: ListView.Horizontal
        spacing: 4
      }
    }

    ListModel {
      id: toolsModel

      property string currentQmlPath: ""

      ListElement {
        name: qsTr("Point at X/Z")
        iconPath: "ic_vertex_tool_white_24dp"
        qmlPath: "cogo/PointAtXZ.qml"
      }
      ListElement {
        name: qsTr("Point at Angle/Distance")
        iconPath: "ic_vertex_tool_white_24dp"
        qmlPath: "cogo/PointAtAngleDistance.qml"
      }
    }

    Repeater {
      model: toolsModel
      delegate: QfToolButton {
        width: 40
        height: 40
        padding: 2
        round: true
        bgcolor: toolsModel.currentQmlPath === qmlPath ? Theme.toolButtonBackgroundColor : Theme.toolButtonBackgroundSemiOpaqueColor
        iconSource: Theme.getThemeVectorIcon(iconPath)
        iconColor: toolsModel.currentQmlPath === qmlPath ? Theme.mainColor : Theme.toolButtonColor
        onClicked: {
          toolsModel.currentQmlPath = qmlPath;
          cogoTool.load(qmlPath, name);
        }
      }
    }
  }

  Loader {
    id: cogoTool
    parent: cogoContainer
    width: parent.width

    function load(qmlPath, name) {
      source = qmlPath;
      displayToast(name);
    }

    Connections {
      target: cogoTool.item

      function onFinished() {
      }

      function requestJumpToPoint(center, scale, handleMargins) {
        cogoToolbar.requestJumpToPoint(center, scale, handleMargins);
      }
    }
  }
}
