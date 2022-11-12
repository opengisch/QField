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
  property bool fullscreen: false

  width: mainWindow.width - (browserPanel.fullscreen ? 0 : Theme.popupScreenEdgeMargin * 2)
  height: mainWindow.height - (browserPanel.fullscreen ? 0 : Theme.popupScreenEdgeMargin * 2)
  x: browserPanel.fullscreen ? 0 : Theme.popupScreenEdgeMargin
  y: browserPanel.fullscreen ? 0 : Theme.popupScreenEdgeMargin
  padding: 0
  modal: true
  closePolicy: Popup.CloseOnEscape

  Page {
    id: browserContainer
    anchors.fill: parent
    header: PageHeader {
      id: pageHeader
      title: browserView && !browserView.loading && browserView.title !== ''
             ? browserView.title
             : qsTr("Browser")

      showBackButton: browserPanel.fullscreen
      showApplyButton: false
      showCancelButton: !browserPanel.fullscreen

      busyIndicatorState: browserView && browserView.loading ? "on" : "off"

      topMargin: browserPanel.fullscreen ? mainWindow.sceneTopMargin : 0

      onBack: {
        browserPanel.cancel()
      }

      onCancel: {
        browserPanel.cancel()
      }
    }

    Item {
      id: browserContent
      anchors {
        top: parent.top
        left: parent.left
      }
      width: parent.width
      height: parent.height
    }
  }

  onAboutToShow: {
    if (url != '') {
      if (browserView === undefined) {
        // avoid cost of WevView creation until needed
        browserView = Qt.createQmlObject('import QtWebView 1.14; WebView { id: browserView; anchors { top: parent.top; left: parent.left; right: parent.right; } onLoadingChanged: { if ( !loading ) { anchors.fill = parent; width = parent.width; height = parent.height; opacity = 1; } } }', browserContent);
      }
      browserView.anchors.fill = undefined
      browserView.url = url
      browserView.opacity = 0
    }
  }
}
