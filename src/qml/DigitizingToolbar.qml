import QtQuick 2.0
import org.qgis 1.0

Row {
  property VectorLayer currentLayer
  property bool isDigitizing //!< Readonly

  signal vertexAdded
  signal vertexRemoved
  signal cancel
  signal confirm

  Button {
    id: addVertexButton
    text: qsTr( "Add Vertex" )

    onClicked: {
      vertexAdded()
    }
  }

  Button {
    id: removeVertexButton
    text: qsTr( "Remove Vertex" )

    onClicked: {
      vertexRemoved()
    }
  }

  Button {
    id: cancelButton
    text: qsTr( "Cancel" )

    onClicked: {
      cancel()
    }
  }

  Button {
    id: confirmButton
    text: qsTr( "Confirm" )
    iconSource: "/themes/holodark/accept.png"

    onClicked: {
      confirm()
    }
  }
}
