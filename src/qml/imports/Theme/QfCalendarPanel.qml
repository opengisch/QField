import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls.Material

QfDialog {
  id: calendarPopup
  focus: true
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

  parent: mainWindow.contentItem
  width: verticalView || showTimePicker ? mainWindow.width / 3 * 2.5 : 500
  height: verticalView && !showDatePicker ? 250 : mainWindow.height / 1.2
  clip: true
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes

  property bool showTimePicker: true
  property bool showDatePicker: true
  property bool verticalView: (mainWindow.width < 500 || mainWindow.screenIsPortrait) || (showTimePicker && !showDatePicker)
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
    Layout.preferredHeight: 40
    verticalAlignment: Text.AlignVCenter

    readonly property var monthNames: ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December']
    readonly property string rowView: selectedDate.getDate() + " " + monthNames[selectedDate.getMonth()] + " " + selectedDate.getFullYear()
    readonly property string columnView: selectedDate.getDate() + "\n" + monthNames[selectedDate.getMonth()] + "\n" + selectedDate.getFullYear()
  }

  CalendarItem {
    id: calendarItem
    Layout.preferredHeight: childrenRect.height
    Layout.preferredWidth: 350
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
    width: calendarItem.width
    __verticalView: !calendarPopup.verticalView
    Layout.preferredWidth: calendarPopup.verticalView ? 350 : 150
    Layout.preferredHeight: 40
  }

  Flickable {
    id: vericalLayout
    anchors.fill: parent
    contentHeight: verticalContent.height
    contentWidth: 350
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.horizontal: QfScrollBar {
    }
    ScrollBar.vertical: QfScrollBar {
    }

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
