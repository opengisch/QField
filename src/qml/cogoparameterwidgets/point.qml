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
    columns: 3

    RowLayout {
      Layout.fillWidth: true
      Layout.columnSpan: 3
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
        text: {
          let content = (parameterLabel ? parameterLabel : qsTr("Point")) + ": ";
          if (mapSettings.destinationCrs.isGeographic) {
            content += qsTr("latitude") + ' ' + qsTr("longitude");
            if (!!parameterConfiguration["hasZ"]) {
              content += ' [' + qsTr("elevation") + ']';
            }
          } else {
            content += qsTr("easting (X)") + ' ' + qsTr("northing (Y)");
            if (!!parameterConfiguration["hasZ"]) {
              content += ' [' + qsTr("elevation (Z)") + ']';
            }
          }
          return content;
        }
      }
    }

    QfTextField {
      id: pointField
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
    const parts = pointField.text.split(" ").filter(Boolean);
    if (parts.length === 2 || parts.length === 3) {
      const isGeographic = destinationCrs.isGeographic;
      const x = parseFloat(parts[isGeographic ? 1 : 0]);
      const y = parseFloat(parts[isGeographic ? 0 : 1]);
      const z = parts.length === 3 ? parseFloat(parts[3]) : NaN;
      if (!isNaN(x) && !isNaN(y)) {
        valueChangeRequested(GeometryUtils.point(x, y, z));
        return;
      }
    }
    valueChangeRequested(undefined);
  }

  function requestedPositionReceived(position, positionInformation) {
    if (!isNaN(position.x)) {
      const isGeographic = destinationCrs.isGeographic;
      const precision = isGeographic ? 7 : 3;
      let content = isGeographic ? position.y.toFixed(precision) + ' ' + position.x.toFixed(precision) : position.x.toFixed(precision) + ' ' + position.y.toFixed(precision);
      if (!!parameterConfiguration["hasZ"]) {
        if (!isNaN(position.z)) {
          content += ' ' + position.z.toFixed(3);
        }
      }
      pointField.text = content;
      processValue();
    } else {
      pointField.text = '';
      processValue();
    }
    pointParameter.positionInformation = positionInformation;
    cogoOperationSettings.requestedPositionReceived(parameterName, position, positionInformation);
  }
}
