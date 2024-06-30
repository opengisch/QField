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
    property var mainWindow: mainWindowItem

    model: AttributeFormModel {
      featureModel: FeatureModel {
        project: qgisProject
        currentLayer: qgisProject.mapLayersByName('Apiary')[0]
        // feature: qgisProject.mapLayersByName('Apiary')[0].getFeature(FEATURE_ID)
        // features: featureFormList.selection.model.selectedFeatures
        // cloudUserInformation: projectInfo.cloudUserInformation
      }
    }

    Component.onCompleted: {
      console.log("1 ==>", qgisProject)
      console.log("2 ==>", qgisProject.mapLayersByName('Apiary'))
      console.log("3 ==>", getMethods(qgisProject.mapLayersByName('Apiary')[0]))
    }
  }

  function test_01_featureForm() {
    console.log(featureForm.model)
    console.log(featureForm.model.rowCount())

    console.log("------------------------------------------")
    console.log("- ", qgisProject.mapLayersByName('Apiary'))
    console.log("- ", qgisProject.mapLayersByName('Tracks'))
    console.log("- ", qgisProject.mapLayersByName('Fields'))
    console.log("- ", qgisProject.mapLayersByName('Basemaps'))
    console.log("- ", qgisProject.mapLayersByName('Tables'))
    console.log("------------------------------------------")
    console.log("------------------------------------------")
    console.log("- ", qgisProject.mapLayersByName('Apiary')[0].getFeatures())
    // console.log("- ", qgisProject.mapLayersByName('Tracks').getFeatures())
    // console.log("- ", qgisProject.mapLayersByName('Fields').getFeatures())
    console.log("------------------------------------------")
    compare(featureForm.model.rowCount(), 6)
  }
}
