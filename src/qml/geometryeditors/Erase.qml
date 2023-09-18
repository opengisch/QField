import QtQuick 2.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

import ".."

VisibilityFadingRow {
  id: eraseToolbar

  signal finished()

  property FeatureModel featureModel
  property GeometryRenderer editorRenderer
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  property int size: sizeMediumButton.sizeValue

  readonly property bool blocking: drawPolygonToolbar.isDigitizing

  spacing: 4

  function canvasClicked(point)
  {
    drawPolygonToolbar.addVertex()
    return true // handled
  }

  function canvasLongPressed(point)
  {
    drawPolygonToolbar.confirm()
    return true // handled
  }

  function canvasFreehandBegin()
  {
    geometryEditorsRubberband.visible = false
    drawPolygonToolbar.rubberbandModel.measureValue = 0.0
    drawPolygonToolbar.addVertex()
    adjustSize()
    return true // handled
  }

  function canvasFreehandEnd()
  {
    drawPolygonToolbar.rubberbandModel.measureValue = 0.0
    drawPolygonToolbar.addVertex()
    return true // handled
  }

  DigitizingToolbar {
    id: drawPolygonToolbar
    showConfirmButton: true
    screenHovering: eraseToolbar.screenHovering

    digitizingLogger.type: 'edit_erase'
    digitizingLogger.digitizingLayer: featureModel.currentLayer

    function addVertex() {
      digitizingLogger.addCoordinate( coordinateLocator.currentCoordinate )
      coordinateLocator.flash()

      rubberbandModel.addVertex()
    }

    onConfirmed: {
      digitizingLogger.writeCoordinates()

      rubberbandModel.frozen = true
      if (!featureModel.currentLayer.editBuffer())
          featureModel.currentLayer.startEditing()
      var result = GeometryUtils.eraseFromRubberband(featureModel.currentLayer, featureModel.feature.id, rubberbandModel)
      if ( result !== GeometryUtils.Success )
      {
          displayToast( qsTr( 'The geometry could not be erased' ), 'error' );
          featureModel.currentLayer.rollBack()
          rubberbandModel.reset()
      }
      else
      {
          featureModel.currentLayer.commitChanges()
          rubberbandModel.reset()
          featureModel.refresh()
          featureModel.applyGeometryToVertexModel()
      }
      rubberbandModel.reset()
      eraseToolbar.editorRenderer.geometryWrapper.clear()
    }

    onCancel: {
      rubberbandModel.reset()
      eraseToolbar.editorRenderer.geometryWrapper.clear()
    }

    onVertexCountChanged: {
      editorRenderer.geometryWrapper.crs = featureModel.currentLayer.crs
      editorRenderer.geometryWrapper.qgsGeometry = GeometryUtils.variableWidthBufferByMFromRubberband(drawPolygonToolbar.rubberbandModel, featureModel.currentLayer.crs)
    }
  }

  Connections {
    target: drawPolygonToolbar.mapSettings

    function onExtentChanged() {
      adjustSize();
    }
  }

  QfToolButton {
    id: sizeSmallButton
    iconSource: Theme.getThemeVectorIcon( "ic_size_small_white_24dp" )
    round: true
    visible: true
    bgcolor: Theme.darkGray

    property int sizeValue: 2
    state: eraseToolbar.size == sizeValue ? "On" : "Off"

    states: [
      State {
        name: "Off"
        PropertyChanges {
          target: sizeSmallButton
          iconColor: "white"
          bgcolor: Qt.hsla(Theme.darkGray.hslHue, Theme.darkGray.hslSaturation, Theme.darkGray.hslLightness, 0.3)
        }
      },

      State {
        name: "On"
        PropertyChanges {
          target: sizeSmallButton
          iconColor: Theme.mainColor
          bgcolor: Theme.darkGray
        }
      }
    ]

    onClicked: {
      eraseToolbar.size = sizeValue;
      adjustSize();
    }
  }

  QfToolButton {
    id: sizeMediumButton
    iconSource: Theme.getThemeVectorIcon( "ic_size_medium_white_24dp" )
    round: true
    visible: true
    bgcolor: Theme.darkGray

    property int sizeValue: 5
    state: eraseToolbar.size == sizeValue ? "On" : "Off"

    states: [
      State {
        name: "Off"
        PropertyChanges {
          target: sizeMediumButton
          iconColor: "white"
          bgcolor: Qt.hsla(Theme.darkGray.hslHue, Theme.darkGray.hslSaturation, Theme.darkGray.hslLightness, 0.3)
        }
      },

      State {
        name: "On"
        PropertyChanges {
          target: sizeMediumButton
          iconColor: Theme.mainColor
          bgcolor: Theme.darkGray
        }
      }
    ]

    onClicked: {
      eraseToolbar.size = sizeValue;
      adjustSize();
    }
  }

  QfToolButton {
    id: sizeLargeButton
    iconSource: Theme.getThemeVectorIcon( "ic_size_large_white_24dp" )
    round: true
    visible: true
    bgcolor: Theme.darkGray

    property int sizeValue: 10
    state: eraseToolbar.size == sizeValue ? "On" : "Off"

    states: [
      State {
        name: "Off"
        PropertyChanges {
          target: sizeLargeButton
          iconColor: "white"
          bgcolor: Qt.hsla(Theme.darkGray.hslHue, Theme.darkGray.hslSaturation, Theme.darkGray.hslLightness, 0.3)
        }
      },

      State {
        name: "On"
        PropertyChanges {
          target: sizeLargeButton
          iconColor: Theme.mainColor
          bgcolor: Theme.darkGray
        }
      }
    ]

    onClicked: {
      eraseToolbar.size = sizeValue;
      adjustSize();
    }
  }

  function adjustSize()
  {
    drawPolygonToolbar.rubberbandModel.measureValue = drawPolygonToolbar.mapSettings.mapUnitsPerPoint * 5 * eraseToolbar.size
  }

  function init(featureModel, mapSettings, editorRubberbandModel, editorRenderer)
  {
    eraseToolbar.featureModel = featureModel
    eraseToolbar.editorRenderer = editorRenderer
    eraseToolbar.editorRenderer.mapSettings = mapSettings
    drawPolygonToolbar.mapSettings = mapSettings
    drawPolygonToolbar.rubberbandModel = editorRubberbandModel
    drawPolygonToolbar.rubberbandModel.geometryType = Qgis.GeometryType.Line
    adjustSize()
    drawPolygonToolbar.stateVisible = true
  }

  function cancel()
  {
    drawPolygonToolbar.cancel()
    geometryEditorsRubberband.visible = true
  }
}
