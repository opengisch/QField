

# File BookmarkProperties.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**BookmarkProperties.qml**](BookmarkProperties_8qml.md)

[Go to the documentation of this file](BookmarkProperties_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

QfPopup {
  id: bookmarkProperties

  property string bookmarkId: ''
  property string bookmarkName: ''
  property string bookmarkGroup: ''
  property bool bookmarkDeleted: false

  parent: mainWindow.contentItem
  width: Math.min(350, mainWindow.width - Theme.popupScreenEdgeHorizontalMargin)

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
        font: Theme.defaultFont
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
          height: 48

          clip: true
          interactive: false
          currentIndex: 0

          RowLayout {
            id: currentColorView
            width: colorContainer.width
            height: 48
            spacing: 5

            Rectangle {
              id: colorArea
              Layout.fillWidth: true
              Layout.preferredHeight: 48
              height: 48
              radius: height / 2

              color: {
                switch (colorContainer.value) {
                case "orange":
                  return Theme.bookmarkOrange;
                case "red":
                  return Theme.bookmarkRed;
                case "blue":
                  return Theme.bookmarkBlue;
                }
                return Theme.bookmarkDefault;
              }

              Label {
                anchors.left: parent.left
                anchors.leftMargin: 24
                anchors.verticalCenter: colorPicker.verticalCenter
                font: Theme.defaultFont
                text: qsTr("Change color")
                color: "white"
              }

              QfToolButton {
                id: colorPicker
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: 48
                height: 48
                visible: true
                enabled: false
                iconSource: Theme.getThemeVectorIcon("ic_chevron_right_white_24dp")
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
            height: 48
            spacing: 5

            ListView {
              Layout.fillWidth: true
              Layout.preferredHeight: 48
              orientation: ListView.Horizontal
              spacing: 10
              model: ["", "orange", "red", "blue"]

              clip: true

              delegate: QfToolButton {
                Layout.preferredWidth: 48
                Layout.preferredHeight: 48
                bgcolor: {
                  switch (modelData) {
                  case "orange":
                    return Theme.bookmarkOrange;
                  case "red":
                    return Theme.bookmarkRed;
                  case "blue":
                    return Theme.bookmarkBlue;
                  }
                  return Theme.bookmarkDefault;
                }
                round: true

                iconSource: modelData === colorContainer.value ? Theme.getThemeVectorIcon("ic_check_white_24dp") : ""
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
          height: 48
          width: 48
          iconSource: Theme.getThemeVectorIcon("ic_copy_black_24dp")
          iconColor: enabled ? Theme.mainTextColor : Theme.mainTextDisabledColor
          bgcolor: "transparent"

          onClicked: {
            var point = bookmarkModel.getBookmarkPoint(bookmarkProperties.bookmarkId);
            var crs = bookmarkModel.getBookmarkCrs(bookmarkProperties.bookmarkId);
            var coordinates = StringUtils.pointInformation(point, crs);
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


