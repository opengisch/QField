import QtQuick 2.6
import org.qgis 1.0
import org.qfield 1.0
import "js/style.js" as Style

Row {
  id: geometryEditingToolbar

  property VertexModel vertexModel

  spacing: 4 * dp
  padding: 4 * dp

  signal vertexAdded
  signal vertexRemoved
  signal cancel
  signal confirm

  Button {
    id: cancelButton
    iconSource: Style.getThemeIcon( "ic_clear_white_24dp" )
    round: true
    bgcolor: "#616161"

    onClicked: {
      cancel()
    }
  }

  Button {
    id: previousVertexButton
    iconSource: Style.getThemeIcon( "ic_chevron_left_white_24dp" )
    round: true
    bgcolor: "#616161"

    onClicked: {
      vertexModel.previousVertex()
    }
  }

  Button {
    id: removeVertexButton
    iconSource: Style.getThemeIcon( "ic_remove_white_24dp" )
    round: true
    bgcolor: "#616161"

    onClicked: {
      vertexRemoved()
    }
  }

  Button {
    id: nextVertexButton
    iconSource: Style.getThemeIcon( "ic_chevron_right_white_24dp" )
    round: true
    bgcolor: "#616161"

    onClicked: {
      vertexModel.nextVertex()
    }
  }
}
