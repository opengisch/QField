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

  QFieldControls.FeatureForm {
    id: featureForm
    property real testValue: 20
    property var mainWindow: mainWindowItem
    model: tmpModel

    AttributeFormModel{
      id: tmpModel
    }
  }

  function test_01_featureForm() {
    compare(featureForm.testValue, 20)
  }
}
