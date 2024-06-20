import QtQuick 2.3
import QtTest 1.0

import org.qfield 1.0

import Theme 1.0

import "../../src/qml/editorwidgets" as EditorWidgets

TestCase {
  name: "EditorWidgets"

  EditorWidgets.TextEdit {
    id: textEdit
    property string value: 'one'
    property var config: undefined
    property bool isEnabled: true
  }

  EditorWidgets.Range {
    id: range
    property real value: 1
    property var config: undefined
    property bool isEnabled: true
  }

  EditorWidgets.DateTime {
    id: dateTime
    property string value: '2022-01-01'
    property var config: undefined
    property bool isEnabled: true
  }

  function test_textEdit() {
    compare(textEdit.children[1].text, 'one')
    textEdit.value = 'two'
    compare(textEdit.children[1].text, 'two')
  }

  function test_range() {
    compare(range.children[0].children[0].text, '1')
    range.value = 2
    compare(range.children[0].children[0].text, '2')
  }
}
