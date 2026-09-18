import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
Rectangle {
  id: digitizingLayerInformationView

  property bool isDigitizing: false
  property VectorLayer activeDigitizingLayer
  property QfMarkupCollection activeDigitizingCollection

  width: parent.width
  height: childrenRect.height + 10
  color: QfTheme.mainBackgroundColorSemiOpaque
  radius: 8
  clip: true

  RowLayout {
    width: parent.width - 10
    height: childrenRect.height
    anchors.centerIn: parent

    QfComboBox {
      id: digitizingLayerComboBox

      property bool skipCurrentIndexChange: false

      Layout.fillWidth: true

      model: QfDigitizingLayerModel {
        id: digitizingLayerModel
        enabled: true
        project: qgisProject
        markups: markupManager
      }

      textRole: "Name"

      font: Theme.tipFont

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
