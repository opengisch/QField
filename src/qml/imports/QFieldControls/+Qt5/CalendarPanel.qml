import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import Qt.labs.calendar 1.0

import Theme 1.0
import org.qfield 1.0

import "."


Popup {
    id: calendarPopup

    property bool isDateTime: false;
    property date selectedDate: new Date()

    signal dateTimePicked(var date);

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    parent: ApplicationWindow.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes

    onAboutToShow: {
        if (selectedDate !== undefined) {
            calendar.month = selectedDate.getMonth();
            calendar.year = selectedDate.getFullYear();
            hoursSpinBox.value = selectedDate.getHours();
            minutesSpinBox.value = selectedDate.getMinutes();
            secondsSpinBox.value = selectedDate.getSeconds();
        }
    }

    Timer {
        id: changeCalendarTimer
        interval: 700
        repeat: true

        property string action: ''

        onTriggered: {
          switch(action) {
              case 'decreaseYear':
                  calendar.decreaseYear();
                  break;
              case 'decreaseMonth':
                  calendar.decreaseMonth();
                  break;
              case 'increaseYear':
                  calendar.increaseYear();
                  break;
              case 'increaseMonth':
                  calendar.increaseMonth();
                  break;
          }
          if ( interval > 100 ) interval = interval * 0.8;
        }
    }

    ColumnLayout {
        Rectangle {
            id: calendarOverlay
            width: 350
            height: childrenRect.height
            color: "transparent"

            MouseArea {
                anchors.fill: parent
                onClicked: mouse.accepted = true
                onWheel: wheel.accepted = true
            }

            GridLayout {
                id: calendarGrid
                anchors.left: parent.left
                anchors.right: parent.right
                columns: 3

                Row {
                    Layout.column: 0
                    Layout.row: 0

                    QfToolButton {
                        enabled: true
                        iconSource: Theme.getThemeIcon( 'ic_doublearrow_left_black_24dp' )
                        bgcolor: "transparent"
                        roundborder: true

                        onPressed: {
                            calendar.decreaseYear();
                            changeCalendarTimer.action = 'decreaseYear';
                            changeCalendarTimer.interval = 700;
                            changeCalendarTimer.restart();
                        }
                        onReleased: {
                            changeCalendarTimer.stop()
                        }
                        onCanceled: {
                            changeCalendarTimer.stop()
                        }
                    }

                    QfToolButton {
                        enabled: true
                        iconSource: Theme.getThemeIcon( 'ic_arrow_left_black_24dp' )
                        bgcolor: "transparent"
                        roundborder: true

                        onPressed: {
                            calendar.decreaseMonth();
                            changeCalendarTimer.action = 'decreaseMonth';
                            changeCalendarTimer.interval = 700;
                            changeCalendarTimer.restart();
                        }
                        onReleased: {
                            changeCalendarTimer.stop()
                        }
                        onCanceled: {
                            changeCalendarTimer.stop()
                        }
                    }
                }

                Text {
                    text: calendar.title
                    horizontalAlignment: Text.AlignHCenter
                    Layout.column: 1
                    Layout.row: 0
                    Layout.fillWidth: true
                    font: Theme.tipFont
                }

                Row {
                    Layout.column: 2
                    Layout.row: 0

                    QfToolButton {
                        enabled: true
                        iconSource: Theme.getThemeIcon( 'ic_arrow_right_black_24dp' )
                        bgcolor: "transparent"
                        roundborder: true

                        onPressed: {
                            calendar.increaseMonth();
                            changeCalendarTimer.action = 'increaseMonth';
                            changeCalendarTimer.interval = 700;
                            changeCalendarTimer.restart();
                        }
                        onReleased: {
                            changeCalendarTimer.stop()
                        }
                        onCanceled: {
                            changeCalendarTimer.stop()
                        }
                    }
                    QfToolButton {
                        enabled: true
                        iconSource: Theme.getThemeIcon( 'ic_doublearrow_right_black_24dp' )
                        bgcolor: "transparent"
                        roundborder: true

                        onPressed: {
                            calendar.increaseYear();
                            changeCalendarTimer.action = 'increaseYear';
                            changeCalendarTimer.interval = 700;
                            changeCalendarTimer.restart();
                        }
                        onReleased: {
                            changeCalendarTimer.stop()
                        }
                        onCanceled: {
                            changeCalendarTimer.stop()
                        }
                    }
                }

                DayOfWeekRow {
                    locale: calendar.locale

                    Layout.column: 0
                    Layout.columnSpan: 3
                    Layout.row: 1
                    Layout.fillWidth: true
                }

                MonthGrid {
                    id: calendar

                    property date today: new Date()

                    function decreaseYear() {
                        year -= 1;
                    }
                    function increaseYear() {
                        year += 1;
                    }
                    function decreaseMonth() {
                        if (month !== Calendar.January) {
                            month -= 1;
                        } else {
                            year -= 1;
                            month = Calendar.December;
                        }
                    }
                    function increaseMonth() {
                        if (month != Calendar.December) {
                            month += 1;
                        } else {
                            month = Calendar.January
                            year += 1;
                        }
                    }

                    month: Calendar.January
                    year: 2020
                    Layout.row: 2
                    Layout.column: 0
                    Layout.columnSpan: 3
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40 * 6

                    delegate: Rectangle {
                        property bool isSelectedDate: calendarPopup.selectedDate.getFullYear() === model.date.getFullYear() && calendarPopup.selectedDate.getMonth() === model.date.getMonth() && calendarPopup.selectedDate.getDate() === model.date.getDate()
                        property bool isNow: calendar.today.getFullYear() === model.date.getFullYear() && calendar.today.getMonth() === model.date.getMonth() && calendar.today.getDate() === model.date.getDate()
                        color:  isSelectedDate ? Theme.mainColor : "transparent"
                        width: 18
                        height: 18
                        radius: 10

                        Text {
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            opacity: model.month !== calendar.month ? 0.5 : 1
                            text: model.day
                            font.pointSize: Theme.tipFont.pointSize
                            font.bold: parent.isSelectedDate ? true : false
                            font.underline: parent.isNow ? true : false
                            color: parent.isSelectedDate ? "white" : "black"
                        }
                    }

                    onClicked: {
                        if (calendarPopup.selectedDate.getFullYear() !== date.getFullYear() ||
                            calendarPopup.selectedDate.getMonth() !== date.getMonth() ||
                            calendarPopup.selectedDate.getDate() !== date.getDate()) {
                          calendarPopup.selectedDate = date;
                        } else {
                          calendarPopup.selectDate();
                        }
                    }

                    function resetDate() {
                        calendarPopup.selectedDate = new Date()
                    }
                }
            }
        }

        RowLayout {
            GridLayout {
                id: timeGrid
                visible: calendarPanel.isDateTime
                Layout.alignment: Qt.AlignHCenter
                Layout.leftMargin: 20
                rows: 3
                columns: 2

                Label {
                    Layout.alignment: Qt.AlignRight
                    Layout.row: 0
                    Layout.column: 0
                    text: qsTr( "Hours" )
                    font: Theme.tipFont
                }
                SpinBox {
                    id: hoursSpinBox
                    Layout.row: 0
                    Layout.column: 1
                    editable: true
                    from: 0
                    to: 23
                    value: 12
                    inputMethodHints: Qt.ImhTime
                    font: Theme.tipFont
                }
                Label {
                    Layout.alignment: Qt.AlignRight
                    Layout.row: 1
                    Layout.column: 0
                    text: qsTr( "Minutes" )
                    font: Theme.tipFont
                }
                SpinBox {
                    id: minutesSpinBox
                    Layout.row: 1
                    Layout.column: 1
                    editable: true
                    from: 0
                    to: 59
                    value: 30
                    inputMethodHints: Qt.ImhTime
                    font: Theme.tipFont
                }
                Label {
                    Layout.alignment: Qt.AlignRight
                    Layout.row: 2
                    Layout.column: 0
                    text: qsTr( "Seconds" )
                    font: Theme.tipFont
                }
                SpinBox {
                    id: secondsSpinBox
                    Layout.row: 2
                    Layout.column: 1
                    editable: true
                    from: 0
                    to: 59
                    value: 30
                    inputMethodHints: Qt.ImhTime
                    font: Theme.tipFont
                }
            }
        }

        RowLayout {
            QfButton {
                id: okButton
                text: qsTr( "OK" )
                font: Theme.tipFont
                Layout.fillWidth: true

                onClicked: selectDate();
            }
        }
    }

    function selectDate() {
        var newDate = calendarPopup.selectedDate
        if (calendarPanel.isDateTime) {
            newDate.setHours(hoursSpinBox.value);
            newDate.setMinutes(minutesSpinBox.value);
            newDate.setSeconds(secondsSpinBox.value);
        }
        dateTimePicked(newDate);
        calendarPanel.close()
    }
}


