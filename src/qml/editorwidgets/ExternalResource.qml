import QtQuick 2.5
import QtQuick.Controls 1.4
import org.qgis 1.0
import "../js/style.js" as Style
import ".." as QField

Item {
  signal valueChanged( var value )

  anchors.left: parent.left
  anchors.right: parent.right

  height: childrenRect.height

  property PictureSource __pictureSource

  Image {
    id: image
    width: parent.width
    autoTransform: true
    fillMode: Image.PreserveAspectFit
    source: 'file://' + project.homePath + '/' + value;

    QField.Button {
      width: 36 * dp
      height: 36 * dp

      anchors.right: parent.right
      anchors.bottom: parent.bottom

      bgcolor: enabled ? "#66BB6A" : "#888888"

      onClicked: __pictureSource = platformUtilities.getPicture( project.homePath + '/DCIM' )

      iconSource: Style.getThemeIcon( "ic_camera_white_36dp" )
    }
  }


  Connections {
    target: __pictureSource
    onPictureReceived: {
      console.info( "QML Picture received " + path )
      value = 'DCIM/' + path
      valueChanged( value )
    }
  }
}
