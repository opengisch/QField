

# File QfCalendarPanel.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfCalendarPanel.qml**](QfCalendarPanel_8qml.md)

[Go to the documentation of this file](QfCalendarPanel_8qml.md)


```C++
import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls.Material
import org.qfield

QfDialog {
  id: calendarPopup
  focus: true
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

  parent: mainWindow.contentItem
  width: {
    if (verticalView)
      return Math.min(400, mainWindow.width / 3 * 2.5);
    else
      return Math.min(horizontalContent.width + 60, mainWindow.width / 3 * 2.5);
  }
  height: {
    if (verticalView)
      return Math.min(verticalContent.height + 100, mainWindow.height / 1.2);
    else
      return Math.min(Math.max(calendarItem.height, timePicker.height) + 100, mainWindow.height / 1.2);
  }
  clip: true
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes

  property bool showTimePicker: true
  property bool showDatePicker: true
  property bool verticalView: (mainWindow.width < 800 || mainWindow.screenIsPortrait) || !showDatePicker
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
    verticalAlignment: Text.AlignTop
    clip: true
    wrapMode: Text.WordWrap
    Layout.fillWidth: true
    color: Theme.mainTextColor
    readonly property var monthNames: [qsTr("January"), qsTr("February"), qsTr("March"), qsTr("April"), qsTr("May"), qsTr("June"), qsTr("July"), qsTr("August"), qsTr("September"), qsTr("October"), qsTr("November"), qsTr("December")]
    readonly property string rowView: selectedDate.getDate() + " " + monthNames[selectedDate.getMonth()] + " " + selectedDate.getFullYear()
    readonly property string columnView: selectedDate.getDate() + "\n" + monthNames[selectedDate.getMonth()] + "\n" + selectedDate.getFullYear()
  }

  QfCalendarItem {
    id: calendarItem
    Layout.preferredHeight: childrenRect.height
    Layout.preferredWidth: Math.min(250, calendarPopup.width)
    initialDate: selectedDate
    compactMode: !calendarPopup.verticalView
    onDateClicked: date => {
      if (selectedDate.getFullYear() !== date.getFullYear() || selectedDate.getMonth() !== date.getMonth() || selectedDate.getDate() !== date.getDate()) {
        selectedDate = date;
      } else {
        selectDate();
      }
    }
  }

  QfTimeItem {
    id: timePicker
    __verticalView: !calendarPopup.verticalView || calendarPopup.width < 400
    Layout.preferredWidth: Math.min(__verticalView ? 150 : 350, (calendarPopup.width - 20))
    Layout.minimumHeight: Math.min(contentHeight, (calendarPopup.height - 20))
    Layout.alignment: Qt.AlignTop
  }

  Flickable {
    id: vericalLayout
    anchors.fill: parent
    contentHeight: verticalContent.height
    contentWidth: Math.max(calendarItem.visible ? calendarItem.width : 0, timePicker.visible ? timePicker.width : 0)
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.horizontal: QfScrollBar {}
    ScrollBar.vertical: QfScrollBar {}
    anchors.horizontalCenter: parent.horizontalCenter
    clip: true

    ColumnLayout {
      id: verticalContent
      width: vericalLayout.width
      spacing: 8

      LayoutItemProxy {
        target: result
      }

      LayoutItemProxy {
        target: calendarItem
        visible: showDatePicker
        Layout.alignment: Qt.AlignHCenter
      }

      LayoutItemProxy {
        target: timePicker
        visible: showTimePicker
        Layout.alignment: Qt.AlignHCenter
      }
    }
  }

  Flickable {
    id: horizontalLayout
    anchors.fill: parent
    contentHeight: horizontalContent.height
    contentWidth: horizontalContent.width
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.horizontal: QfScrollBar {}
    ScrollBar.vertical: QfScrollBar {}
    clip: true

    RowLayout {
      id: horizontalContent
      height: Math.max(calendarItem.childrenRect.height, timePicker.height)
      spacing: 30

      LayoutItemProxy {
        target: result
        Layout.fillHeight: true
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
```


