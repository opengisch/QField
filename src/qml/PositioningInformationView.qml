import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

Rectangle {
  id: positioningInformationView

  property Positioning positionSource
  property color backgroundColor: "transparent"
  property color alternateBackgroundColor: Theme.positionBackgroundColor
  property color textColor: positionSource.currentness ? Theme.mainTextColor : Theme.secondaryTextColor
  property double cellHeight: 26
  property double cellPadding: 6

  anchors.margins: 20
  width: parent.width
  color: Theme.mainBackgroundColorSemiOpaque

  GridView {
    id: grid
    anchors.fill: parent
    cellHeight: positioningInformationView.cellHeight
    cellWidth: {
      if (parent.width >= 1000)
        return parent.width / 4;
      else if (parent.width >= 620)
        return parent.width / 3;
      else if (parent.width >= 200)
        return parent.width / 2;
      return parent.width;
    }
    clip: true
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.vertical: QfScrollBar {
    }
    model: PositioningModel {
      id: positioningModel
      positioningSource: positioningInformationView.positionSource
    }

    Component.onCompleted: {
      positioningModel.setupConnections();
    }

    property real numberOfColumns: parent.width / cellWidth

    delegate: Rectangle {
      width: grid.cellWidth
      height: grid.cellHeight

      property real currentRow: parseInt(index / grid.numberOfColumns)

      color: {
        if (grid.numberOfColumns % 2 == 0) {
          if (currentRow % 2 == 0) {
            return index % 2 == 0 ? positioningInformationView.alternateBackgroundColor : positioningInformationView.backgroundColor;
          } else {
            return index % 2 == 0 ? positioningInformationView.backgroundColor : positioningInformationView.alternateBackgroundColor;
          }
        } else {
          return index % 2 == 0 ? positioningInformationView.alternateBackgroundColor : positioningInformationView.backgroundColor;
        }
      }

      RowLayout {
        anchors.margins: cellPadding
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: VariableName
        }

        Text {
          Layout.fillWidth: true
          font: Theme.tipFont
          color: positioningInformationView.textColor
          text: VariableValue ? VariableValue : qsTr("N/A")
          verticalAlignment: Text.AlignVCenter
          elide: Text.ElideRight
        }
      }
    }
  }
}
