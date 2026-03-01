import QtQuick
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 *
 * A reusable banner that displays the current QFieldCloud service status.
 */
QfCollapsibleMessage {
  id: statusBanner

  property QFieldCloudStatus cloudServiceStatus: null

  visible: cloudServiceStatus && cloudServiceStatus.hasProblem

  Layout.fillWidth: true
  Layout.minimumHeight: visible ? height : 0

  color: {
    if (!cloudServiceStatus)
      return Theme.secondaryTextColor;
    switch (cloudServiceStatus.statusType) {
    case QFieldCloudStatus.Incident:
      return Theme.errorColor;
    case QFieldCloudStatus.Degraded:
      return Theme.warningColor;
    case QFieldCloudStatus.Maintenance:
      return Theme.cloudColor;
    default:
      return Theme.secondaryTextColor;
    }
  }

  detailsColor: Theme.secondaryTextColor
  font: Theme.tipFont
  titleText: cloudServiceStatus ? cloudServiceStatus.statusMessage : ''
  detailsText: cloudServiceStatus ? cloudServiceStatus.detailsMessage : ''
  externalLink: cloudServiceStatus ? cloudServiceStatus.statusPageUrl : ''
}
