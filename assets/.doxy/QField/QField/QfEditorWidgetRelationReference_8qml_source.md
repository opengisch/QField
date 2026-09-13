

# File QfEditorWidgetRelationReference.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_aa4aab3cdee284f0e217d9df55b13787.md) **>** [**QfEditorWidgetRelationReference.qml**](QfEditorWidgetRelationReference_8qml.md)

[Go to the documentation of this file](QfEditorWidgetRelationReference_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui
import org.qgis

QfEditorWidgetBase {
  id: relationReferenceEditorWidget

  anchors {
    left: parent.left
    right: parent.right
  }
  height: Math.max(48, relationReference.height) + 4

  property bool showOpenFormButton: config['ShowOpenFormButton'] === undefined || config['ShowOpenFormButton'] === true
  property var _rel: QfRelationUtils.resolveReferencingRelation(qgisProject, currentLayer, field.name, config['Relation'])

  QfFeatureCheckListModel {
    id: listModel

    currentLayer: _rel.referencedLayer
    keyField: _rel.resolveReferencedField(field.name)
    addNull: !!config['AllowNULL'] // no, it is not a misspelled version of config['AllowNull']
    orderByValue: true
    attributeField: field
    currentFormFeature: currentFeature
    appExpressionContextScopesGenerator: appScopesGenerator
    filterExpression: config['FilterExpression'] ? config['FilterExpression'] : ""
    allowMulti: false

    // passing "" instead of undefined, so the model is cleared on adding new features
    // attributeValue has to be the last one set to make sure the property’s value is handled properly (e.g. allow multiple)
    attributeValue: value !== undefined ? value : ''

    onListUpdated: {
      valueChangeRequested(attributeValue, false);
    }
  }

  RowLayout {
    id: relationReferenceEditorWidgetLayout
    anchors {
      left: parent.left
      right: parent.right
    }
    spacing: 5

    QfRelationCombobox {
      id: relationReference

      Layout.fillWidth: true
      enabled: isEnabled
      featureListModel: listModel
      useSearch: false
      allowAddFeature: config['AllowAddFeatures'] !== undefined && config['AllowAddFeatures'] === true
      relation: _rel

      onRequestJumpToPoint: function (center, scale, handleMargins) {
        relationReferenceEditorWidget.requestJumpToPoint(center, scale, handleMargins);
      }
    }

    QfToolButton {
      id: viewButton

      property bool isVisible: listModel.currentLayer !== undefined && listModel.currentLayer !== null && listModel.currentLayer.geometryType() !== Qgis.GeometryType.Unknown && listModel.currentLayer.geometryType() !== Qgis.GeometryType.Null

      Layout.alignment: Qt.AlignTop
      visible: isVisible
      enabled: relationReference.currentKeyValue !== undefined && relationReference.currentKeyValue !== ''
      width: isVisible && enabled ? QfTheme.toolButtonSize : 0
      height: QfTheme.toolButtonSize

      iconSource: QfTheme.getThemeVectorIcon("ic_view_black_24dp")
      iconColor: QfTheme.mainTextColor
      bgcolor: "transparent"

      onClicked: {
        if (listModel.currentLayer !== undefined) {
          var feature = listModel.getFeatureFromKeyValue(relationReference.currentKeyValue);
          geometryHighlighter.geometryWrapper.qgsGeometry = feature.geometry;
          geometryHighlighter.geometryWrapper.crs = listModel.currentLayer.crs;
          const extentRect = QfFeatureUtils.extent(mapCanvas.mapSettings, listModel.currentLayer, feature);
          const scale = mapCanvas.mapSettings.computeScaleForExtent(extentRect, true);
          requestJumpToPoint(extentRect.center, scale, true);
        }
      }
    }

    QfToolButton {
      id: openFormButton

      Layout.alignment: Qt.AlignTop
      enabled: showOpenFormButton && relationReference.currentKeyValue !== undefined && relationReference.currentKeyValue !== ''
      width: enabled ? QfTheme.toolButtonSize : 0
      height: QfTheme.toolButtonSize

      iconSource: isEnabled ? QfTheme.getThemeVectorIcon('ic_edit_attributes_white_24dp') : QfTheme.getThemeVectorIcon('ic_baseline-list_white_24dp')
      iconColor: QfTheme.mainTextColor
      bgcolor: "transparent"

      onClicked: {
        if (relationReference.currentKeyValue !== undefined && relationReference.currentKeyValue !== '') {
          relationReference.ensureEmbeddedFormLoaded();
          relationReference.embeddedFeatureForm.state = isEnabled ? 'Edit' : 'ReadOnly';
          relationReference.embeddedFeatureForm.currentLayer = listModel.currentLayer;
          relationReference.embeddedFeatureForm.feature = listModel.getFeatureFromKeyValue(relationReference.currentKeyValue);
          relationReference.embeddedFeatureForm.open();
        }
      }
    }
  }
}
```


