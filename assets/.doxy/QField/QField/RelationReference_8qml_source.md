

# File RelationReference.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**RelationReference.qml**](RelationReference_8qml.md)

[Go to the documentation of this file](RelationReference_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import org.qgis
import Theme
import ".."

EditorWidgetBase {
  id: relationReferenceEditorWidgetBase
  height: childrenRect.height
  anchors {
    left: parent.left
    right: parent.right
  }

  property bool showOpenFormButton: config['ShowOpenFormButton'] === undefined || config['ShowOpenFormButton'] === true
  property var _rel: RelationUtils.resolveReferencingRelation(qgisProject, currentLayer, field.name, config['Relation'])

  FeatureCheckListModel {
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
    anchors {
      left: parent.left
      right: parent.right
      top: parent.top
    }
    spacing: 5

    RelationCombobox {
      id: relationReference
      featureListModel: listModel
      Layout.fillWidth: true

      enabled: isEnabled
      useSearch: false
      allowAddFeature: config['AllowAddFeatures'] !== undefined && config['AllowAddFeatures'] === true
      relation: _rel

      onRequestJumpToPoint: function (center, scale, handleMargins) {
        relationReferenceEditorWidgetBase.requestJumpToPoint(center, scale, handleMargins);
      }
    }

    QfToolButton {
      id: viewButton

      property bool isVisible: listModel.currentLayer !== undefined && listModel.currentLayer !== null && listModel.currentLayer.geometryType() !== Qgis.GeometryType.Unknown && listModel.currentLayer.geometryType() !== Qgis.GeometryType.Null
      visible: isVisible
      enabled: relationReference.currentKeyValue !== undefined && relationReference.currentKeyValue !== ''
      width: isVisible && enabled ? 48 : 0
      height: 48

      iconSource: Theme.getThemeVectorIcon("ic_view_black_24dp")
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"

      onClicked: {
        if (listModel.currentLayer !== undefined) {
          var feature = listModel.getFeatureFromKeyValue(relationReference.currentKeyValue);
          geometryHighlighter.geometryWrapper.qgsGeometry = feature.geometry;
          geometryHighlighter.geometryWrapper.crs = listModel.currentLayer.crs;
          const extentRect = FeatureUtils.extent(mapCanvas.mapSettings, listModel.currentLayer, feature);
          const scale = mapCanvas.mapSettings.computeScaleForExtent(extentRect, true);
          requestJumpToPoint(extentRect.center, scale, true);
        }
      }
    }

    QfToolButton {
      id: openFormButton

      enabled: showOpenFormButton && relationReference.currentKeyValue !== undefined && relationReference.currentKeyValue !== ''
      width: enabled ? 48 : 0
      height: 48

      iconSource: isEnabled ? Theme.getThemeVectorIcon('ic_edit_attributes_white_24dp') : Theme.getThemeVectorIcon('ic_baseline-list_white_24dp')
      iconColor: Theme.mainTextColor
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


