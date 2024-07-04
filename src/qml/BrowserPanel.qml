import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtWebView 1.14
import org.qfield 1.0
import Theme 1.0

Popup {
  id: browserPanel

  signal cancel

  property var browserView: undefined
  property var browserCookies: []

  property string url: ''
  property bool fullscreen: false
  property bool clearCookiesOnOpen: false

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
    header: QfPageHeader {
      id: pageHeader
      title: browserView && !browserView.loading && browserView.title !== '' ? browserView.title : qsTr("Browser")

      showBackButton: browserPanel.fullscreen
      showApplyButton: false
      showCancelButton: !browserPanel.fullscreen

      busyIndicatorState: browserView && browserView.loading ? "on" : "off"

      topMargin: browserPanel.fullscreen ? mainWindow.sceneTopMargin : 0

      onBack: {
        browserPanel.cancel();
      }

      onCancel: {
        browserPanel.cancel();
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
    // Reset tracked cookies
    browserCookies = [];
    if (url != '') {
      if (browserView === undefined) {
        // avoid cost of WevView creation until needed
        if (qVersion >= '6.0.0') {
          browserView = Qt.createQmlObject('import QtWebView
            WebView {
              id: browserView
              anchors { top: parent.top; left: parent.left; right: parent.right; }
              onLoadingChanged: {
                if ( !loading ) {
                  anchors.fill = parent; width = parent.width
                  height = parent.height; opacity = 1
                }
              }
              onCookieAdded: (domain, name) => {
                browserPanel.browserCookies.push([domain, name])
              }
            }', browserContent);
          if (clearCookiesOnOpen) {
            browserView.deleteAllCookies();
          }
        } else {
          browserView = Qt.createQmlObject('import QtWebView 1.14
            WebView {
              id: browserView
              anchors { top: parent.top; left: parent.left; right: parent.right; }
              onLoadingChanged: {
                if ( !loading ) {
                  anchors.fill = parent; width = parent.width
                  height = parent.height; opacity = 1
                }
              }
            }', browserContent);
        }
      }
      browserView.anchors.fill = undefined;
      browserView.url = url;
      browserView.opacity = 0;
    }
    clearCookiesOnOpen = false;
  }

  function deleteCookies() {
    for (const [domain, name] of browserCookies) {
      browserView.deleteCookie(domain, name);
    }
    browserCookies = [];
  }
}
