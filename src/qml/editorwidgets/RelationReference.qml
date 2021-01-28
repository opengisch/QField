import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

import ".."
import "."

EditorWidgetBase {
  anchors { left: parent.left; right: parent.right; }

  property bool showOpenFormButton: config['ShowOpenFormButton'] === undefined || config['ShowOpenFormButton'] === true

  RelationCombobox {
    id: relationReference
    anchors { left: parent.left; right: parent.right; rightMargin: showOpenFormButton ? 24 : 0 }
    enabled: isEnabled
    useCompleter: true

    signal valueChanged(var value, bool isNull)

    property var _relation: qgisProject.relationManager.relation(config['Relation'])

    FeatureCheckListModel {
      id: featureListModel

      currentLayer: qgisProject.relationManager.relation(config['Relation']).referencedLayer
      keyField: qgisProject.relationManager.relation(config['Relation']).resolveReferencedField(field.name)
      // no, it is not a misspelled version of config['AllowNull']
      addNull: config['AllowNULL']
      orderByValue: config['OrderByValue']

      attributeField: field
      //passing "" instead of undefined, so the model is cleared on adding new features
      attributeValue: value !== undefined ? value : ''
      currentFormFeature: currentFeature
      filterExpression: ""
      allowMulti: false
      onListUpdated: {
        valueChanged( attributeValue, false )
      }
    }

    onValueChanged: parent.valueChanged(value, isNull)
  }

  Image {
    id: viewButton
    anchors { right: parent.right; top: relationReference.top; topMargin: relationReference.childrenRect.height / 2 - 9 }
    source: Theme.getThemeIcon("ic_view_green_48dp")
    enabled: showOpenFormButton && relationReference.currentKeyValue !== undefined && relationReference.currentKeyValue !== ''
    width: enabled ? 18 : 0
    height: 18

    MouseArea {
      anchors.fill: parent
      onClicked: {
        if ( relationReference.currentKeyValue !== undefined && relationReference.currentKeyValue !== '' ) {
          relationReference.embeddedFeatureForm.state = isEnabled ? 'Edit' : 'ReadOnly'
          relationReference.embeddedFeatureForm.currentLayer = featureListModel.currentLayer
          relationReference.embeddedFeatureForm.feature = featureListModel.getFeatureFromKeyValue( relationReference.currentKeyValue )
          relationReference.embeddedFeatureForm.open()
        }
      }
    }
  }
}
