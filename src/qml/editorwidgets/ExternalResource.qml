import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Window 2.12

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import ".." as QField

Item {
  signal valueChanged(var value, bool isNull)

  anchors.left: parent.left
  anchors.right: parent.right

  height: Math.max(isImage? image.height : linkField.height, button_camera.height, button_gallery.height)

  property PictureSource __pictureSource
  property ViewStatus __viewStatus

  //on all mimetypes "image/..." and on empty values it should appear as an image widget except when it's configured as a link
  property bool isImage: {
    if ( value == null ) {
      return true
    } else if ( config.UseLink ) {
      return false
    } else if ( 
        FileUtils.mimeTypeName( qgisProject.homePath + '/' + value ).startsWith("image/") 
        || FileUtils.fileName( qgisProject.homePath + '/' + value ) === ''
    ) {
      return true
    } else {
      return false
    }
  }

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
    expressionText: {
      if ( currentLayer && currentLayer.customProperty('QFieldSync/photo_naming') !== undefined ) {
        return JSON.parse(currentLayer.customProperty('QFieldSync/photo_naming'))[field.name] 
      } else {
        return ''
      }
    }
  }

  function getPictureFilePath() {
    var evaluatedFilepath = expressionEvaluator.evaluate()
    
    if ( evaluatedFilepath && FileUtils.fileSuffix(evaluatedFilepath) !== '' ) {
      return evaluatedFilepath
    } else {
      return 'DCIM/JPEG_' + (new Date()).toISOString().replace(/[^0-9]/g, '') + '.jpg'
    }
  }

  Label {
    id: linkField
    height: fontMetrics.height + 20
    topPadding: 10
    bottomPadding: 10
    visible: !isImage
    anchors.left: parent.left
    anchors.right: parent.right
    font: Theme.defaultFont
    color: FileUtils.fileExists(qgisProject.homePath + '/' + value) ? Theme.hyperlinkBlue : 'gray'

    text: {
      var fieldValue = value

      if(UrlUtils.isRelativeOrFileUrl(fieldValue))
        fieldValue = config.FullUrl ? fieldValue : FileUtils.fileName(fieldValue)

      fieldValue = StringUtils.insertLinks(fieldValue)

      if ( !fieldValue )
      {
        font.italic = true
        return qsTr('No Value')
      }

      font.italic = false

      return fieldValue
    }

    background: Rectangle {
      y: linkField.height - height - linkField.bottomPadding / 2
      implicitWidth: 120
      height: 1
      color: "#C8E6C9"
    }

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if ( ! value )
          return

        // matches `http://...` but not `file://...` paths
        if ( ! UrlUtils.isRelativeOrFileUrl(value)) {
          Qt.openUrlExternally(value)
          return
        }

        // relative paths `./path/to/image.jpg` or 'path/to/image.jpg`
        if (FileUtils.fileExists(qgisProject.homePath + '/' + value) ) {
          __viewStatus = platformUtilities.open(qgisProject.homePath + '/' + value)
          return
        }
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
    width: 200
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
    anchors.margins: 4
    fillMode: Image.PreserveAspectFit
    width: 24
    height: 24
    source: hasGeoTag ? Theme.getThemeIcon("ic_geotag_24dp") : Theme.getThemeIcon("ic_geotag_missing_24dp")
    sourceSize.width: 24 * Screen.devicePixelRatio
    sourceSize.height: 24 * Screen.devicePixelRatio

  }

  DropShadow {
    anchors.fill: geoTagBadge
    visible: geoTagBadge.visible
    horizontalOffset: 0
    verticalOffset: 0
    radius: 6.0
    samples: 17
    color: "#DD000000"
    source: geoTagBadge
  }

  QfToolButton {
    id: button_camera
    width: 36
    height: 36

    anchors.right: button_gallery.left
    anchors.bottom: parent.bottom

    bgcolor: "transparent"
    visible: isImage && isEnabled

    onClicked: {
        if ( settings.valueBool("nativeCamera", true) ) {
            var filepath = getPictureFilePath()
            __pictureSource = platformUtilities.getCameraPicture(qgisProject.homePath+'/', filepath, FileUtils.fileSuffix(filepath) )
        } else {
            platformUtilities.createDir( qgisProject.homePath, 'DCIM' )
            camloader.active = true
        }
    }

    iconSource: Theme.getThemeIcon("ic_camera_alt_border_24dp")
  }

  QfToolButton {
    id: button_gallery
    width: 36
    height: 36

    anchors.right: parent.right
    anchors.bottom: parent.bottom

    bgcolor: "transparent"
    visible: isImage && isEnabled

    onClicked: {
          var filepath = getPictureFilePath()
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
            var filepath = getPictureFilePath()
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
