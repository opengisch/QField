import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

Frame{
    height: 100
    width: 400

    property var _relation

    Component.onCompleted: {
      _relation = qgisProject.relationManager.relation( relationId )
      referencedFeatureListModel.relation = _relation
      referencedFeatureListModel.featureId = AttributeValue
      referencedFeatureListModel.feedModel()
    }

    ListView {
        implicitHeight: 400
        implicitWidth: 100
        clip: true

        model: referencedFeatureListModel

        delegate: RowLayout {
            focus: true
            width: parent.width
            TextField { text: model.displayRole }
        }
    }
    ReferencedFeatureListModel{
        id: referencedFeatureListModel
    }
}

