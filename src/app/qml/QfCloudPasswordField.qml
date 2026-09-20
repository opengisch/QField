import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_app
 */
ColumnLayout {
  id: cloudPasswordField

  property alias password: passwordField.text
  property alias placeholderText: passwordField.placeholderText
  property string username: ""

  readonly property bool hasMinimumLength: passwordField.text.length >= 8
  readonly property bool isDistinctFromUsername: passwordField.text.length > 0 && (username.length < 3 || passwordField.text.toLowerCase().indexOf(username.toLowerCase()) === -1)
  readonly property bool isNotEntirelyNumeric: passwordField.text.length > 0 && !/^\d+$/.test(passwordField.text)
  readonly property int satisfiedRequirementCount: (hasMinimumLength ? 1 : 0) + (isDistinctFromUsername ? 1 : 0) + (isNotEntirelyNumeric ? 1 : 0)

  signal accepted

  spacing: 10

  QfTextField {
    id: passwordField
    Layout.fillWidth: true
    echoMode: TextInput.Password
    inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
    placeholderText: qsTr("Password")

    Keys.onReturnPressed: cloudPasswordField.accepted()
  }

  RowLayout {
    Layout.fillWidth: true
    spacing: 10

    Repeater {
      model: 3

      Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 4
        radius: 2
        color: index < cloudPasswordField.satisfiedRequirementCount ? QfTheme.goodColor : QfTheme.controlBorderColor

        Behavior on color {
          ColorAnimation {
            duration: 200
          }
        }
      }
    }
  }

  Text {
    Layout.fillWidth: true
    text: qsTr("At least 8 characters")
    font: QfTheme.tipFont
    color: cloudPasswordField.hasMinimumLength ? QfTheme.goodColor : QfTheme.secondaryTextColor
    wrapMode: Text.WordWrap
  }

  Text {
    Layout.fillWidth: true
    text: qsTr("Not similar to your username")
    font: QfTheme.tipFont
    color: cloudPasswordField.isDistinctFromUsername ? QfTheme.goodColor : QfTheme.secondaryTextColor
    wrapMode: Text.WordWrap
  }

  Text {
    Layout.fillWidth: true
    text: qsTr("Not entirely numeric")
    font: QfTheme.tipFont
    color: cloudPasswordField.isNotEntirelyNumeric ? QfTheme.goodColor : QfTheme.secondaryTextColor
    wrapMode: Text.WordWrap
  }
}
