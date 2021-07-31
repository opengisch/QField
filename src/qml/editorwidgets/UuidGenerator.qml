import QtQuick 2.12
import QtQuick.Controls 2.12

import Theme 1.0
import org.qfield 1.0

import "."

EditorWidgetBase {
  height: childrenRect.height

  onProcessValue: {
      if (isEnabled && (value === undefined || value == '')) {
          valueChangeRequested(StringUtils.createUuid() ,false);
      }
  }

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
      text: value !== undefined ? value : ''
  }

  Rectangle {
      id: backgroundRect
      anchors.left: parent.left
      anchors.right: parent.right
      y: uuidLabel.height - height - uuidLabel.bottomPadding / 2
      implicitWidth: 120
      height: 1
      color: "#C8E6C9"
  }

  FontMetrics {
    id: fontMetrics
    font: uuidLabel.font
  }
}
