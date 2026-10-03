

# File QfElevationProfile.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfElevationProfile.qml**](QfElevationProfile_8qml.md)

[Go to the documentation of this file](QfElevationProfile_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQml
import org.qgis
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: elevationProfile

  property alias project: elevationProfileCanvas.project
  property alias crs: elevationProfileCanvas.crs
  property alias profileCurve: elevationProfileCanvas.profileCurve
  property alias tolerance: elevationProfileCanvas.tolerance

  function populateLayersFromProject() {
    elevationProfileCanvas.populateLayersFromProject();
  }

  function refresh() {
    elevationProfileCanvas.refresh();
  }

  function clear() {
    elevationProfileCanvas.clear();
  }

  color: QfTheme.mainBackgroundColorSemiOpaque
  radius: 0

  ElevationProfileCanvas {
    id: elevationProfileCanvas

    width: elevationProfile.width
    height: elevationProfile.height

    tolerance: crs.isGeographic ? 0.00005 : 5

    backgroundColor: QfTheme.mainBackgroundColorSemiOpaque
    borderColor: QfTheme.controlBackgroundAlternateColor
    axisLabelColor: QfTheme.secondaryTextColor
    axisLabelSize: QfTheme.tipFont.pointSize
  }

  ProgressBar {
    id: busyIndicator
    anchors.top: parent.top
    anchors.left: parent.left
    width: parent.width
    height: 6
    value: 50
    indeterminate: elevationProfileCanvas.isRendering ? true : false

    state: elevationProfileCanvas.isRendering ? "on" : "off"

    visible: opacity > 0

    states: [
      State {
        name: 'on'
        PropertyChanges {
          target: busyIndicator
          opacity: 1.0
        }
      },
      State {
        name: 'off'
        PropertyChanges {
          target: busyIndicator
          opacity: 0.0
        }
      }
    ]
    transitions: [
      Transition {
        from: "off"
        to: "on"
        SequentialAnimation {
          NumberAnimation {
            target: busyIndicator
            property: 'opacity'
            duration: 100
          }
        }
      },
      Transition {
        from: "on"
        to: "off"
        SequentialAnimation {
          PauseAnimation {
            duration: 100
          }
          NumberAnimation {
            target: busyIndicator
            property: 'opacity'
            duration: 200
          }
        }
      }
    ]
  }

  Text {
    id: instrutionLabel
    visible: elevationProfileCanvas.isRendering || elevationProfileCanvas.profileCurve.isNull
    anchors.centerIn: parent
    width: parent.width
    color: QfTheme.mainTextColor
    font: QfTheme.tinyFont
    horizontalAlignment: Text.AlignHCenter
    wrapMode: Text.WordWrap
    text: elevationProfileCanvas.isRendering ? qsTr('Rendering elevation profile…') : qsTr('Digitize a path to render the elevation profile')
    style: Text.Outline
    styleColor: QfTheme.mainBackgroundColorSemiOpaque
  }
}
```


