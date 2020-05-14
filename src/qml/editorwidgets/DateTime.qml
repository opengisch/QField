import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

import Theme 1.0


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

Item {
  signal valueChanged(var value, bool isNull)

  height: childrenRect.height

  ColumnLayout {
    id: main
    property bool isDateTimeType: field.isDateOrTime
    property bool fieldIsDate: platformUtilities.fieldType( field ) === 'QDate'
    property var currentValue: isDateTimeType ? value : Qt.formatDateTime(value, config['field_format'])

    anchors { right: parent.right; left: parent.left }

    Item {
      Layout.fillWidth: true
      Layout.minimumHeight: 48

      Rectangle {
        id: backgroundRect
        anchors.top: label.top
        anchors.left: label.left
        width: label.width
        height: label.height - label.bottomPadding / 2
        border.color: label.activeFocus ? "#17a81a" : "#21be2b"
        border.width: label.activeFocus ? 2 : 1
        color: enabled ? Theme.lightGray : "transparent"
        radius: 2
        visible: enabled
      }

      TextField {
        id: label

        anchors.left: parent.left
        anchors.right: parent.right
        verticalAlignment: Text.AlignVCenter
        font: Theme.defaultFont
        height: fontMetrics.height + 20
        topPadding: 10
        bottomPadding: 10

        inputMethodHints: Qt.ImhDigitsOnly

        // TODO[DR] generate input mask using regex
        inputMask:      if (config['display_format'] === "yyyy-MM-dd" ) { "9999-99-99;_" }
                   else if (config['display_format'] === "yyyy.MM.dd" ) { "9999.99.99;_" }
                   else if (config['display_format'] === "yyyy-MM-dd HH:mm:ss" ) { "9999-99-99 99:99:99;_" }
                   else if (config['display_format'] === "HH:mm:ss" ) { "99:99:99;_" }
                   else if (config['display_format'] === "HH:mm" ) { "99:99;_" }
                   else { "" }

        text: if ( value === undefined )
              {
                qsTr('(no date)')
              }
              else
              {
                if ( main.isDateTimeType )
                {
                  // if the field is a QDate, the automatic conversion to JS date [1]
                  // leads to the creation of date time object with the time zone.
                  // For instance shapefiles has support for dates but not date/time or time.
                  // So a date coming from a shapefile as 2001-01-01 will become 2000-12-31 19:00:00 -05 in QML/JS (in the carribeans).
                  // And when formatting this with the display format, this is shown as 2000-12-31.
                  // So we detect if the field is a date only and revert the time zone offset.
                  // [1] http://doc.qt.io/qt-5/qtqml-cppintegration-data.html#basic-qt-data-types
                  if (main.fieldIsDate) {
                    Qt.formatDateTime( new Date(value.getTime() + value.getTimezoneOffset() * 60000), config['display_format'])
                  } else {
                    Qt.formatDateTime(value, config['display_format'])
                  }
                }
                else
                {
                  var date = Date.fromLocaleString(Qt.locale(), value, config['field_format'])
                  Qt.formatDateTime(date, config['display_format'])
                }
              }

        color: value === undefined || !enabled ? 'gray' : 'black'

        background: Rectangle {
          visible: !enabled
          y: label.height - height - label.bottomPadding / 2
          implicitWidth: 120
          height: label.activeFocus ? 2: 1
          color: label.activeFocus ? "#4CAF50" : "#C8E6C9"
        }

        MouseArea {
          enabled: config['calendar_popup']
          anchors.fill: parent
          onClicked: {
            var usedDate = new Date();
            if (value !== undefined && value != '') {
                usedDate = value;
            }

            calendar.selectedDate = usedDate
            calendar.year = usedDate.getFullYear()
            calendar.month = usedDate.getMonth()

            popup.open()
          }
        }

        onTextEdited: {
          var newDate = Date.fromLocaleString(Qt.locale(), label.text, config['display_format'])
          if ( newDate.toLocaleString() !== "" )
          {
            if ( !main.isDateTimeType )
            {
              newDate = Qt.formatDateTime(newDate, config['field_format'])
            }
            valueChanged(newDate, newDate === undefined)
          }
          else
          {
            valueChanged(undefined, true)
          }
        }

        onActiveFocusChanged: {
          if (activeFocus) {
            // getting focus => placing cursor at proper position
            // TODO: make it work on empty value
            var mytext = label.text
            var cur = label.cursorPosition
            while ( cur > 0 )
            {
              if (!mytext.charAt(cur-1).match("[0-9]") )
                break
              cur--
            }
            label.cursorPosition = cur
          } else {
            // leaving field => if invalid, clear
            var newDate = Date.fromLocaleString(Qt.locale(), label.text, config['display_format'])
            if ( newDate.toLocaleString() === "" )
            {
              label.text = qsTr('(no date)')
            }
          }
        }

        Image {
          id: todayButton
          source: Theme.getThemeIcon("ic_calendar_today_black_18dp")
          anchors.right: parent.right
          anchors.rightMargin: 4
          anchors.verticalCenter: parent.verticalCenter
          anchors.verticalCenterOffset: -2
          visible: enabled

          MouseArea {
            anchors.fill: parent
            onClicked: {
              if ( main.isDateTimeType )
              {
                var currentDateTime = new Date()
                valueChanged(currentDateTime, false)
              }
              else
              {
                var currentDate = new Date()
                var textDate = Qt.formatDateTime(currentDate, config['field_format'])
                valueChanged(textDate, false)
              }
              displayToast(qsTr( 'Date value set to today.'))
            }
          }
        }

        Image {
          id: clearButton
          source: Theme.getThemeIcon("ic_clear_black_18dp")
          anchors.right: todayButton.left
          anchors.rightMargin: 4
          anchors.verticalCenter: parent.verticalCenter
          anchors.verticalCenterOffset: -2
          visible: ( value !== undefined ) && config['allow_null'] && enabled

          MouseArea {
            anchors.fill: parent
            onClicked: {
              valueChanged(undefined, true)
            }
          }
        }
      }
    }

    Popup {
      id: popup
      modal: true
      focus: true
      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
      parent: ApplicationWindow.overlay
      x: (parent.width - width) / 2
      y: (parent.height - height) / 2


      // TODO: fixme no signal when date is clicked on current
      ColumnLayout {
          // Calendar overlay for selecting the date ranges
          Rectangle {
              id: calendarOverlay
              width: 350
              height: 240
              color: "transparent"

              MouseArea {
                  anchors.fill: parent
                  onClicked: mouse.accepted = true
                  onWheel: wheel.accepted = true
              }

              GridLayout {
                  id: calendarGrid
                  anchors.centerIn: parent
                  columns: 3
                  Button {
                      text: "<"
                      onClicked: {
                          calendar.month -= 1;
                      }
                      enabled: true
                      Layout.column: 0
                      Layout.row: 0
                  }

                  Text {
                      text: calendar.title
                      horizontalAlignment: Text.AlignHCenter
                      Layout.column: 1
                      Layout.row: 0
                      Layout.fillWidth: true
                  }

                  Button {
                      text: ">"
                      onClicked: {
                          calendar.month += 1;
                      }
                      enabled: true
                      Layout.column: 2
                      Layout.row: 0
                  }

                  DayOfWeekRow {
                      locale: calendar.locale

                      Layout.column: 1
                      Layout.row: 1
                      Layout.fillWidth: true
                  }

                  MonthGrid {
                      id: calendar
                      month: Calendar.January
                      year: 2020
                      Layout.row: 2
                      Layout.column: 1
                      Layout.fillWidth: true
                      Layout.fillHeight: true

                      property date selectedDate: new Date()

                      delegate: Text {
                          horizontalAlignment: Text.AlignHCenter
                          verticalAlignment: Text.AlignVCenter
                          opacity: model.month !== calendar.month ? 0 : 1
                          text: model.day
                          font.bold: calendar.selectedDate.getDate() === model.date.getDate()
                      }

                      onClicked: {
                          selectedDate = date;
                      }

                      function resetDate() {
                        selectedDate = main.currentValue ? main.isDateTimeType ? main.currentValue : Date.fromLocaleString(Qt.locale(), main.currentValue, config['field_format']) : new Date()
                      }
                  }
              }
          }

          RowLayout {
              QfButton {
                  text: qsTr( "OK" )
                  font: Theme.tipFont
                  Layout.fillWidth: true

                  onClicked: {
                      var newDate = calendar.selectedDate

                      if ( main.isDateTimeType )
                      {
                          valueChanged(newDate, newDate === undefined)
                      }
                      else
                      {
                          var textDate = Qt.formatDateTime(newDate, config['field_format'])
                          valueChanged(textDate, textDate === undefined)
                      }

                      popup.close()
                  }
              }
          }
      }
    }

    onIsDateTimeTypeChanged: {
        calendar.resetDate()
    }

    onCurrentValueChanged: {
        calendar.resetDate()
    }
  }

  FontMetrics {
      id: fontMetrics
      font: label.font
  }
}
