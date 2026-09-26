

# File QfDigitizingLayerInformationView.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfDigitizingLayerInformationView.qml**](QfDigitizingLayerInformationView_8qml.md)

[Go to the documentation of this file](QfDigitizingLayerInformationView_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: digitizingLayerInformationView

  property bool allowActiveDigitizingLayerChange: true
  property VectorLayer activeDigitizingLayer
  property QfMarkupCollection activeDigitizingCollection

  width: parent.width
  height: digitizingLayerInformationRow.height + 10
  color: QfTheme.mainBackgroundColorSemiOpaque
  radius: 8
  clip: true

  RowLayout {
    id: digitizingLayerInformationRow
    width: parent.width - 10
    height: childrenRect.height
    anchors.centerIn: parent

    Rectangle {
      Layout.fillWidth: true
      Layout.preferredHeight: digitizingLayerComboBox.height

      color: QfTheme.mainBackgroundColorSemiOpaque

      QfComboBox {
        id: digitizingLayerComboBox

        property bool skipCurrentIndexChange: false

        width: parent.width

        model: QfDigitizingLayerModel {
          id: digitizingLayerModel
          enabled: true
          project: qgisProject
          markups: markupManager
        }
        textRole: "Name"

        font: QfTheme.tipFont

        indicator.visible: digitizingLayerInformationView.allowActiveDigitizingLayerChange

        background: MaterialTextContainer {
          implicitWidth: 120
          implicitHeight: digitizingLayerComboBox.Material.textFieldHeight

          filled: false
          outlineColor: "transparent"
          focusedOutlineColor: "transparent"
          controlHasActiveFocus: false
          controlHasText: true
          horizontalPadding: digitizingLayerComboBox.Material.textFieldHorizontalPadding
        }

        popup: Popup {
          id: digitizingLayerPopup
          width: digitizingLayerComboBox.width
          implicitHeight: contentItem.implicitHeight + 2
          topMargin: mainWindow.sceneTopMargin
          bottomMargin: mainWindow.sceneTopMargin
          padding: 1
          font: QfTheme.tipFont

          onAboutToShow: {
            contentItem.model = digitizingLayerComboBox.delegateModel;
          }

          onAboutToHide: {
            contentItem.model = null;
          }

          background: Rectangle {
            radius: digitizingLayerPopup.Material.roundedScale
            color: QfTheme.controlBackgroundColor

            layer.enabled: digitizingLayerPopup.Material.elevation > 0
            layer.effect: RoundedElevationEffect {
              elevation: digitizingLayerPopup.Material.elevation
              roundedScale: digitizingLayerPopup.Material.roundedScale
            }
          }

          contentItem: ListView {
            clip: true
            implicitHeight: Math.min(mainWindow.height - mainWindow.sceneTopMargin - mainWindow.sceneTopMargin, contentHeight)
            currentIndex: digitizingLayerComboBox.highlightedIndex

            section.property: "LayerType"
            section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
            section.delegate: Component {
              Rectangle {
                width: parent.width
                height: 30
                color: QfTheme.controlBorderColor

                Text {
                  anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                  }
                  font: QfTheme.strongResultFont
                  color: QfTheme.mainTextColor
                  text: {
                    if (section == "MapLayer") {
                      return qsTr("Map Layers");
                    } else if (section == "MarkupCollection") {
                      return qsTr("Markups");
                    }
                    return '';
                  }
                }
              }
            }
            ScrollIndicator.vertical: ScrollIndicator {}
          }
        }

        delegate: ItemDelegate {
          width: digitizingLayerComboBox.width
          height: 36
          icon.source: {
            switch (LayerType) {
            case QfDigitizingLayerModel.MapLayer:
              switch (GeometryType) {
              case Qgis.GeometryType.Point:
                return QfTheme.getThemeVectorIcon('ic_geometry_point_18dp');
              case Qgis.GeometryType.Line:
                return QfTheme.getThemeVectorIcon('ic_geometry_line_18dp');
              case Qgis.GeometryType.Polygon:
                return QfTheme.getThemeVectorIcon('ic_geometry_polygon_18dp');
              case Qgis.GeometryType.Null:
              case Qgis.GeometryType.Unknown:
                return QfTheme.getThemeVectorIcon('ic_table_18dp');
              }
              return '';
            case QfDigitizingLayerModel.MarkupCollection:
              return QfTheme.getThemeVectorIcon('ic_markup_collection_18dp');
            }

            return '';
          }
          icon.width: 16
          icon.height: 16
          icon.color: QfTheme.mainTextColor
          text: Name
          font: QfTheme.tipFont
          highlighted: digitizingLayerComboBox.highlightedIndex === index
        }

        contentItem: MenuItem {
          width: digitizingLayerComboBox.width
          height: 36

          icon.source: {
            const digitizingLayerDetails = digitizingLayerModel.get(digitizingLayerComboBox.currentIndex);
            switch (digitizingLayerDetails["LayerType"]) {
            case QfDigitizingLayerModel.MapLayer:
              switch (digitizingLayerDetails["GeometryType"]) {
              case Qgis.GeometryType.Point:
                return QfTheme.getThemeVectorIcon('ic_geometry_point_18dp');
              case Qgis.GeometryType.Line:
                return QfTheme.getThemeVectorIcon('ic_geometry_line_18dp');
              case Qgis.GeometryType.Polygon:
                return QfTheme.getThemeVectorIcon('ic_geometry_polygon_18dp');
              case Qgis.GeometryType.Null:
              case Qgis.GeometryType.Unknown:
                return QfTheme.getThemeVectorIcon('ic_table_18dp');
              }
              return '';
            case QfDigitizingLayerModel.MarkupCollection:
              return QfTheme.getThemeVectorIcon('ic_markup_collection_18dp');
            }
            return '';
          }
          icon.width: 16
          icon.height: 16
          icon.color: QfTheme.mainTextColor

          text: digitizingLayerComboBox.currentText
          font: QfTheme.tipFont

          onClicked: if (digitizingLayerInformationView.allowActiveDigitizingLayerChange) {
            digitizingLayerComboBox.popup.open();
          }
        }

        onCurrentIndexChanged: {
          if (skipCurrentIndexChange) {
            skipCurrentIndexChange = false;
            return;
          }

          if (currentIndex !== -1) {
            const digitizingLayerDetails = digitizingLayerModel.get(currentIndex);
            switch (digitizingLayerDetails["LayerType"]) {
            case QfDigitizingLayerModel.MapLayer:
              activeDigitizingCollection = null;
              activeDigitizingLayer = digitizingLayerDetails["LayerPointer"];
              break;
            case QfDigitizingLayerModel.MarkupCollection:
              activeDigitizingLayer = null;
              activeDigitizingCollection = digitizingLayerDetails["LayerPointer"];
              break;
            }
          }
        }
      }
    }
  }

  onActiveDigitizingLayerChanged: {
    if (activeDigitizingLayer) {
      const idx = digitizingLayerModel.findLayer(activeDigitizingLayer);
      if (digitizingLayerComboBox.currentIndex !== idx) {
        digitizingLayerComboBox.skipCurrentIndexChange = true;
        digitizingLayerComboBox.currentIndex = idx;
      }
    }
  }

  onActiveDigitizingCollectionChanged: {
    if (activeDigitizingCollection) {
      const idx = digitizingLayerModel.findCollection(activeDigitizingCollection);
      if (digitizingLayerComboBox.currentIndex !== idx) {
        digitizingLayerComboBox.skipCurrentIndexChange = true;
        digitizingLayerComboBox.currentIndex = idx;
      }
    }
  }
}
```


