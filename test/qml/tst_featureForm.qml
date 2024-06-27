import QtQuick 2.3
import QtTest 1.0

import org.qfield 1.0
import Theme 1.0

import "../../src/qml/" as QFieldControls

TestCase {
  name: "FeatureForm"


  QFieldControls.FeatureForm {
    id: featureForm
    property real testValue: 20
    property var mainWindow: item
    model: tmpModel

    AttributeFormModel{
      id: tmpModel
    }

    Item {
      id: item
    }
  }

  function test_01_featureForm() {
    compare(featureForm.testValue, 20)
  }
}
