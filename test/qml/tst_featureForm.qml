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

  /**
   * Test case for featureForm (Apiary type).
   *
   * This function checks the visibility of the toolbar, the title label, and the tab row.
   * It also verifies the presence and values of various UI elements within each tab.
   *
   * Specifically, it checks:
   * - The toolbar visibility
   * - The title text
   * - The tab row count
   * - Each tab has a corresponding text and is currently selected
   * - The fields in the "General" tab are correctly populated with values
   */
  function test_01_featureForm() {
    const toolbar = Utils.findChildren(featureForm, "toolbar")
    compare(toolbar.visible, false)

    const titleLabel = Utils.findChildren(featureForm, "titleLabel")
    compare(titleLabel.text, "View feature on Apiary")

    const tabRow = Utils.findChildren(featureForm, "tabRow")
    compare(tabRow.model.hasTabs, true)
    compare(tabRow.model.rowCount(), 6)

    const tabs = ["General", "Picture", "Issues", "Review", "Consumption", "GNSS"]
    for (var i = 0; i < tabRow.model.rowCount(); ++i) {
      tabRow.currentIndex = i
      const delegate = Utils.findChildren(featureForm, "tabRowdDlegate_" + i)
      compare(delegate.text, tabs[i])
      compare(delegate.isCurrentIndex, true)
      compare(tabRow.currentIndex, i)
    }

    // test fields in tabRow.currentIndex = 0 ("General")
    const fieldItem = Utils.findChildren(featureForm, "fieldRepeater")
    const testModel = [{
                         "containerName": "Number of Boxes",
                         "widgetType": "Range",
                         "source": "editorwidgets/Range.qml",
                         "value": 7
                       }, {
                         "containerName": "Species of Bees",
                         "widgetType": "ValueMap",
                         "source": "editorwidgets/ValueMap.qml",
                         "value": "Apis Mellifera Carnica"
                       }, {
                         "containerName": "Amount of Bees",
                         "widgetType": "ValueMap",
                         "source": "editorwidgets/ValueMap.qml",
                         "value": "1000"
                       }, {
                         "containerName": "Beekeeper",
                         "widgetType": "TextEdit",
                         "source": "editorwidgets/TextEdit.qml",
                         "value": "Stephen Hawking"
                       }, {
                         "containerName": "Yearly Harvest (kg)",
                         "widgetType": "Range",
                         "source": "editorwidgets/Range.qml",
                         "value": 10
                       }]

    for (var j = 0; j < fieldItem.count; ++j) {
      const itemLoader = fieldItem.itemAt(j).children[2].children[0]
      const fieldContainer = fieldItem.itemAt(j).children[2].children[1]
      const attributeEditorLoader = Utils.findChildren(featureForm, "attributeEditorLoader" + itemLoader.containerName)
      const attributeConfig = attributeEditorLoader.config
      const initialValue = attributeEditorLoader.currentFeature.attribute(itemLoader.containerName)

      compare(itemLoader.containerName, testModel[j].containerName)
      compare(fieldContainer.children[0].text, testModel[j].containerName)
      compare(attributeEditorLoader.widget, testModel[j].widgetType)
      compare(attributeEditorLoader.source, testModel[j].source)
      compare(initialValue, testModel[j].value)
    }
  }
}
