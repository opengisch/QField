

# File QfCalendarItem.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfCalendarItem.qml**](QfCalendarItem_8qml.md)

[Go to the documentation of this file](QfCalendarItem_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield

Item {
  id: calendarItem

  property date initialDate
  property bool compactMode: false

  property alias month: calendar.month
  property alias year: calendar.year

  signal dateClicked(var date)

  MouseArea {
    anchors.fill: parent
    onClicked: mouse => {
      mouse.accepted = true;
    }
    onWheel: wheel => {
      wheel.accepted = false;
    }
  }

  GridLayout {
    anchors.left: parent.left
    anchors.right: parent.right
    columns: 3

    Row {
      Layout.column: 0
      Layout.row: 0

      QfToolButton {
        enabled: true
        iconSource: Theme.getThemeVectorIcon('ic_doublearrow_left_black_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"
        roundborder: true

        onPressed: {
          calendar.decreaseYear();
          changeCalendarTimer.action = 'decreaseYear';
          changeCalendarTimer.interval = 700;
          changeCalendarTimer.restart();
        }
        onReleased: {
          changeCalendarTimer.stop();
        }
        onCanceled: {
          changeCalendarTimer.stop();
        }
      }

      QfToolButton {
        enabled: true
        iconSource: Theme.getThemeVectorIcon('ic_arrow_left_black_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"
        roundborder: true

        onPressed: {
          calendar.decreaseMonth();
          changeCalendarTimer.action = 'decreaseMonth';
          changeCalendarTimer.interval = 700;
          changeCalendarTimer.restart();
        }
        onReleased: {
          changeCalendarTimer.stop();
        }
        onCanceled: {
          changeCalendarTimer.stop();
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
      color: Theme.mainTextColor
    }

    Row {
      Layout.column: 2
      Layout.row: 0

      QfToolButton {
        enabled: true
        iconSource: Theme.getThemeVectorIcon('ic_arrow_right_black_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"
        roundborder: true

        onPressed: {
          calendar.increaseMonth();
          changeCalendarTimer.action = 'increaseMonth';
          changeCalendarTimer.interval = 700;
          changeCalendarTimer.restart();
        }
        onReleased: {
          changeCalendarTimer.stop();
        }
        onCanceled: {
          changeCalendarTimer.stop();
        }
      }
      QfToolButton {
        enabled: true
        iconSource: Theme.getThemeVectorIcon('ic_doublearrow_right_black_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"
        roundborder: true

        onPressed: {
          calendar.increaseYear();
          changeCalendarTimer.action = 'increaseYear';
          changeCalendarTimer.interval = 700;
          changeCalendarTimer.restart();
        }
        onReleased: {
          changeCalendarTimer.stop();
        }
        onCanceled: {
          changeCalendarTimer.stop();
        }
      }
    }

    DayOfWeekRow {
      locale: calendar.locale

      Layout.column: 0
      Layout.columnSpan: 3
      Layout.row: 1
      Layout.fillWidth: true

      delegate: Text {
        text: model.shortName
        font: Theme.tipFont
        color: Theme.secondaryTextColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
      }
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
        if (month !== Calendar.December) {
          month += 1;
        } else {
          month = Calendar.January;
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
      Layout.maximumHeight: compactMode ? 30 * 6 : 40 * 6

      delegate: Rectangle {
        property bool isSelectedDate: initialDate.getFullYear() === model.date.getFullYear() && initialDate.getMonth() === model.date.getMonth() && initialDate.getDate() === model.date.getDate()
        property bool isNow: calendar.today.getFullYear() === model.date.getFullYear() && calendar.today.getMonth() === model.date.getMonth() && calendar.today.getDate() === model.date.getDate()
        color: isSelectedDate ? Theme.mainColor : "transparent"
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
          color: parent.isSelectedDate ? Theme.mainOverlayColor : Theme.mainTextColor
        }
      }

      onClicked: function (date) {
        dateClicked(date);
      }
    }
  }

  Timer {
    id: changeCalendarTimer
    interval: 700
    repeat: true

    property string action: ''

    onTriggered: {
      switch (action) {
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
      if (interval > 100)
        interval = interval * 0.8;
    }
  }
}
```


