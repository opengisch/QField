import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls.Material

Dialog {
  id: calendarPopup
  modal: true
  focus: true
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
  standardButtons: Dialog.Ok | Dialog.Cancel

  parent: mainWindow.contentItem
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  width: __verticalView || isDateTime ? mainWindow.width / 3 * 2.5 : 500
  height: mainWindow.height / 1.2
  clip: true
  z: 10000

  property bool __verticalView: mainWindow.width < 500
  property bool isDateTime: true
  property date selectedDate: new Date()

  signal dateTimePicked(var date)

  function setFittingLayout() {
    if (__verticalView) {
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
    text: (__verticalView ? rowView + " " : columnView + "\n") + (isDateTime ? Qt.formatTime(new Date(1970, 1, 1, timePicker.hours, timePicker.minutes, timePicker.seconds), "hh:mm:ss") : "")
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
    verticalView: !calendarPopup.__verticalView
    Layout.preferredWidth: calendarPopup.__verticalView ? 350 : 150
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
      spacing: 20

      LayoutItemProxy {
        target: result
      }

      LayoutItemProxy {
        target: calendarItem
      }

      LayoutItemProxy {
        target: timePicker
        visible: isDateTime
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
      }
      LayoutItemProxy {
        target: timePicker
        visible: isDateTime
      }
    }
  }

  function selectDate() {
    var newDate = calendarPopup.selectedDate;
    if (calendarPanel.isDateTime) {
      newDate.setHours(hoursSpinBox.value);
      newDate.setMinutes(minutesSpinBox.value);
      newDate.setSeconds(secondsSpinBox.value);
    }
    dateTimePicked(newDate);
    calendarPanel.close();
  }
}
