import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQml 2.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Item {
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

  Rectangle {
      width: elevationProfile.width
      height: elevationProfile.height
      color: "#ffffff"
      radius: 6
  }

  ElevationProfileCanvas {
    id: elevationProfileCanvas

    width: elevationProfile.width
    height: elevationProfile.height

    tolerance: crs.isGeographic ? 0.00005 : 5
  }

  Text {
    id: instrutionLabel
    visible: elevationProfileCanvas.profileCurve.isNull
    anchors.centerIn: parent
    width: parent.width
    color: Theme.gray
    font: Theme.tinyFont
    horizontalAlignment: Text.AlignHCenter
    wrapMode: Text.WordWrap
    text: qsTr('Digitize a path to render the elevation profile')
    style: Text.Outline
    styleColor: "white"
  }
}
