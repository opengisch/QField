

# File point.qml

[**File List**](files.md) **>** [**cogoparameterwidgets**](dir_a00d444b399c5bcccd8697d65fbda43e.md) **>** [**point.qml**](point_8qml.md)

[Go to the documentation of this file](point_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme
import org.qgis
import org.qfield

CogoParameterWidgetBase {
  id: pointParameter

  property var positionInformation: undefined

  width: parent.width
  height: childrenRect.height

  GridLayout {
    anchors {
      left: parent.left
      right: parent.right
    }
    columns: 4

    RowLayout {
      Layout.fillWidth: true
      Layout.columnSpan: 4
      Rectangle {
        visible: parameterColor != "#00000000"
        width: 10
        height: 10
        radius: 5
        color: parameterColor
      }

      Label {
        color: Theme.mainTextColor
        font: Theme.tipFont
        text: parameterLabel
      }

      Label {
        color: Theme.secondaryTextColor
        font: Theme.tipFont
        text: {
          let content = '';
          if (mapSettings.destinationCrs.isGeographic) {
            content += qsTr("latitude") + ' ' + qsTr("longitude");
            if (!!parameterConfiguration["hasZ"]) {
              content += ' ' + qsTr("elevation");
            }
          } else {
            content += qsTr("easting [X]") + ' ' + qsTr("northing [Y]");
            if (!!parameterConfiguration["hasZ"]) {
              content += ' ' + qsTr("elevation [Z]");
            }
          }
          return content;
        }
      }
    }

    QfTextField {
      id: pointField
      Layout.fillWidth: true
      font: Theme.tipFont

      onTextEdited: {
        processValue();
      }
    }

    QfToolButton {
      iconSource: Theme.getThemeVectorIcon("ic_list_black_24dp")
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"
      round: true

      onClicked: {
        mainWindow.closeToast();
        pointPickerLoader.active = true;
      }
    }

    QfToolButton {
      iconSource: Theme.getThemeVectorIcon("ic_coordinate_locator_white_24dp")
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"
      round: true

      onClicked: {
        requestPosition(pointParameter, true);
      }
    }

    QfToolButton {
      iconSource: Theme.getThemeVectorIcon("ic_location_valid_white_24dp")
      iconColor: Theme.positionColor
      bgcolor: "transparent"
      round: true

      onClicked: {
        requestPosition(pointParameter, false);
      }
    }
  }

  Loader {
    id: pointPickerLoader
    active: false

    sourceComponent: QfPopup {
      id: pointPickerPopup
      readonly property int minimumHeight: mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)

      parent: mainWindow.contentItem
      width: mainWindow.width - Theme.popupScreenEdgeHorizontalMargin * 2
      height: minimumHeight > 0 ? minimumHeight : 200
      x: Theme.popupScreenEdgeHorizontalMargin
      y: (mainWindow.height - height) / 2
      z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes
      closePolicy: Popup.CloseOnEscape
      visible: true
      focus: visible

      onAboutToShow: {
        searchBar.clear();
      }

      onClosed: {}

      Page {
        anchors.fill: parent
        padding: 5

        header: QfPageHeader {
          title: qsTr("Point Feature Picker")
          showBackButton: false
          showApplyButton: false
          showCancelButton: true
          onCancel: {
            pointPickerLoader.active = false;
          }
        }

        ColumnLayout {
          anchors.fill: parent

          QfComboBox {
            id: layersComboBox
            Layout.fillWidth: true
            model: MapLayerModel {
              id: layersModel
              enabled: true
              project: qgisProject
              filters: Qgis.LayerFilters.PointLayer
            }

            textRole: "Name"
            valueRole: "LayerPointer"
          }

          QfSearchBar {
            id: searchBar
            Layout.fillWidth: true
          }

          ListView {
            id: pointsList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: FeatureCheckListModel {
              id: featureListModel
              searchTerm: searchBar.searchTerm
              currentLayer: layersComboBox.currentValue
            }

            width: parent.width
            height: pointPickerPopup.height - layersComboBox.height - searchBar.height - 50
            clip: true
            ScrollBar.vertical: QfScrollBar {}

            delegate: Rectangle {
              id: rectangle

              property int fid: featureId
              property string itemText: StringUtils.highlightText(displayString, featureListModel.searchTerm, Theme.mainTextColor)

              anchors.margins: 10
              width: parent ? parent.width : undefined
              height: line.height + 20
              color: pointPickerPopup.Material ? pointPickerPopup.Material.dialogColor : Theme.mainBackgroundColor

              Row {
                id: line
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5

                Text {
                  id: contentText
                  width: rectangle.width - 10
                  anchors.verticalCenter: parent.verticalCenter
                  leftPadding: 5
                  font: Theme.defaultFont
                  elide: Text.ElideRight
                  wrapMode: Text.WordWrap
                  color: featureListModel.searchTerm != '' ? Theme.secondaryTextColor : Theme.mainTextColor
                  textFormat: Text.RichText
                  text: itemText
                }
              }

              Rectangle {
                anchors.bottom: parent.bottom
                height: 1
                color: Theme.controlBorderColor
                width: parent.width
              }
            }

            MouseArea {
              anchors.fill: parent
              propagateComposedEvents: true

              onClicked: mouse => {
                const item = pointsList.itemAt(pointsList.contentX + mouse.x, pointsList.contentY + mouse.y);
                if (!item) {
                  return;
                }
                const feature = featureListModel.currentLayer.getFeature(item.fid);
                if (feature) {
                  let point = GeometryUtils.centroid(feature.geometry);
                  point = GeometryUtils.reprojectPoint(point, featureListModel.currentLayer.crs, mapSettings.destinationCrs);
                  applyPoint(point);
                }
                pointPickerLoader.active = false;
              }
            }

            onMovementStarted: {
              Qt.inputMethod.hide();
            }
          }
        }
      }
    }
  }

  function processValue() {
    const parts = pointField.text.split(" ").filter(Boolean);
    if (parts.length === 2 || parts.length === 3) {
      const isGeographic = destinationCrs.isGeographic;
      const x = parseFloat(parts[isGeographic ? 1 : 0]);
      const y = parseFloat(parts[isGeographic ? 0 : 1]);
      const z = parts.length === 3 ? parseFloat(parts[3]) : NaN;
      if (!isNaN(x) && !isNaN(y)) {
        valueChangeRequested(GeometryUtils.point(x, y, z));
        return;
      }
    }
    valueChangeRequested(undefined);
  }

  function applyPoint(point) {
    if (!isNaN(point.x)) {
      const isGeographic = destinationCrs.isGeographic;
      const precision = isGeographic ? 7 : 3;
      let content = isGeographic ? point.y.toFixed(precision) + ' ' + point.x.toFixed(precision) : point.x.toFixed(precision) + ' ' + point.y.toFixed(precision);
      if (!!parameterConfiguration["hasZ"]) {
        if (!isNaN(point.z)) {
          content += ' ' + point.z.toFixed(3);
        }
      }
      pointField.text = content;
    } else {
      pointField.text = '';
    }
    processValue();
    pointField.cursorPosition = 0;
  }

  function requestedPositionReceived(position, positionInformation) {
    applyPoint(position);
    pointParameter.positionInformation = positionInformation;
    cogoOperationSettings.requestedPositionReceived(parameterName, position, positionInformation);
  }
}
```


