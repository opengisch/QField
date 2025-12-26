import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme
import org.qgis
import org.qfield

CogoParameterWidgetBase {
  id: pointParameter

  property var positionInformation: undefined

  width: parent.width
  height: childrenRect.height

  GridLayout {
    anchors {
      left: parent.left
      right: parent.right
    }
    columns: 4

    RowLayout {
      Rectangle {
        visible: parameterColor != "#00000000"
        width: 10
        height: 10
        radius: 5
        color: parameterColor
      }

      Label {
        Layout.fillWidth: true
        color: Theme.secondaryTextColor
        font: Theme.tipFont
        text: (parameterLabel ? parameterLabel + ": " : "") + qsTr("X")
      }
    }

    Label {
      Layout.fillWidth: true
      Layout.columnSpan: 3
      color: Theme.secondaryTextColor
      font: Theme.tipFont
      text: qsTr("Y")
    }

    QfTextField {
      id: xField
      Layout.fillWidth: true
      font: Theme.tipFont

      onTextEdited: {
        processValue();
      }
    }

    QfTextField {
      id: yField
      Layout.fillWidth: true
      font: Theme.tipFont

      onTextEdited: {
        processValue();
      }
    }

    QfToolButton {
      iconSource: Theme.getThemeVectorIcon("ic_coordinate_locator_white_24dp")
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"
      round: true

      onClicked: {
        requestPosition(pointParameter, true);
      }
    }

    QfToolButton {
      iconSource: Theme.getThemeVectorIcon("ic_location_valid_white_24dp")
      iconColor: Theme.positionColor
      bgcolor: "transparent"
      round: true

      onClicked: {
        requestPosition(pointParameter, false);
      }
    }
  }

  function processValue() {
    const x = parseFloat(xField.text);
    const y = parseFloat(yField.text);
    if (!isNaN(x) && !isNaN(y)) {
      valueChangeRequested(GeometryUtils.point(x, y));
    } else {
      valueChangeRequested(undefined);
    }
  }

  function requestedPositionReceived(position, positionInformation) {
    if (!isNaN(position.x)) {
      pointParameter.positionInformation = positionInformation;
      xField.text = position.x.toFixed(3);
      yField.text = position.y.toFixed(3);
      processValue();
    } else {
      pointParameter.positionInformation = undefined;
      xField.text = "";
      yField.text = "";
      processValue();
    }
    cogoOperationSettings.requestedPositionReceived(parameterName, position, positionInformation);
  }
}
