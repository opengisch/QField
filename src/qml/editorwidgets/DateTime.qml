import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme
import ".."

/*
  Config:
  * field_format
  * display_format
  * calendar_popup
  * allow_null

  If the calendar_popup is enabled, no direct editing is possible in the TextField.
  If not, it will try to match the display_format with the best possible InputMask.
  A Date/Time object (Date in QML) is used even with text field as source (not DateTime)
  to allow a full flexibility of field and display formats.

 */
EditorWidgetBase {
  id: main

  anchors {
    right: parent.right
    left: parent.left
  }

  height: childrenRect.height
  enabled: isEnabled

  property bool isDateTimeType: field.isDateOrTime
  property bool fieldIsDate: LayerUtils.fieldType(field) === 'QDate'
  property var currentValue: {
    const formattedDate = formatDateTime(value);
    label.text = formattedDate;
    return formattedDate;
  }

  function formatDateTime(value) {
    // Will handle both null and undefined as date values
    if (value == null || value === '') {
      return qsTr('(no date)');
    } else {
      const displayFormat = config['display_format'] == null ? 'yyyy-MM-dd' : config['display_format'];
      if (main.isDateTimeType) {
        // if the field is a QDate, the automatic conversion to JS date [1]
        // leads to the creation of date time object with the time zone.
        // For instance shapefiles has support for dates but not date/time or time.
        // So a date coming from a shapefile as 2001-01-01 will become 2000-12-31 19:00:00 -05 in QML/JS (in the carribeans).
        // And when formatting this with the display format, this is shown as 2000-12-31.
        // So we detect if the field is a date only and revert the time zone offset.
        // [1] http://doc.qt.io/qt-5/qtqml-cppintegration-data.html#basic-qt-data-types
        if (main.fieldIsDate) {
          const date = new Date(value);
          return Qt.formatDateTime(new Date(date.getTime() + date.getTimezoneOffset() * 60000), displayFormat);
        } else {
          return Qt.formatDateTime(value, displayFormat);
        }
      } else {
        const date = Date.fromLocaleString(Qt.locale(), value, config['field_format']);
        return Qt.formatDateTime(date, displayFormat);
      }
    }
  }

  Rectangle {
    visible: !enabled
    y: label.height - height
    implicitWidth: 120
    width: label.width
    height: 1
    color: Theme.accentLightColor
  }

  RowLayout {
    anchors.left: parent.left
    anchors.right: parent.right

    TextField {
      id: label

      Layout.fillWidth: true

      verticalAlignment: Text.AlignVCenter
      font: Theme.defaultFont
      color: value === undefined || !enabled ? Theme.mainTextDisabledColor : Theme.mainTextColor
      topPadding: 6
      bottomPadding: 6
      rightPadding: 0
      leftPadding: enabled ? 5 : 0

      inputMethodHints: Qt.ImhDigitsOnly

      // TODO[DR] generate input mask using regex
      inputMask: if (config['display_format'] === "yyyy-MM-dd") {
        "9999-99-99;_";
      } else if (config['display_format'] === "yyyy.MM.dd") {
        "9999.99.99;_";
      } else if (config['display_format'] === "yyyy-MM-dd HH:mm:ss") {
        "9999-99-99 99:99:99;_";
      } else if (config['display_format'] === "HH:mm:ss") {
        "99:99:99;_";
      } else if (config['display_format'] === "HH:mm") {
        "99:99;_";
      } else {
        "";
      }

      text: main.currentValue

      background: Rectangle {
        id: backgroundRect
        width: label.width
        height: label.height
        border.color: label.activeFocus ? Theme.accentColor : Theme.accentLightColor
        border.width: label.activeFocus ? 2 : 1
        color: enabled ? Theme.controlBackgroundAlternateColor : "transparent"
        radius: 2
        visible: enabled
      }

      MouseArea {
        enabled: config['calendar_popup'] === undefined || config['calendar_popup']
        anchors.fill: parent
        onClicked: {
          var usedDate = new Date();
          if (value !== undefined && value != '') {
            usedDate = value;
          }
          todayButton.forceActiveFocus();
          calendarPanel.selectedDate = usedDate;
          calendarPanel.open();
        }
      }

      onTextEdited: {
        var newDate = Date.fromLocaleString(Qt.locale(), label.text, config['display_format']);
        if (newDate.toLocaleString() !== "") {
          if (!main.isDateTimeType) {
            newDate = Qt.formatDateTime(newDate, config['field_format']);
          }
          valueChangeRequested(newDate, newDate === undefined);
        } else {
          valueChangeRequested(undefined, true);
        }
      }

      onActiveFocusChanged: {
        if (activeFocus) {
          // getting focus => placing cursor at proper position
          // TODO: make it work on empty value
          var mytext = label.text;
          var cur = label.cursorPosition;
          while (cur > 0) {
            if (!mytext.charAt(cur - 1).match("[0-9]"))
              break;
            cur--;
          }
          label.cursorPosition = cur;
        } else {
          // leaving field => if invalid, clear
          var newDate = Date.fromLocaleString(Qt.locale(), label.text, config['display_format']);
          if (newDate.toLocaleString() === "") {
            label.text = qsTr('(no date)');
          }
        }
      }

      QfToolButton {
        id: clearButton
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.verticalCenter: parent.verticalCenter
        width: 24
        height: 24
        padding: 0
        iconSource: Theme.getThemeVectorIcon("ic_clear_black_18dp")
        iconColor: Theme.mainTextColor
        visible: (value !== undefined) && enabled && (config['allow_null'] === undefined || config['allow_null'])

        onClicked: {
          valueChangeRequested(undefined, true);
        }
      }
    }

    QfToolButton {
      id: todayButton
      width: enabled ? 48 : 0
      visible: enabled

      iconSource: Theme.getThemeVectorIcon("ic_calendar_today_black_24dp")
      iconColor: Theme.mainTextColor

      onClicked: {
        if (main.isDateTimeType) {
          var currentDateTime = new Date();
          valueChangeRequested(currentDateTime, false);
        } else {
          var currentDate = new Date();
          var textDate = Qt.formatDateTime(currentDate, config['field_format']);
          valueChangeRequested(textDate, false);
        }
        displayToast(qsTr('Date value set to today.'));
      }
    }
  }

  FontMetrics {
    id: fontMetrics
    font: label.font
  }

  QfCalendarPanel {
    id: calendarPanel
    isDateTime: !main.fieldIsDate
    onDateTimePicked: {
      if (main.isDateTimeType) {
        valueChangeRequested(date, date === undefined);
      } else {
        var textDate = Qt.formatDateTime(date, config['field_format']);
        valueChangeRequested(date, date === undefined);
      }
    }
  }
}
