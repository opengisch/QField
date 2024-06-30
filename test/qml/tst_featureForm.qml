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
        feature: qgisProject.mapLayersByName('Apiary')[0].getFeature("64")
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
    console.log("- ", qgisProject.mapLayersByName('Apiary')[0].getFeature("64"))
    // console.log("- ", qgisProject.mapLayersByName('Tracks').getFeatures())
    // console.log("- ", qgisProject.mapLayersByName('Fields').getFeatures())
    console.log("------------------------------------------")
    let mf = qgisProject.mapLayersByName('Apiary')[0].getFeature("64")
    console.log("=>", mf.attributes)
    console.log("->", mf.attribute("photo"))
    console.log("->", mf.attribute("beekeeper"))
    console.log("->", mf.attribute("number of boxes"))

    compare(featureForm.model.rowCount(), 6)
  }
}

// qgisProject.mapLayersByName('Apiary')[0].getFeature("64").attributes
// QVariant(
//   QgsAttributes,
// 	QList(
// 		QVariant(qlonglong, 64),
// 		QVariant(int, 7),
// 		QVariant(QString, "Apis Mellifera Carnica"),
// 		QVariant(QString, "1000"),
// 		QVariant(QString, "Stephen Hawking"),
// 		QVariant(QString, "DCIM/3.jpg"),
// 		QVariant(bool, false),
// 		QVariant(QString, ""),
// 		QVariant(int, 10),
// 		QVariant(int, 0),
// 		QVariant(QString, "{3180afb7-080d-4382-91fc-4a30c1ddd001}"),
// 		QVariant(QString, ""),
// 		QVariant(QString, ""),
// 		QVariant(double, 0),
// 		QVariant(double, 0),
// 		QVariant(double, 0),
// 		QVariant(double, 0),
// 		QVariant(int, 0),
// 		QVariant(QString, ""),
// 		QVariant(bool, false)
// 	)
// )
