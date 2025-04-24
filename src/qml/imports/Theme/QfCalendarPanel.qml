import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls.Material

QfDialog {
  id: calendarPopup
  focus: true
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

  parent: mainWindow.contentItem
  width: verticalView && !showDatePicker ? Math.min(400, mainWindow.width - 32) : verticalView || showTimePicker ? mainWindow.width - 32 : 500
  height: verticalView && !showDatePicker ? 380 : mainWindow.height - 32
  clip: true
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes

  property bool showTimePicker: true
  property bool showDatePicker: true
  property bool verticalView: (mainWindow.width < 600 || mainWindow.screenIsPortrait) || (showTimePicker && !showDatePicker)
  property date selectedDate: new Date()

  signal dateTimePicked(var date)

  function setFittingLayout() {
    if (verticalView) {
      vericalLayout.visible = true;
      horizontalLayout.visible = false;
    } else {
      vericalLayout.visible = false;
      horizontalLayout.visible = true;
    }
  }

  onWidthChanged: setFittingLayout()
  Component.onCompleted: setFittingLayout()
  onAboutToShow: {
    if (selectedDate !== undefined) {
      calendarItem.month = selectedDate.getMonth();
      calendarItem.year = selectedDate.getFullYear();
      timePicker.hours = selectedDate.getHours();
      timePicker.minutes = selectedDate.getMinutes();
      timePicker.seconds = selectedDate.getSeconds();
    }
  }

  onAccepted: selectDate()

  Text {
    id: result
    font.pixelSize: 32
    text: (showDatePicker ? (verticalView ? rowView + " " : columnView + "\n") : "") + (showTimePicker ? Qt.formatTime(new Date(1970, 1, 1, timePicker.hours, timePicker.minutes, timePicker.seconds), "hh:mm:ss") : "")
    verticalAlignment: Text.AlignVCenter
    clip: true
    wrapMode: Text.WordWrap
    Layout.fillWidth: true
    width: parent.width
    readonly property var monthNames: ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December']
    readonly property string rowView: selectedDate.getDate() + " " + monthNames[selectedDate.getMonth()] + " " + selectedDate.getFullYear()
    readonly property string columnView: selectedDate.getDate() + "\n" + monthNames[selectedDate.getMonth()] + "\n" + selectedDate.getFullYear()
  }

  CalendarItem {
    id: calendarItem
    Layout.preferredHeight: childrenRect.height
    Layout.preferredWidth: Math.min(250, calendarPopup.width - 48)
    initialDate: selectedDate
    onDateClicked: date => {
      if (selectedDate.getFullYear() !== date.getFullYear() || selectedDate.getMonth() !== date.getMonth() || selectedDate.getDate() !== date.getDate()) {
        selectedDate = date;
      } else {
        selectDate();
      }
    }
  }

  TimePickerItem {
    id: timePicker
    __verticalView: !calendarPopup.verticalView || calendarPopup.width < 400
    Layout.preferredWidth: Math.min(__verticalView ? 150 : 350, (calendarPopup.width - 20))
    Layout.minimumHeight: Math.min(__verticalView ? 120 : 40, (calendarPopup.height - 20))
    Layout.alignment: Qt.AlignHCenter
    anchors.horizontalCenter: parent.horizontalCenter
  }

  Flickable {
    id: vericalLayout
    anchors.fill: parent
    contentHeight: verticalContent.height
    contentWidth: Math.max(calendarItem.visible ? calendarItem.width : 0, timePicker.visible ? timePicker.width : 0)
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.horizontal: QfScrollBar {
    }
    ScrollBar.vertical: QfScrollBar {
    }
    anchors.horizontalCenter: parent.horizontalCenter
    clip: true

    ColumnLayout {
      id: verticalContent
      width: parent.width
      spacing: showDatePicker ? 20 : 40

      LayoutItemProxy {
        target: result
      }

      LayoutItemProxy {
        target: calendarItem
        visible: showDatePicker
      }

      LayoutItemProxy {
        target: timePicker
        visible: showTimePicker
        Layout.alignment: Qt.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
      }
    }
  }

  Flickable {
    id: horizontalLayout
    anchors.fill: parent
    contentHeight: horizontalContent.height
    contentWidth: horizontalContent.width
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.horizontal: QfScrollBar {
    }
    ScrollBar.vertical: QfScrollBar {
    }

    RowLayout {
      id: horizontalContent
      height: calendarItem.height

      LayoutItemProxy {
        target: result
      }
      LayoutItemProxy {
        target: calendarItem
        visible: showDatePicker
      }
      LayoutItemProxy {
        target: timePicker
        visible: showTimePicker
      }
    }
  }

  function selectDate() {
    var newDate = new Date();
    if (calendarPanel.showDatePicker) {
      newDate = calendarPopup.selectedDate;
    }
    if (calendarPanel.showTimePicker) {
      newDate.setHours(timePicker.hours);
      newDate.setMinutes(timePicker.minutes);
      newDate.setSeconds(timePicker.seconds);
    }
    dateTimePicked(newDate);
    calendarPanel.close();
  }
}
