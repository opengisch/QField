

# File QfCloudStatusBanner.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfCloudStatusBanner.qml**](QfCloudStatusBanner_8qml.md)

[Go to the documentation of this file](QfCloudStatusBanner_8qml.md)


```C++
import QtQuick
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

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
```


