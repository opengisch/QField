import QtQuick 2.14
import QtQuick.Controls 2.14

import Theme 1.0
import org.qfield 1.0

import "."

EditorWidgetBase {
  height: childrenRect.height

  anchors {
    right: parent.right
    left: parent.left
  }

  Label {
      id: uuidLabel
      height: fontMetrics.height + 20
      anchors {
          left: parent.left
          right: parent.right
      }

      topPadding: 10
      bottomPadding: 10
      font: Theme.defaultFont
      color: 'gray'
      text: {
        var displayValue = value !== undefined ? value : ''
        if (isLoaded && isEnabled && (value === undefined || value == '')) {
            displayValue = StringUtils.createUuid();
            valueChangeRequested(displayValue ,false);
        }
        return displayValue;
      }
  }

  Rectangle {
      id: backgroundRect
      anchors.left: parent.left
      anchors.right: parent.right
      y: uuidLabel.height - height - uuidLabel.bottomPadding / 2
      implicitWidth: 120
      height: 1
      color: Theme.accentLightColor
  }

  FontMetrics {
    id: fontMetrics
    font: uuidLabel.font
  }
}
