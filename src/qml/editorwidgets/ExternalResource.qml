import QtQuick 2.5
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import ".." as QField
import QtQuick.Window 2.2

Item {
  signal valueChanged(var value, bool isNull)

  anchors.left: parent.left
  anchors.right: parent.right

  height: Math.max(image.height, button_camera.height, button_gallery.height)

  property PictureSource __pictureSource
  property ViewStatus __viewStatus

  //on all mimetypes image/... and on empty values it should appear as an image widget
  property bool isImage: FileUtils.mimeTypeName( qgisProject.homePath + '/' + value ).startsWith("image/") || FileUtils.fileName( qgisProject.homePath + '/' + value ) === ''

  Label {
    id: linkField
    height: fontMetrics.height + 20 * dp
    topPadding: 10 * dp
    bottomPadding: 10 * dp
    visible: !isImage
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
    width: 200 * dp
    autoTransform: true
    fillMode: Image.PreserveAspectFit
    horizontalAlignment: Image.AlignLeft

    source: if ( FileUtils.fileName( qgisProject.homePath + '/' + value ) !== '' ) {
                if( FileUtils.fileExists(qgisProject.homePath + '/' + value) && image.status !== Image.Error ) {
                    'file://' + qgisProject.homePath + '/' + value
                } else {
                  Theme.getThemeIcon("ic_broken_image_black_24dp")
                }
            } else {
                Theme.getThemeIcon("ic_photo_notavailable_white_48dp")
            }

    onSourceChanged: {
        geoTagBadge.hasGeoTag = ExifTools.hasGeoTag(qgisProject.homePath + '/' + value)
    }

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if (value)
          platformUtilities.open( qgisProject.homePath + '/' + value );
      }
    }
  }

  Image {
    property bool hasGeoTag: false
    id: geoTagBadge
    visible: isImage
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
        __pictureSource = platformUtilities.getCameraPicture(qgisProject.homePath + '/DCIM')
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
        __pictureSource = platformUtilities.getGalleryPicture(qgisProject.homePath + '/DCIM')
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
          var timestamp = (new Date()).toISOString().replace(/[^0-9]/g, "")
          var filename = timestamp+'.jpg';
          platformUtilities.renameFile( path, qgisProject.homePath +'/DCIM/' + filename)
          valueChanged('DCIM/' + filename, false)
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
          valueChanged('DCIM/' + path, false)
      }
    }
  }

  Connections {
    target: __viewStatus
    onStatusReceived: {
      if( status )
      {
        //default message (we would have the passed the error message still)
        displayToast( qsTr("Cannot handle this file type"))
      }
    }
  }

}
