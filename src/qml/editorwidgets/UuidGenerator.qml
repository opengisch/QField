import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

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
    color: Theme.mainTextDisabledColor
    text: {
      var displayValue = value !== undefined ? value : '';
      if (isLoaded && isAdding && (value == undefined || value === '')) {
        displayValue = StringUtils.createUuid();
        valueChangeRequested(displayValue, false);
      }
      return displayValue;
    }
    elide: Text.ElideMiddle
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
