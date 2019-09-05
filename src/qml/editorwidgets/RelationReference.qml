import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

RelationCombobox {
  id: relationReference

  signal valueChanged(var value, bool isNull)

  property var _relation: qgisProject.relationManager.relation(config['Relation'])

  FeatureListModel {
    id: featureListModel

    currentLayer: qgisProject.relationManager.relation(config['Relation']).referencedLayer
    keyField: qgisProject.relationManager.relation(config['Relation']).resolveReferencedField(field.name)
    addNull: config['AllowNULL']
    orderByValue: config['OrderByValue']
  }
}
