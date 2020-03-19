import QtQuick 2.5
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import ".." as QField
import QtQuick.Window 2.2

import org.qfield 1.0

Item {
  signal valueChanged(var value, bool isNull)

  anchors.left: parent.left
  anchors.right: parent.right

  height: Math.max(isImage? image.height : linkField.height, button_camera.height, button_gallery.height)

  property PictureSource __pictureSource
  property ViewStatus __viewStatus

  //on all mimetypes image/... and on empty values it should appear as an image widget
  property bool isImage: value === undefined || FileUtils.mimeTypeName( qgisProject.homePath + '/' + value ).startsWith("image/") || FileUtils.fileName( qgisProject.homePath + '/' + value ) === ''

  //to not break any binding of image.source
  property var currentValue: value
  onCurrentValueChanged: {
      if ( isImage ) {
          if ( value === undefined || FileUtils.fileName( qgisProject.homePath + '/' + value ) === '' ) {
              image.source=Theme.getThemeIcon("ic_photo_notavailable_black_24dp")
              geoTagBadge.visible = false
          } else if ( image.status === Image.Error || !FileUtils.fileExists( qgisProject.homePath + '/' + value ) ) {
              image.source=Theme.getThemeIcon("ic_broken_image_black_24dp")
              geoTagBadge.visible = false
          } else {
              geoTagBadge.hasGeoTag = ExifTools.hasGeoTag(qgisProject.homePath + '/' + value)
              image.source= 'file://' + qgisProject.homePath + '/' + value
              geoTagBadge.visible = true
          }
      } else {
          geoTagBadge.visible = false
      }
  }

  ExpressionEvaluator {
    id: expressionEvaluator
    feature: currentFeature
    layer: currentLayer
    expressionText: currentLayer ? currentLayer.customProperty('QFieldSync/photo_naming')!==undefined ? JSON.parse(currentLayer.customProperty('QFieldSync/photo_naming'))[field.name] : '' : ''
  }

  Label {
    id: linkField
    height: fontMetrics.height + 20 * dp
    topPadding: 10 * dp
    bottomPadding: 10 * dp
    visible: !isImage
    enabled: !isImage
    anchors.left: parent.left
    anchors.right: parent.right
    font: Theme.defaultFont
    color: FileUtils.fileExists(qgisProject.homePath + '/' + value) ? '#0000EE' : 'black'

    text: FileUtils.fileName( qgisProject.homePath + '/' + value )

    background: Rectangle {
      y: linkField.height - height - linkField.bottomPadding / 2
      implicitWidth: 120 * dp
      height: 1 * dp
      color: "#C8E6C9"
    }

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if (value && FileUtils.fileExists(qgisProject.homePath + '/' + value) )
          __viewStatus = platformUtilities.open( qgisProject.homePath + '/' + value );
      }
    }

  }

  FontMetrics {
    id: fontMetrics
    font: linkField.font
  }

  Image {
    id: image
    visible: isImage
    enabled: isImage
    width: 200 * dp
    autoTransform: true
    fillMode: Image.PreserveAspectFit
    horizontalAlignment: Image.AlignLeft

    source: Theme.getThemeIcon("ic_photo_notavailable_black_24dp")

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if ( FileUtils.fileExists( qgisProject.homePath + '/' + value ) )
          platformUtilities.open( qgisProject.homePath + '/' + value );
      }
    }
  }

  Image {
    property bool hasGeoTag: false
    id: geoTagBadge
    visible: false
    anchors.bottom: image.bottom
    anchors.right: image.right
    anchors.margins: 4 * dp
    source: hasGeoTag ? Theme.getThemeIcon("ic_geotag_24dp") : Theme.getThemeIcon("ic_geotag_missing_24dp")
  }

  DropShadow {
    anchors.fill: geoTagBadge
    visible: geoTagBadge.visible
    horizontalOffset: 0
    verticalOffset: 0
    radius: 6.0 * dp
    samples: 17
    color: "#DD000000"
    source: geoTagBadge
  }

  QField.Button {
    id: button_camera
    width: 36 * dp
    height: 36 * dp

    anchors.right: button_gallery.left
    anchors.bottom: parent.bottom

    bgcolor: "transparent"
    visible: !readOnly && isImage

    onClicked: {
        if ( settings.valueBool("nativeCamera", true) ) {
            var evaluated_filepath = expressionEvaluator.evaluate()
            var filepath = !evaluated_filepath || FileUtils.fileSuffix(evaluated_filepath) === '' ? 'DCIM/JPEG_'+(new Date()).toISOString().replace(/[^0-9]/g, "")+'.jpg' : evaluated_filepath
            __pictureSource = platformUtilities.getCameraPicture(qgisProject.homePath+'/', filepath, FileUtils.fileSuffix(filepath) )
        } else {
            platformUtilities.createDir( qgisProject.homePath, 'DCIM' )
            camloader.active = true
        }
    }

    iconSource: Theme.getThemeIcon("ic_camera_alt_border_24dp")
  }

  QField.Button {
    id: button_gallery
    width: 36 * dp
    height: 36 * dp

    anchors.right: parent.right
    anchors.bottom: parent.bottom

    bgcolor: "transparent"
    visible: !readOnly && isImage

    onClicked: {
        var evaluated_filepath = expressionEvaluator.evaluate()
        var filepath = !evaluated_filepath || FileUtils.fileSuffix(evaluated_filepath) === '' ? 'DCIM/JPEG_'+(new Date()).toISOString().replace(/[^0-9]/g, "")+'.jpg' : evaluated_filepath
        __pictureSource = platformUtilities.getGalleryPicture(qgisProject.homePath+'/', filepath)
    }

    iconSource: Theme.getThemeIcon("baseline_photo_library_black_24")
  }


  Loader {
    id: camloader
    sourceComponent: camcomponent
    active: false
  }

  Component {
    id: camcomponent

    Popup {
      id: campopup

      Component.onCompleted: {
        if ( platformUtilities.checkCameraPermissions() )
          open()
      }

      parent: ApplicationWindow.overlay

      x: 0
      y: 0
      height: parent.height
      width: parent.width

      modal: true
      focus: true

      QField.QFieldCamera {
        id: qfieldCamera

        visible: true

        onFinished: {
            var evaluated_filepath = expressionEvaluator.evaluate()
            var filepath = !evaluated_filepath || FileUtils.fileSuffix(evaluated_filepath) === '' ? 'DCIM/JPEG_'+(new Date()).toISOString().replace(/[^0-9]/g, "")+'.jpg' : evaluated_filepath
            platformUtilities.renameFile( path, qgisProject.homePath +'/' + filepath)
            valueChanged(filepath, false)
            campopup.close()
        }
        onCanceled: {
          campopup.close()
        }
      }
      onClosed: camloader.active = false
    }
  }

  Connections {
    target: __pictureSource
    onPictureReceived: {
      if( path )
      {
          valueChanged(path, false)
      }
    }
  }

  Connections {
    target: __viewStatus
    onStatusReceived: {
      if( status )
      {
        //default message (we would have the passed error message still)
        displayToast( qsTr("Cannot handle this file type"))
      }
    }
  }

}
