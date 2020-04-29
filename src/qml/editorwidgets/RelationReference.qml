import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0
import ".."


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
