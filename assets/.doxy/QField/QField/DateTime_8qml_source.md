

# File DateTime.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**DateTime.qml**](DateTime_8qml.md)

[Go to the documentation of this file](DateTime_8qml.md)


```C++
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
  property bool fieldIsDateTime: LayerUtils.fieldType(field) === 'QDateTime'
  property bool fieldIsDate: LayerUtils.fieldType(field) === 'QDate'
  property bool fieldIsTime: LayerUtils.fieldType(field) === 'QTime'
  property bool fieldIsString: LayerUtils.fieldType(field) === 'QString'

  property var currentValue: {
    const formattedDate = formatDateTime(value);
    label.text = formattedDate;
    return formattedDate;
  }

  function formatDateTime(value) {
    // Will handle both null and undefined as date values
    if (FeatureUtils.attributeIsNull(value) || value === '') {
      return qsTr('(no date)');
    } else {
      let displayFormat = config['display_format'];
      if (displayFormat === undefined || displayFormat === "") {
        if (main.fieldIsDateTime) {
          displayFormat = "yyyy-MM-dd HH:mm:ss";
        } else if (main.fieldIsTime) {
          displayFormat = "HH:mm:ss";
        } else {
          displayFormat = "yyyy-MM-dd";
        }
      }
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
        let dateFormat = config['display_format'] !== undefined ? config['display_format'] : '';
        if (!!config['field_format_overwrite']) {
          dateFormat = !!config['field_iso_format'] ? 'yyyy-MM-dd HH:mm:ss+t' : config['field_format'];
        }
        const date = Date.fromLocaleString(Qt.locale(), value, dateFormat);
        if (date.toString() === "Invalid Date") {
          try {
            return Qt.formatDateTime(value, displayFormat);
          } catch (e) {
            return qsTr('(no date)');
          }
        }
        return Qt.formatDateTime(date, displayFormat);
      }
    }
  }

  RowLayout {
    anchors.left: parent.left
    anchors.right: parent.right

    TextField {
      id: label

      Layout.fillWidth: true

      verticalAlignment: Text.AlignVCenter
      font: Theme.defaultFont
      color: (!isEditable && isEditing) ? Theme.mainTextDisabledColor : Theme.mainTextColor
      topPadding: 6
      bottomPadding: 6
      rightPadding: 0
      leftPadding: isEnabled || (!isEditable && isEditing) ? 10 : 0

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

      text: main.currentValue !== undefined ? main.currentValue : ''

      MouseArea {
        enabled: config['calendar_popup'] === undefined || config['calendar_popup']
        anchors.fill: parent
        onClicked: {
          let usedDate = new Date();
          if (value !== undefined && value != '') {
            usedDate = value;
          }
          if (!(usedDate instanceof Date)) {
            usedDate = Date.fromLocaleString(Qt.locale(), label.text, !!config['field_iso_format'] ? 'yyyy-MM-dd HH:mm:ss+t' : config['display_format']);
          }
          todayButton.forceActiveFocus();
          calendarPanel.selectedDate = usedDate;
          calendarPanel.open();
        }
      }

      onTextEdited: {
        let newDate = Date.fromLocaleString(Qt.locale(), label.text, !!config['field_iso_format'] ? 'yyyy-MM-dd HH:mm:ss+t' : config['display_format']);
        if (newDate.toLocaleString() !== "") {
          if (!main.isDateTimeType) {
            newDate = Qt.formatDateTime(newDate, !!config['field_iso_format'] ? Qt.ISODate : config['field_format']);
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
          const mytext = label.text;
          let cur = label.cursorPosition;
          while (cur > 0) {
            if (!mytext.charAt(cur - 1).match("[0-9]"))
              break;
            cur--;
          }
          label.cursorPosition = cur;
        } else {
          // leaving field => if invalid, clear
          let newDate = Date.fromLocaleString(Qt.locale(), label.text, !!config['field_iso_format'] ? 'yyyy-MM-dd HH:mm:ss+t' : config['display_format']);
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

      background.visible: isEnabled || (!isEditable && isEditing)
    }

    QfToolButton {
      id: todayButton
      width: enabled ? 48 : 0
      visible: enabled

      iconSource: Theme.getThemeVectorIcon("ic_calendar_today_black_24dp")
      iconColor: Theme.mainTextColor

      onClicked: {
        const currentDate = new Date();
        if (main.isDateTimeType) {
          valueChangeRequested(currentDate, false);
        } else {
          let dateFormat = config['display_format'] !== undefined ? config['display_format'] : '';
          if (!!config['field_format_overwrite']) {
            dateFormat = !!config['field_iso_format'] ? 'yyyy-MM-dd HH:mm:ss+t' : config['field_format'];
          }
          const textDate = Qt.formatDateTime(currentDate, dateFormat);
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
    showTimePicker: main.fieldIsDateTime || main.fieldIsTime || (main.fieldIsString && config['field_format_overwrite'] && (config['field_format'].includes("HH:mm") || !!config['field_iso_format'])) || (main.fieldIsString && !config['field_format_overwrite'])
    showDatePicker: main.fieldIsDate || main.fieldIsDateTime || (main.fieldIsString && config['field_format_overwrite'] && (config['field_format'].includes("yyyy-MM") || config['field_format'].includes("yyyy.MM") || !!config['field_iso_format'])) || (main.fieldIsString && !config['field_format_overwrite'])

    onDateTimePicked: date => {
      if (main.isDateTimeType) {
        valueChangeRequested(date, date === undefined);
      } else {
        let dateFormat = config['display_format'] !== undefined ? config['display_format'] : '';
        if (!!config['field_format_overwrite']) {
          dateFormat = !!config['field_iso_format'] ? 'yyyy-MM-dd HH:mm:ss+t' : config['field_format'];
        }
        const textDate = Qt.formatDateTime(date, dateFormat);
        valueChangeRequested(textDate, date === undefined);
      }
    }
  }
}
```


