import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0
import Theme 1.0
import QFieldControls 1.0

import "."
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
    property bool fieldIsDate: LayerUtils.fieldType( field ) === 'QDate'
    property var currentValue: isDateTimeType ? value : Qt.formatDateTime(value, config['field_format'])


    Rectangle {
        visible: !enabled
        y: label.height - height - label.bottomPadding / 2
        implicitWidth: 120
        width: label.width
        height: 1
        color: Theme.accentLightColor
    }

    TextField {
        id: label

        anchors.left: parent.left
        anchors.right: parent.right
        verticalAlignment: Text.AlignVCenter
        font: Theme.defaultFont
        leftPadding: enabled ? 5 : 0

        inputMethodHints: Qt.ImhDigitsOnly

        // TODO[DR] generate input mask using regex
        inputMask:      if (config['display_format'] === "yyyy-MM-dd" ) { "9999-99-99;_" }
                        else if (config['display_format'] === "yyyy.MM.dd" ) { "9999.99.99;_" }
                        else if (config['display_format'] === "yyyy-MM-dd HH:mm:ss" ) { "9999-99-99 99:99:99;_" }
                        else if (config['display_format'] === "HH:mm:ss" ) { "99:99:99;_" }
                        else if (config['display_format'] === "HH:mm" ) { "99:99;_" }
                        else { "" }

        text: if ( value === undefined || value == '' )
              {
                  qsTr('(no date)')
              }
              else
              {
                  var displayFormat = config['display_format'] == null
                          ? 'yyyy-MM-dd'
                          : config['display_format']

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
                          const date = new Date(value)
                          Qt.formatDateTime(new Date(date.getTime() + date.getTimezoneOffset() * 60000), displayFormat)
                      } else {
                          Qt.formatDateTime(value, displayFormat)
                      }
                  }
                  else
                  {
                      const date = Date.fromLocaleString(Qt.locale(), value, config['field_format'])
                      Qt.formatDateTime(date, displayFormat)
                  }
              }

        color: value === undefined || !enabled ? 'gray' : 'black'

        background: Item {
            implicitWidth: 120
            height: label.height

            Rectangle {
                id: backgroundRect
                anchors.fill: parent
                anchors.topMargin: label.topPadding / 2
                anchors.bottomMargin: label.bottomPadding / 2
                border.color: label.activeFocus ? Theme.accentColor : Theme.accentLightColor
                border.width: label.activeFocus ? 2 : 1
                color: enabled ? Theme.lightGray : "transparent"
                radius: 2
                visible: enabled
            }
        }

        MouseArea {
            enabled: config['calendar_popup'] === undefined || config['calendar_popup']
            anchors.fill: parent
            onClicked: {
                var usedDate = new Date();
                if (value !== undefined && value != '') {
                    usedDate = value;
                }
                calendarPanel.selectedDate = usedDate
                calendarPanel.open();
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
                valueChangeRequested(newDate, newDate === undefined)
            }
            else
            {
                valueChangeRequested(undefined, true)
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
            width: 20
            height: 20
            source: Theme.getThemeIcon("ic_calendar_today_black_18dp")
            sourceSize.width: 20 * screen.devicePixelRatio
            sourceSize.height: 20 * screen.devicePixelRatio
            fillMode: Image.PreserveAspectFit
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
                        valueChangeRequested(currentDateTime, false)
                    }
                    else
                    {
                        var currentDate = new Date()
                        var textDate = Qt.formatDateTime(currentDate, config['field_format'])
                        valueChangeRequested(textDate, false)
                    }
                    displayToast(qsTr( 'Date value set to today.'))
                }
            }
        }

        Image {
            id: clearButton
            width: 20
            height: 20
            source: Theme.getThemeIcon("ic_clear_black_18dp")
            sourceSize.width: 20 * screen.devicePixelRatio
            sourceSize.height: 20 * screen.devicePixelRatio
            fillMode: Image.PreserveAspectFit
            anchors.right: todayButton.left
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -2
            visible: (value !== undefined) && enabled
                     && (config['allow_null'] === undefined || config['allow_null'])

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    valueChangeRequested(undefined, true)
                }
            }
        }
    }

    FontMetrics {
        id: fontMetrics
        font: label.font
    }

    CalendarPanel {
        id: calendarPanel
        isDateTime: !main.fieldIsDate
        onDateTimePicked: {
            if ( main.isDateTimeType ) {
                valueChangeRequested(date, date === undefined)
            } else {
                var textDate = Qt.formatDateTime(date, config['field_format'])
                valueChangeRequested(date, date === undefined)
            }
        }
    }
}
