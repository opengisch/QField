import QtQuick
import QtQuick.Controls
import org.qfield.core
import org.qfield.gui

QfEditorWidgetBase {
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
    font: QfTheme.defaultFont
    color: QfTheme.mainTextDisabledColor
    text: {
      var displayValue = value !== undefined ? value : '';
      if (isLoaded && isAdding && (value == undefined || value === '')) {
        displayValue = QfStringUtils.createUuid();
        valueChangeRequested(displayValue, false);
      }
      return displayValue;
    }
    elide: Text.ElideMiddle
  }

  FontMetrics {
    id: fontMetrics
    font: uuidLabel.font
  }
}
