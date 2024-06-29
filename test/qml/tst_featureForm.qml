import QtQuick 2.3
import QtTest 1.0

import org.qfield 1.0
import Theme 1.0

import "../../src/qml/" as QFieldControls

TestCase {
  name: "FeatureForm"


  Item {
    id: mainWindowItem
  }

  function getMethods(obj)
  {
      var res = [];
      for(var m in obj) {
          if(typeof obj[m] == "function") {
              res.push(m)
          }
      }
      return res;
  }

  QFieldControls.FeatureForm {
    id: featureForm
    property real testValue: 20
    property var mainWindow: mainWindowItem

    model: AttributeFormModel {
      featureModel: FeatureModel {
        project: qgisProject
        // currentLayer: qgisProject.mapLayersByName('Apiary')[0]
        // feature: qgisProject.mapLayersByName('Apiary')[0][0] // .getFeature(FEATURE_ID)
        // features: featureFormList.selection.model.selectedFeatures
        // cloudUserInformation: projectInfo.cloudUserInformation
      }
    }

    Component.onCompleted: {
      console.log("1 ==>", qgisProject)
      // console.log("2 ==>", qgisProject.mapLayer())
      console.log("3 ==>", qgisProject.mapLayersByName('Apiary'))
      console.log("4 ==>", qgisProject.mapLayersByName('Apiary')[0])
      mapLayersByShortName
    }
  }

  function test_01_featureForm() {
    compare(featureForm.testValue, 20)
  }
}
