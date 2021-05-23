import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtWebView 1.14

import org.qfield 1.0
import Theme 1.0

Page {
    signal cancel()

    property var browserView: undefined
    property string url: ''

    header: PageHeader {
        id: pageHeader
        title: qsTr("Browser")

        showApplyButton: false
        showCancelButton: true

        onCancel: {
            parent.cancel()
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
