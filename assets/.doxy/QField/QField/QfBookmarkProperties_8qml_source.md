

# File QfBookmarkProperties.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfBookmarkProperties.qml**](QfBookmarkProperties_8qml.md)

[Go to the documentation of this file](QfBookmarkProperties_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: bookmarkProperties

  property string bookmarkId: ''
  property string bookmarkName: ''
  property string bookmarkGroup: ''
  property bool bookmarkDeleted: false

  parent: mainWindow.contentItem
  width: Math.min(350, mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin)

  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  onAboutToShow: {
    nameField.text = bookmarkName;
    colorContainer.value = bookmarkGroup;
  }

  onAboutToHide: {
    if (!bookmarkDeleted) {
      bookmarkModel.store();
    } else {
      bookmarkDeleted = false;
    }
  }

  function updateBookmark() {
    bookmarkModel.updateBookmarkDetails(bookmarkProperties.bookmarkId, nameField.text, colorContainer.value);
  }

  Page {
    width: parent.width
    padding: 5
    header: QfPageHeader {
      id: pageHeader
      title: qsTr("Bookmark Properties")

      showBackButton: false
      showApplyButton: true
      showCancelButton: false
      showRemoveButton: true
      backgroundFill: false

      onApply: {
        bookmarkProperties.close();
      }

      onRemove: {
        removeBookmarkDialog.open();
      }
    }

    ColumnLayout {
      id: propertiesLayout
      spacing: 10
      width: parent.width

      TextArea {
        id: nameField
        Layout.fillWidth: true
        Layout.fillHeight: false
        Layout.preferredHeight: Math.min(mainWindow.height - mainWindow.sceneTopMargin - mainWindow.sceneBottomMargin - 200, Math.max(144, contentHeight) + 24)
        font: QfTheme.defaultFont
        wrapMode: Text.Wrap
        placeholderText: qsTr("Description")
        text: ''

        onTextChanged: {
          updateBookmark();
        }
      }

      RowLayout {
        spacing: 8
        Layout.fillWidth: true
        Layout.preferredWidth: propertiesLayout.width
        Layout.alignment: Qt.AlignHCenter

        SwipeView {
          id: colorContainer

          property string value: ''
          onValueChanged: {
            updateBookmark();
          }

          Layout.fillWidth: true
          height: QfTheme.toolButtonSize

          clip: true
          interactive: false
          currentIndex: 0

          RowLayout {
            id: currentColorView
            width: colorContainer.width
            height: QfTheme.toolButtonSize
            spacing: 5

            Rectangle {
              id: colorArea
              Layout.fillWidth: true
              Layout.preferredHeight: QfTheme.toolButtonSize
              height: QfTheme.toolButtonSize
              radius: height / 2

              color: {
                switch (colorContainer.value) {
                case "orange":
                  return QfTheme.bookmarkOrange;
                case "red":
                  return QfTheme.bookmarkRed;
                case "blue":
                  return QfTheme.bookmarkBlue;
                }
                return QfTheme.bookmarkDefault;
              }

              Label {
                anchors.left: parent.left
                anchors.leftMargin: 24
                anchors.verticalCenter: colorPicker.verticalCenter
                font: QfTheme.defaultFont
                text: qsTr("Change color")
                color: "white"
              }

              QfToolButton {
                id: colorPicker
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: QfTheme.toolButtonSize
                height: QfTheme.toolButtonSize
                visible: true
                enabled: false
                iconSource: QfTheme.getThemeVectorIcon("ic_chevron_right_white_24dp")
                iconColor: "white"
                bgcolor: "transparent"
              }

              MouseArea {
                anchors.fill: parent
                enabled: true

                onClicked: {
                  colorContainer.currentIndex = 1;
                }
              }
            }
          }

          RowLayout {
            id: selectColorView
            width: colorContainer.width
            height: QfTheme.toolButtonSize
            spacing: 5

            ListView {
              Layout.fillWidth: true
              Layout.preferredHeight: QfTheme.toolButtonSize
              orientation: ListView.Horizontal
              spacing: 10
              model: ["", "orange", "red", "blue"]

              clip: true

              delegate: QfToolButton {
                Layout.preferredWidth: QfTheme.toolButtonSize
                Layout.preferredHeight: QfTheme.toolButtonSize
                bgcolor: {
                  switch (modelData) {
                  case "orange":
                    return QfTheme.bookmarkOrange;
                  case "red":
                    return QfTheme.bookmarkRed;
                  case "blue":
                    return QfTheme.bookmarkBlue;
                  }
                  return QfTheme.bookmarkDefault;
                }
                round: true

                iconSource: modelData === colorContainer.value ? QfTheme.getThemeVectorIcon("ic_check_white_24dp") : ""
                iconColor: "#ffffff"

                onClicked: {
                  colorContainer.value = modelData;
                  colorContainer.currentIndex = 0;
                }
              }
            }
          }
        }

        QfToolButton {
          height: QfTheme.toolButtonSize
          width: QfTheme.toolButtonSize
          iconSource: QfTheme.getThemeVectorIcon("ic_copy_black_24dp")
          iconColor: enabled ? QfTheme.mainTextColor : QfTheme.mainTextDisabledColor
          bgcolor: "transparent"

          onClicked: {
            const point = bookmarkModel.getBookmarkPoint(bookmarkProperties.bookmarkId);
            const crs = bookmarkModel.getBookmarkCrs(bookmarkProperties.bookmarkId);
            const coordinates = QfStringUtils.pointInformation(point, crs);
            platformUtilities.copyTextToClipboard(nameField.text + '\n' + coordinates);
            displayToast(qsTr('Bookmark details copied to clipboard'));
          }
        }
      }
    }
  }

  QfDialog {
    id: removeBookmarkDialog
    parent: mainWindow.contentItem
    z: 10000 // 1000s are embedded feature forms, user a higher value to insure the dialog will always show above embedded feature forms
    title: qsTr("Remove bookmark")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("You are about to remove a bookmark, proceed?")
    }

    onAccepted: {
      bookmarkModel.removeBookmark(bookmarkProperties.bookmarkId);
      bookmarkDeleted = true;
      bookmarkProperties.close();
    }
  }
}
```


