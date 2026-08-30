import QtQuick
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

/**
 * A reusable banner that displays the current QFieldCloud service status.
 * \ingroup qml_app
 */
QfCollapsibleMessage {
  id: statusBanner

  property QfCloudStatus cloudServiceStatus: null

  visible: cloudServiceStatus && cloudServiceStatus.hasProblem

  Layout.fillWidth: true
  Layout.minimumHeight: visible ? height : 0

  color: {
    if (!cloudServiceStatus)
      return QfTheme.secondaryTextColor;
    switch (cloudServiceStatus.statusType) {
    case QfCloudStatus.Incident:
      return QfTheme.errorColor;
    case QfCloudStatus.Degraded:
      return QfTheme.warningColor;
    case QfCloudStatus.Maintenance:
      return QfTheme.cloudColor;
    default:
      return QfTheme.secondaryTextColor;
    }
  }

  detailsColor: QfTheme.secondaryTextColor
  font: QfTheme.tipFont
  iconSource: QfTheme.getThemeVectorIcon('ic_error_outline_24dp')
  titleText: cloudServiceStatus ? cloudServiceStatus.statusMessage : ''
  detailsText: cloudServiceStatus ? cloudServiceStatus.detailsMessage : ''
  externalLink: cloudServiceStatus ? cloudServiceStatus.statusPageUrl : ''
}
