import QtQuick 2.5
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import org.qgis 1.0
import Theme 1.0
import ".." as QField
import QtQuick.Window 2.2

Item {
  signal valueChanged(var value, bool isNull)

  anchors.left: parent.left
  anchors.right: parent.right

  height: Math.max(image.height, button_camera.height, button_gallery.height)

  property PictureSource __pictureSource

  Image {
    property var currentValue: value

    id: image
    width: 200 * dp
    autoTransform: true
    fillMode: Image.PreserveAspectFit
    horizontalAlignment: Image.AlignLeft

    //source is managed over onCurrentValueChanged since the binding would break somewhere
    source: Theme.getThemeIcon("ic_photo_notavailable_white_48dp")

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if (image.currentValue && settings.value("useNativeCamera", false))
          platformUtilities.open(image.currentValue, "image/*");
      }
    }

    onCurrentValueChanged: {
      if (image.status === Image.Error) {
        image.source=Theme.getThemeIcon("ic_broken_image_black_24dp")
      } else if (image.currentValue) {
        geoTagBadge.hasGeoTag = ExifTools.hasGeoTag(qgisProject.homePath + '/' + image.currentValue)
        image.source= 'file://' + qgisProject.homePath + '/' + image.currentValue
      } else {
        image.source=Theme.getThemeIcon("ic_photo_notavailable_white_48dp")
      }
    }
  }

  Image {
    property bool hasGeoTag: false
    id: geoTagBadge
    visible: true
    anchors.bottom: image.bottom
    anchors.right: image.right
    anchors.margins: 4 * dp
    source: hasGeoTag ? Theme.getThemeIcon("ic_geotag_24dp") : Theme.getThemeIcon("ic_geotag_missing_24dp")
  }

  DropShadow {
    anchors.fill: geoTagBadge
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
    visible: !readOnly

    onClicked: {
      if ( settings.valueBool("useNativeCamera", true) ) {
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
    visible: !readOnly

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
}
