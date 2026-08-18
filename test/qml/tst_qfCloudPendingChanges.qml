import QtQuick
import QtTest
import org.qfield.core
import org.qfield.app as QFieldControls

TestCase {
  name: "QfCloudPendingChanges"
  when: windowShown

  QFieldControls.QfCloudPendingChanges {
    id: pendingChanges
    width: 400
    height: 600
  }

  function test_01_idleWithoutDeltaFileWrapper() {
    verify(!pendingChanges.deltaFileWrapper);
  }

  function test_02_attributeValueText() {
    compare(pendingChanges.attributeValueText(null), "NULL");
    compare(pendingChanges.attributeValueText(undefined), "NULL");
    // the feature form tells an unset attribute apart from a cleared one, so does this
    compare(pendingChanges.attributeValueText(""), "Empty");
    compare(pendingChanges.attributeValueText("stringy"), "stringy");
    compare(pendingChanges.attributeValueText(42), "42");
    // a zero is a value, not an absent one
    compare(pendingChanges.attributeValueText(0), "0");
    compare(pendingChanges.attributeValueText(false), "false");
  }

  function test_03_attributeChangeText() {
    const change = {
      "name": "height",
      "oldValue": 10,
      "newValue": 12
    };

    compare(pendingChanges.attributeChangeText(change, QfPendingDeltaModel.PatchMethod), "height: 10 to 12");
    compare(pendingChanges.attributeChangeText(change, QfPendingDeltaModel.DeleteMethod), "height: 10");
    compare(pendingChanges.attributeChangeText(change, QfPendingDeltaModel.CreateMethod), "height: 12");

    const cleared = {
      "name": "name",
      "oldValue": "oak",
      "newValue": null
    };

    compare(pendingChanges.attributeChangeText(cleared, QfPendingDeltaModel.PatchMethod), "name: oak to NULL");
  }

  function test_04_deltaMethodLabels() {
    compare(pendingChanges.deltaMethodLabel(QfPendingDeltaModel.CreateMethod), "added");
    compare(pendingChanges.deltaMethodLabel(QfPendingDeltaModel.PatchMethod), "edited");
    compare(pendingChanges.deltaMethodLabel(QfPendingDeltaModel.DeleteMethod), "deleted");
    compare(pendingChanges.deltaMethodLabel(QfPendingDeltaModel.UnknownMethod), "");
  }
}
