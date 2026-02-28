

# File TemporalProperties.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**TemporalProperties.qml**](TemporalProperties_8qml.md)

[Go to the documentation of this file](TemporalProperties_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

QfPopup {
  id: temporalProperties

  property MapSettings mapSettings

  parent: mainWindow.contentItem
  width: Math.min(350, mainWindow.width - Theme.popupScreenEdgeHorizontalMargin)
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  Page {
    width: parent.width
    padding: 5
    header: QfPageHeader {
      id: pageHeader
      title: qsTr("Temporal Properties")

      showBackButton: false
      showApplyButton: false
      showCancelButton: true
      backgroundFill: false

      onCancel: {
        temporalProperties.close();
      }
    }

    ColumnLayout {
      spacing: 4
      width: parent.width

      CheckBox {
        id: isTemporalCheckBox
        Layout.fillWidth: true
        topPadding: 5
        bottomPadding: 5
        text: qsTr('Enable temporal filter')
        font: Theme.defaultFont
        // visible for all layer tree items but nonspatial vector layer
        indicator.height: 16
        indicator.width: 16
        indicator.implicitHeight: 24
        indicator.implicitWidth: 24
        checked: mapCanvas.mapSettings.isTemporal

        onClicked: {
          mapCanvas.mapSettings.isTemporal = !mapCanvas.mapSettings.isTemporal;
        }
      }

      RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 20
        Layout.rightMargin: 20

        QfTextField {
          id: beginField
          Layout.fillWidth: true
          Layout.leftMargin: 20
          Layout.rightMargin: 20

          enabled: mapCanvas.mapSettings.isTemporal
          font: Theme.defaultFont
          horizontalAlignment: TextInput.AlignHCenter

          inputMethodHints: Qt.ImhDigitsOnly
          inputMask: "9999-99-99 99:99:99;_"

          text: Qt.formatDateTime(mapCanvas.mapSettings.temporalBegin, "yyyy-MM-dd HH:mm:ss")

          onTextEdited: {
            var newDate = Date.fromLocaleString(Qt.locale(), beginField.text, "yyyy-MM-dd HH:mm:ss");
            if (newDate.toLocaleString() !== "") {
              mapCanvas.mapSettings.temporalBegin = newDate;
            } else {
              mapCanvas.mapSettings.temporalBegin = Date.fromLocaleString('');
            }
          }
        }

        QfToolButton {
          iconSource: Theme.getThemeVectorIcon("ic_calendar_month_black_24dp")
          iconColor: Theme.mainTextColor
          bgcolor: "transparent"
          onClicked: {
            calendarPanel.selectedDate = !isNaN(mapCanvas.mapSettings.temporalBegin) ? mapCanvas.mapSettings.temporalBegin : new Date();
            calendarPanel.temporalField = 'begin';
            calendarPanel.open();
          }
        }
      }

      Label {
        Layout.fillWidth: true

        enabled: mapCanvas.mapSettings.isTemporal
        font: Theme.defaultFont
        horizontalAlignment: TextInput.AlignHCenter
        text: '≤ t ≤'
      }

      RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 20
        Layout.rightMargin: 20

        QfTextField {
          id: endField
          Layout.fillWidth: true

          enabled: mapCanvas.mapSettings.isTemporal
          font: Theme.defaultFont
          horizontalAlignment: TextInput.AlignHCenter

          inputMethodHints: Qt.ImhDigitsOnly
          inputMask: "9999-99-99 99:99:99;_"

          text: Qt.formatDateTime(mapCanvas.mapSettings.temporalEnd, "yyyy-MM-dd HH:mm:ss")

          onTextEdited: {
            var newDate = Date.fromLocaleString(Qt.locale(), endField.text, "yyyy-MM-dd HH:mm:ss");
            if (newDate.toLocaleString() !== "") {
              mapCanvas.mapSettings.temporalEnd = newDate;
            } else {
              mapCanvas.mapSettings.temporalEnd = Date.fromLocaleString('');
            }
          }
        }

        QfToolButton {
          iconSource: Theme.getThemeVectorIcon("ic_calendar_month_black_24dp")
          iconColor: Theme.mainTextColor
          bgcolor: "transparent"
          onClicked: {
            calendarPanel.selectedDate = !isNaN(mapCanvas.mapSettings.temporalEnd) ? mapCanvas.mapSettings.temporalEnd : new Date();
            calendarPanel.temporalField = 'end';
            calendarPanel.open();
          }
        }
      }
    }
  }

  QfCalendarPanel {
    id: calendarPanel

    property string temporalField: ''

    onDateTimePicked: date => {
      if (temporalField == 'begin') {
        mapCanvas.mapSettings.temporalBegin = date;
        beginField.text = Qt.formatDateTime(date, "yyyy-MM-dd HH:mm:ss");
      } else if (temporalField == 'end') {
        mapCanvas.mapSettings.temporalEnd = date;
        endField.text = Qt.formatDateTime(date, "yyyy-MM-dd HH:mm:ss");
      }
      temporalField = '';
    }
  }
}
```


