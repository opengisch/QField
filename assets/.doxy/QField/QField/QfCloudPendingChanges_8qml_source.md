

# File QfCloudPendingChanges.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfCloudPendingChanges.qml**](QfCloudPendingChanges_8qml.md)

[Go to the documentation of this file](QfCloudPendingChanges_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: pendingChanges

  property alias deltaFileWrapper: deltaChangesModel.deltaFileWrapper

  function deltaMethodColor(method) {
    switch (method) {
    case QfDeltaChangesModel.CreateMethod:
      return QfTheme.mainColor;
    case QfDeltaChangesModel.PatchMethod:
      return QfTheme.secondaryTextColor;
    case QfDeltaChangesModel.DeleteMethod:
      return QfTheme.errorColor;
    default:
      return QfTheme.secondaryTextColor;
    }
  }

  function deltaMethodBackgroundColor(method) {
    switch (method) {
    case QfDeltaChangesModel.CreateMethod:
      return Qt.rgba(QfTheme.mainColor.r, QfTheme.mainColor.g, QfTheme.mainColor.b, 0.12);
    case QfDeltaChangesModel.PatchMethod:
      return QfTheme.groupBoxBackgroundColor;
    case QfDeltaChangesModel.DeleteMethod:
      return Qt.rgba(QfTheme.errorColor.r, QfTheme.errorColor.g, QfTheme.errorColor.b, 0.12);
    default:
      return QfTheme.groupBoxBackgroundColor;
    }
  }

  function deltaMethodLabel(method) {
    switch (method) {
    case QfDeltaChangesModel.CreateMethod:
      return qsTr('added');
    case QfDeltaChangesModel.PatchMethod:
      return qsTr('edited');
    case QfDeltaChangesModel.DeleteMethod:
      return qsTr('deleted');
    default:
      return '';
    }
  }

  function layerGeometryIcon(geometryType) {
    switch (geometryType) {
    case Qgis.GeometryType.Point:
      return QfTheme.getThemeVectorIcon('ic_geometry_point_24dp');
    case Qgis.GeometryType.Line:
      return QfTheme.getThemeVectorIcon('ic_geometry_line_24dp');
    case Qgis.GeometryType.Polygon:
      return QfTheme.getThemeVectorIcon('ic_geometry_polygon_24dp');
    default:
      return QfTheme.getThemeVectorIcon('ic_info_white_24dp');
    }
  }

  function attributeValueText(value) {
    if (value === '') {
      return qsTr('Empty');
    }
    return QfFeatureUtils.attributeIsNull(value) ? qsTr('NULL') : String(value);
  }

  function attributeChangeText(change, method) {
    switch (method) {
    case QfDeltaChangesModel.PatchMethod:
      return qsTr('%1: %2 to %3').arg(change.name).arg(attributeValueText(change.oldValue)).arg(attributeValueText(change.newValue));
    case QfDeltaChangesModel.DeleteMethod:
      return qsTr('%1: %2').arg(change.name).arg(attributeValueText(change.oldValue));
    default:
      return qsTr('%1: %2').arg(change.name).arg(attributeValueText(change.newValue));
    }
  }

  color: QfTheme.controlBackgroundColor
  border.color: QfTheme.controlBorderColor
  border.width: 1

  ListView {
    id: pendingChangesList
    anchors.fill: parent
    anchors.margins: 1
    clip: true
    ScrollBar.vertical: QfScrollBar {}

    model: QfDeltaChangesModel {
      id: deltaChangesModel
    }

    section.property: "LayerId"
    section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
    section.delegate: Rectangle {
      id: layerHeader

      required property string section

      readonly property string layerName: deltaChangesModel.layerName(layerHeader.section)
      readonly property int layerGeometryType: deltaChangesModel.layerGeometryType(layerHeader.section)
      readonly property int layerChangesCount: deltaChangesModel.layerChangesCount(layerHeader.section)

      width: pendingChangesList.width
      height: 30
      color: QfTheme.controlBorderColor

      RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 10

        QfToolButton {
          Layout.alignment: Qt.AlignVCenter
          Layout.preferredWidth: 20
          Layout.preferredHeight: 20
          padding: 0
          icon.width: 20
          icon.height: 20
          enabled: false
          bgcolor: "transparent"
          iconSource: pendingChanges.layerGeometryIcon(layerHeader.layerGeometryType)
          iconColor: QfTheme.secondaryTextColor
        }

        Text {
          Layout.fillWidth: true
          font: QfTheme.tipFont
          color: QfTheme.mainTextColor
          elide: Text.ElideRight
          text: layerHeader.layerName
        }

        Text {
          Layout.alignment: Qt.AlignVCenter
          font: QfTheme.tinyFont
          color: QfTheme.secondaryTextColor
          text: qsTr('%n change(s)', '', layerHeader.layerChangesCount)
        }
      }
    }

    delegate: Rectangle {
      id: featureRow

      readonly property int deltaMethod: Method
      readonly property color badgeColor: pendingChanges.deltaMethodColor(featureRow.deltaMethod)
      readonly property color badgeBackgroundColor: pendingChanges.deltaMethodBackgroundColor(featureRow.deltaMethod)
      readonly property bool hasDetails: AttributeChanges.length > 0 || HasGeometryChange
      property bool expanded: false

      width: pendingChangesList.width
      height: featureHeader.height + (featureRow.expanded ? featureDetails.height + 10 : 0)
      color: "transparent"
      clip: true

      Behavior on height {
        NumberAnimation {
          duration: 150
          easing.type: Easing.OutQuad
        }
      }

      Item {
        id: featureHeader
        width: parent.width
        height: Math.max(48, featureHeaderRow.implicitHeight)

        RowLayout {
          id: featureHeaderRow
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.leftMargin: 10
          anchors.rightMargin: 10
          anchors.verticalCenter: parent.verticalCenter
          spacing: 10

          Text {
            Layout.fillWidth: true
            font: QfTheme.defaultFont
            color: QfTheme.mainTextColor
            elide: Text.ElideRight
            text: FeatureName
          }

          Rectangle {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: methodBadgeLabel.implicitWidth + 10
            Layout.preferredHeight: methodBadgeLabel.implicitHeight + 5
            radius: 4
            color: featureRow.badgeBackgroundColor

            Text {
              id: methodBadgeLabel
              anchors.centerIn: parent
              font: QfTheme.tinyFont
              color: featureRow.badgeColor
              text: pendingChanges.deltaMethodLabel(featureRow.deltaMethod)
            }
          }

          QfToolButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            padding: 0
            icon.width: 20
            icon.height: 20
            visible: featureRow.hasDetails
            enabled: false
            bgcolor: "transparent"
            iconSource: QfTheme.getThemeVectorIcon(featureRow.expanded ? 'ic_chevron_up' : 'ic_chevron_down')
            iconColor: QfTheme.secondaryTextColor
          }
        }

        MouseArea {
          anchors.fill: parent
          enabled: featureRow.hasDetails
          onClicked: featureRow.expanded = !featureRow.expanded
        }
      }

      Column {
        id: featureDetails
        anchors.top: featureHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 5

        Repeater {
          model: AttributeChanges

          Text {
            width: featureDetails.width
            font: QfTheme.tinyFont
            color: QfTheme.secondaryTextColor
            wrapMode: Text.WordWrap
            text: pendingChanges.attributeChangeText(modelData, featureRow.deltaMethod)
          }
        }

        Text {
          width: featureDetails.width
          visible: HasGeometryChange
          font: QfTheme.tinyFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
          text: qsTr('Geometry modified')
        }
      }

      Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1
        color: QfTheme.controlBorderColor
        visible: featureRow.ListView.section === featureRow.ListView.nextSection
      }
    }
  }
}
```


