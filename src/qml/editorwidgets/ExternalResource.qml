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

  Column {
    height: childrenRect.height
    anchors.left: parent.left
    anchors.right: parent.right

    Image {
      id: image
      width: parent.width
      autoTransform: true
      fillMode: Image.PreserveAspectFit
      source: 'file://' + project.homePath + '/' + value;
    }

    QField.Button {
      width: 36 * dp
      height: 36 * dp
      bgcolor: "#66BB6A"

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
