import QtQuick
import QtTest
import org.qfield.core
import org.qfield.app

TestCase {
  name: "QfCloudPendingChanges"
  when: windowShown

  QfCloudPendingChanges {
    id: pendingChanges
    width: 400
    height: 600
  }

  function test_01_idleWithoutDeltaFileWrapper() {
    verify(!pendingChanges.deltaFileWrapper);
  }

  function test_02_attributeChangeValueText() {
    compare(pendingChanges.attributeChangeValueText(null), "NULL");
    compare(pendingChanges.attributeChangeValueText(undefined), "NULL");
    // the feature form tells an unset attribute apart from a cleared one, so does this
    compare(pendingChanges.attributeChangeValueText(""), "Empty");
    compare(pendingChanges.attributeChangeValueText("stringy"), "stringy");
    compare(pendingChanges.attributeChangeValueText(42), "42");
    // a zero is a value, not an absent one
    compare(pendingChanges.attributeChangeValueText(0), "0");
    compare(pendingChanges.attributeChangeValueText(false), "false");
  }

  function test_03_deltaMethodLabels() {
    compare(pendingChanges.deltaMethodLabel(QfDeltaChangesModel.CreateMethod), "added");
    compare(pendingChanges.deltaMethodLabel(QfDeltaChangesModel.PatchMethod), "edited");
    compare(pendingChanges.deltaMethodLabel(QfDeltaChangesModel.DeleteMethod), "deleted");
    compare(pendingChanges.deltaMethodLabel(QfDeltaChangesModel.UnknownMethod), "");
  }
}
