

# File QFieldCloudStatusBanner.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldCloudStatusBanner.qml**](QFieldCloudStatusBanner_8qml.md)

[Go to the documentation of this file](QFieldCloudStatusBanner_8qml.md)


```C++
import QtQuick
import QtQuick.Layouts
import org.qfield
import Theme

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
```


