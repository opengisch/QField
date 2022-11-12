import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtWebView 1.14

import org.qfield 1.0
import Theme 1.0

Popup {
  id: browserPanel

  signal cancel()

  property var browserView: undefined
  property string url: ''

  width: mainWindow.width - Theme.popupScreenEdgeMargin * 2
  height: mainWindow.height - Theme.popupScreenEdgeMargin * 2
  x: Theme.popupScreenEdgeMargin
  y: Theme.popupScreenEdgeMargin
  padding: 0
  modal: true
  closePolicy: Popup.CloseOnEscape

  Page {
    anchors.fill: parent
    header: PageHeader {
      id: pageHeader
      title: qsTr("Browser")

      showBackButton: false
      showApplyButton: false
      showCancelButton: true

      onCancel: {
        browserPanel.cancel()
      }
    }

    Item {
      id: browserContainer
      width: parent.width
      height: parent.height
    }

    onVisibleChanged: {
      // avoid cost of WevView creation until needed
      if (visible && url != '') {
        if (browserView === undefined) {
          browserView = Qt.createQmlObject('import QtWebView 1.14; WebView { id: browserView; onLoadingChanged: if ( !loading ) { anchors.fill = parent; } }', browserContainer);
        }
        browserView.url = url;
      }
    }
  }
}
