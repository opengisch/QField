import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme

/**
 * \ingroup qml
 */
Page {
  signal finished

  visible: false
  focus: visible

  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  header: QfPageHeader {
    title: qsTr("GNSS Status")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false
    backAsCancel: true

    topMargin: mainWindow.sceneTopMargin

    onFinished: {
      parent.finished();
    }
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: 20

    Item {
      Layout.fillHeight: true
    }

    Label {
      text: qsTr("GNSS Status - Coming in Story 5.1")
      font: Theme.strongFont
      color: Theme.mainTextColor
      horizontalAlignment: Text.AlignHCenter
      Layout.fillWidth: true
    }

    Item {
      Layout.fillHeight: true
    }
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true;
      finished();
    }
  }
}
