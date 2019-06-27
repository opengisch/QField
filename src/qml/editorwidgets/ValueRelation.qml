import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

RelationCombobox {
  id: valueRelation
  property var _relation: undefined

  FeatureListModel {
    id: featureListModel

    currentLayer: qgisProject.mapLayer(config['Layer'])
    keyField: config['Key']
    displayValueField: config['Value']
    addNull: config['AllowNULL']
    orderByValue: config['OrderByValue']
  }
}
