

# File QfBrowserPanel.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfBrowserPanel.qml**](QfBrowserPanel_8qml.md)

[Go to the documentation of this file](QfBrowserPanel_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebView
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: browserPanel

  signal cancel

  property var browserView: undefined
  property var browserCookies: []

  property string url: ''
  property bool fullscreen: false
  property bool clearCookiesOnOpen: false

  width: mainWindow.width - (browserPanel.fullscreen ? 0 : QfTheme.popupScreenEdgeHorizontalMargin * 2)
  height: mainWindow.height - (browserPanel.fullscreen ? 0 : QfTheme.popupScreenEdgeVerticalMargin * 2)
  x: browserPanel.fullscreen ? 0 : QfTheme.popupScreenEdgeHorizontalMargin
  y: browserPanel.fullscreen ? 0 : QfTheme.popupScreenEdgeVerticalMargin
  padding: fullscreen ? 0 : 5
  closePolicy: Popup.CloseOnEscape
  focus: visible

  Page {
    id: browserContainer
    anchors.fill: parent
    padding: 0

    header: QfPageHeader {
      id: pageHeader
      title: browserView && !browserView.loading && browserView.title !== '' ? browserView.title : qsTr("Browser")

      showBackButton: browserPanel.fullscreen
      showApplyButton: false
      showCancelButton: !browserPanel.fullscreen

      busyIndicatorState: webViewLoader.item && webViewLoader.item.loading ? "on" : "off"

      topMargin: browserPanel.fullscreen ? mainWindow.sceneTopMargin : 0

      onBack: {
        browserPanel.cancel();
      }

      onCancel: {
        browserPanel.cancel();
      }
    }

    Loader {
      id: webViewLoader

      anchors.fill: parent
      active: browserPanel.opened
      sourceComponent: webViewComponent
    }
  }

  Component {
    id: webViewComponent

    WebView {
      anchors.fill: parent
      url: browserPanel.url

      onLoadingChanged: {
        if (!loading) {
          anchors.fill = parent;
          width = parent.width;
          height = parent.height;
          opacity = 1;
        }
      }
      onCookieAdded: (domain, name) => {
        browserPanel.browserCookies.push([domain, name]);
      }

      Component.onCompleted: {
        if (browserPanel.clearCookiesOnOpen) {
          deleteAllCookies();
          browserPanel.clearCookiesOnOpen = false;
        }
      }
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
  }

  function deleteCookies() {
    if (webViewLoader.item) {
      for (const [domain, name] of browserCookies) {
        webViewLoader.item.deleteCookie(domain, name);
      }
      browserCookies = [];
    }
  }
}
```


