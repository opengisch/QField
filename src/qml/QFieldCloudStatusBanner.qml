import QtQuick
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 *
 * QFieldCloudStatusBanner displays a collapsible message when QFieldCloud
 * service is degraded, has incidents, or is under maintenance.
 * Hidden by default; visible only when there is a problem.
 */
Item {
  id: statusBanner

  property QFieldCloudStatus statusSource: null

  visible: statusSource && statusSource.hasProblem

  Layout.fillWidth: true
  Layout.minimumHeight: visible ? collapsibleMessage.height : 0

  QfCollapsibleMessage {
    id: collapsibleMessage
    width: parent.width

    color: Theme.darkRed
    detailsColor: Theme.secondaryTextColor
    font: Theme.tipFont

    titleText: statusSource ? statusSource.statusMessage : ''
    detailsText: statusSource ? statusSource.detailsMessage : ''
    externalLink: statusSource ? statusSource.statusPageUrl : ''
  }
}
