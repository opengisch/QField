import QtQuick 2.5
import QtQuick.Controls 1.4
import org.qgis 1.0
import "../js/style.js" as Style
import ".." as QField

Item {
  signal valueChanged(var value, bool isNull)

  anchors.left: parent.left
  anchors.right: parent.right

  height: Math.max(image.height, button.height)

  property PictureSource __pictureSource

  Image {
    property var currentValue: value

    id: image
    width: 200 * dp
    autoTransform: true
    fillMode: Image.PreserveAspectFit

    source: {
      if (image.status === Image.Error)
        Style.getThemeIcon("ic_broken_image_black_24dp")
      else if (currentValue)
        'file://' + qgisProject.homePath + '/' + currentValue
      else
        Style.getThemeIcon("ic_photo_notavailable_white_48dp")
    }

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if (currentValue)
          platformUtilities.open(image.source, "image/*");
      }
    }
  }

  QField.Button {
    id: button
    width: 36 * dp
    height: 36 * dp

    anchors.right: parent.right
    anchors.bottom: parent.bottom

    bgcolor: "transparent"

    onClicked: __pictureSource = platformUtilities.getPicture(qgisProject.homePath + '/DCIM')

    iconSource: Style.getThemeIcon("ic_camera_alt_border_24dp")
  }

  Connections {
    target: __pictureSource
    onPictureReceived: {
      valueChanged('DCIM/' + path, false)
    }
  }
}
