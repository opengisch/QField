// src/plugins/voice-polygon/main.qml
import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

Item {
  id: voicePolygonPlugin

  property string surveyType:   ""
  property var    originPoint:  null
  property string originCrs:    ""

  QfToolButton {
    id: voicePolygonButton
    iconSource: Qt.resolvedUrl("icons/voice_polygon.svg")
    iconColor:  Theme.toolButtonColor
    bgcolor:    Theme.toolButtonBackgroundColor
    round:      true
    tooltip:    "VoicePolygon — Levantar por voz"
    onClicked:  surveyTypeSelector.open()
  }

  MapPolygonOverlay {
    id: mapOverlay
    parent: iface.mapCanvas()
    anchors.fill: parent
    visible: false
  }

  VoicePolygonDialog {
    id: voiceDialog
    parent: iface.mainWindow().contentItem
    surveyType:  voicePolygonPlugin.surveyType
    originPoint: voicePolygonPlugin.originPoint
    originCrs:   voicePolygonPlugin.originCrs
    mapOverlay:  mapOverlay

    onPolygonSaved: {
      mapOverlay.visible = false
      mapOverlay.reset()
      voicePolygonPlugin.surveyType  = ""
      voicePolygonPlugin.originPoint = null
    }
  }

  SurveyTypeSelector {
    id: surveyTypeSelector
    parent: iface.mainWindow().contentItem

    onTypeSelected: function(type) {
      voicePolygonPlugin.surveyType = type
      surveyTypeSelector.close()
      mapPointPicker.activate(type)
    }
  }

  MapPointPicker {
    id: mapPointPicker
    parent: iface.mapCanvas()
    anchors.fill: parent

    onPointPicked: function(point, crs) {
      voicePolygonPlugin.originPoint = point
      voicePolygonPlugin.originCrs   = crs
      mapPointPicker.deactivate()

      mapOverlay.setOrigin(point, crs)
      mapOverlay.visible = true

      voiceDialog.open()
      iface.mainWindow().displayToast("🎙️ Habla para trazar — " + voicePolygonPlugin.surveyType)
    }

    onCancelled: {
      voicePolygonPlugin.surveyType  = ""
      voicePolygonPlugin.originPoint = null
      iface.mainWindow().displayToast("Operación cancelada")
    }
  }

  Component.onCompleted: {
    iface.addItemToPluginsToolbar(voicePolygonButton)
    iface.mainWindow().displayToast("🎙️ VoicePolygon v2 listo")
  }
}
