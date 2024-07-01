import QtQuick 2.3
import QtTest 1.0

import org.qfield 1.0
import Theme 1.0

import "../../src/qml/" as QFieldControls
import "Utils.js" as Utils

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
    property var mSelectedLayer: qgisProject.mapLayersByName('Apiary')[0]
    property var mSelectedFeature: qgisProject.mapLayersByName('Apiary')[0].getFeature("64")

    model: AttributeFormModel {
      featureModel: FeatureModel {
        project: qgisProject
        currentLayer: featureForm.mSelectedLayer
        feature: featureForm.mSelectedFeature
        // features: featureFormList.selection.model.selectedFeatures
        // cloudUserInformation: projectInfo.cloudUserInformation
      }
    }
  }


  /**
   * Test function to verify that the qgisProject has been loaded and attributes are accessible.
   *
   * This function checks the initial values of features in three different layers:
   * - Apiary layer with id 64
   * - Tracks layer with id 1
   * - Field layer with id 39
   *
   * It verifies the attribute values for each feature in these layers, including
   * their FID, name, region, editor name, proprietor, photo, date of review, reviewer, and plants.
   */
  function test_00_featureForm() {
    // check initial value of an `Apiary` layer with id = 64
    const apiaryId64 = qgisProject.mapLayersByName('Apiary')[0].getFeature("64")
    compare(apiaryId64.attribute("fid"), 64)
    compare(apiaryId64.attribute("photo"), "DCIM/3.jpg")
    compare(apiaryId64.attribute("beekeeper"), "Stephen Hawking")
    compare(apiaryId64.attribute("Amount of Bees"), "1000")
    compare(apiaryId64.attribute("number of boxes"), 7)
    compare(apiaryId64.attribute("Species of Bees"), "Apis Mellifera Carnica")

    // check initial value of an `Tracks` layer with id 1
    const trackId1 = qgisProject.mapLayersByName('Tracks')[0].getFeature("1")
    compare(trackId1.attribute("fid"), 1)
    compare(trackId1.attribute("Track Name"), "Munt Sura")
    compare(trackId1.attribute("Region"), "")
    compare(trackId1.attribute("Editor Name"), "Linda Camathiias")

    // check initial value of an `Field` layer with id 39
    const reviewDate = new Date("2019-05-23T04:30:00.000")
    const fieldId1 = qgisProject.mapLayersByName('Fields')[0].getFeature("39")
    compare(fieldId1.attribute("fid"), 39)
    compare(fieldId1.attribute("Proprietor"), "national")
    compare(fieldId1.attribute("photo"), "DCIM/taraxacum.jpg")
    compare(fieldId1.attribute("Date of Review"), reviewDate)
    compare(fieldId1.attribute("Reviewer"), "David Signer")
    compare(fieldId1.attribute("Plants"), "taraxacum") // description: Dandelions
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
