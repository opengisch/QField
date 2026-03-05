

# File BrowserPanel.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**BrowserPanel.qml**](BrowserPanel_8qml.md)

[Go to the documentation of this file](BrowserPanel_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebView
import org.qfield
import Theme

QfPopup {
  id: browserPanel

  signal cancel

  property var browserView: undefined
  property var browserCookies: []

  property string url: ''
  property bool fullscreen: false
  property bool clearCookiesOnOpen: false

  width: mainWindow.width - (browserPanel.fullscreen ? 0 : Theme.popupScreenEdgeHorizontalMargin * 2)
  height: mainWindow.height - (browserPanel.fullscreen ? 0 : Theme.popupScreenEdgeVerticalMargin * 2)
  x: browserPanel.fullscreen ? 0 : Theme.popupScreenEdgeVerticalMargin
  y: browserPanel.fullscreen ? 0 : Theme.popupScreenEdgeVerticalMargin
  padding: fullscreen ? 0 : 5
  closePolicy: Popup.CloseOnEscape
  focus: visible

  Page {
    id: browserContainer
    anchors.fill: parent
    padding: 5

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

  onAboutToHide: {
    iface.setScreenDimmerTimeout(settings.value('dimTimeoutSeconds', 60));
  }

  onAboutToShow: {
    // Disable dimming to avoid dark screens while browsing
    iface.setScreenDimmerTimeout(0);

    // Reset tracked cookies
    browserCookies = [];
    if (url !== '') {
      if (browserView === undefined) {
        // avoid cost of WevView creation until needed
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
```


