import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_app
 */
Item {
  id: qfieldCloudPasswordReset

  // The account an emailed link was issued for, and whether the server still accepts it
  property alias currentStepIndex: stepView.currentIndex
  property string username: ""
  property bool isLinkValid: true

  property string email: ""
  property int resendCountdown: 0
  property int sentLinkCount: 0

  readonly property bool isEmailValid: /^[^@\s]+@[^@\s]+\.[^@\s]+$/.test(emailField.text)
  readonly property bool isRateLimited: sentLinkCount >= 3

  signal cancelled
  signal finished

  implicitHeight: passwordResetLayout.implicitHeight

  Material.accent: QfTheme.cloudColor

  ColumnLayout {
    id: passwordResetLayout
    x: 10
    width: parent.width - 20
    spacing: 10

    SwipeView {
      id: stepView
      objectName: "stepView"
      Layout.fillWidth: true
      Layout.topMargin: 10
      implicitHeight: currentItem ? currentItem.implicitHeight : 0
      clip: true
      interactive: false

      Behavior on implicitHeight {
        NumberAnimation {
          duration: 200
          easing.type: Easing.InOutQuad
        }
      }

      ColumnLayout {
        id: requestStep
        spacing: 10

        Text {
          Layout.fillWidth: true
          text: qsTr("Forgot your password?")
          font: QfTheme.strongTitleFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
        }

        Text {
          Layout.fillWidth: true
          text: qsTr("Enter the email you registered with and we will send you a link to pick a new password.")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }

        QfTextField {
          id: emailField
          objectName: "emailField"
          Layout.fillWidth: true
          Layout.topMargin: 10
          inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
          placeholderText: qsTr("Email")
          text: qfieldCloudPasswordReset.email

          onTextChanged: text = text.replace(/\s+/g, '')
          Keys.onReturnPressed: {
            if (sendResetLinkButton.enabled) {
              sendResetLinkButton.clicked();
            }
          }
        }

        Text {
          Layout.fillWidth: true
          visible: qfieldCloudPasswordReset.email !== "" && emailField.text === qfieldCloudPasswordReset.email
          text: qsTr("Carried over from the sign-in form.")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }

        Text {
          Layout.fillWidth: true
          visible: qfieldCloudPasswordReset.isRateLimited
          text: qsTr("Too many reset links requested. Wait a few minutes before trying again.")
          font: QfTheme.tipFont
          color: QfTheme.errorColor
          wrapMode: Text.WordWrap
        }

        QfButton {
          id: sendResetLinkButton
          objectName: "sendResetLinkButton"
          Layout.fillWidth: true
          Layout.topMargin: 10
          text: qsTr("Send reset link")
          enabled: qfieldCloudPasswordReset.isEmailValid && !qfieldCloudPasswordReset.isRateLimited
          bgcolor: QfTheme.cloudColor
          color: QfTheme.light

          onClicked: {
            qfieldCloudPasswordReset.sentLinkCount += 1;
            qfieldCloudPasswordReset.resendCountdown = 45;
            stepView.currentIndex = 1;
          }
        }

        Text {
          Layout.alignment: Qt.AlignHCenter
          text: qsTr("Back to sign in")
          font: QfTheme.tipFont
          color: QfTheme.cloudColor

          MouseArea {
            anchors.fill: parent

            onClicked: qfieldCloudPasswordReset.cancelled()
          }
        }
      }

      ColumnLayout {
        id: sentStep
        spacing: 10

        Image {
          Layout.alignment: Qt.AlignHCenter
          Layout.maximumWidth: 140
          Layout.maximumHeight: 140
          fillMode: Image.PreserveAspectFit
          smooth: true
          source: "qrc:/images/nyuki.svg"
          sourceSize.width: 140
          sourceSize.height: 140
        }

        Text {
          Layout.fillWidth: true
          text: qsTr("Check your inbox")
          horizontalAlignment: Text.AlignHCenter
          font: QfTheme.strongTitleFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
        }

        Text {
          Layout.fillWidth: true
          text: qsTr("We sent a reset link to %1. Opening it brings you right back to QField.").arg(emailField.text)
          horizontalAlignment: Text.AlignHCenter
          font: QfTheme.defaultFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }

        Text {
          Layout.fillWidth: true
          Layout.topMargin: 10
          text: qsTr("Nothing yet? Check your spam folder, or %1use another email%2.").arg('<a href="changeEmail">').arg('</a>')
          horizontalAlignment: Text.AlignHCenter
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          textFormat: Text.RichText
          wrapMode: Text.WordWrap

          onLinkActivated: stepView.currentIndex = 0
        }

        QfButton {
          id: resendResetLinkButton
          objectName: "resendResetLinkButton"
          Layout.fillWidth: true
          Layout.topMargin: 10
          text: qfieldCloudPasswordReset.resendCountdown > 0 ? qsTr("Resend link in %1s").arg(qfieldCloudPasswordReset.resendCountdown) : qsTr("Resend link")
          enabled: qfieldCloudPasswordReset.resendCountdown === 0 && !qfieldCloudPasswordReset.isRateLimited
          bgcolor: QfTheme.cloudColor
          color: QfTheme.light

          onClicked: {
            qfieldCloudPasswordReset.sentLinkCount += 1;
            qfieldCloudPasswordReset.resendCountdown = 45;
          }
        }

        Text {
          Layout.fillWidth: true
          visible: qfieldCloudPasswordReset.isRateLimited
          text: qsTr("Too many reset links requested. Wait a few minutes before trying again.")
          horizontalAlignment: Text.AlignHCenter
          font: QfTheme.tipFont
          color: QfTheme.errorColor
          wrapMode: Text.WordWrap
        }

        Text {
          Layout.alignment: Qt.AlignHCenter
          text: qsTr("Back to sign in")
          font: QfTheme.tipFont
          color: QfTheme.cloudColor

          MouseArea {
            anchors.fill: parent

            onClicked: qfieldCloudPasswordReset.cancelled()
          }
        }
      }

      ColumnLayout {
        id: newPasswordStep
        spacing: 10

        RowLayout {
          Layout.fillWidth: true
          visible: qfieldCloudPasswordReset.isLinkValid
          spacing: 10

          Rectangle {
            Layout.preferredWidth: 48
            Layout.preferredHeight: 48
            radius: width / 2
            color: QfTheme.controlBackgroundAlternateColor

            Image {
              anchors.centerIn: parent
              width: 40
              height: 40
              fillMode: Image.PreserveAspectFit
              smooth: true
              source: "qrc:/images/nyuki.svg"
              sourceSize.width: 40
              sourceSize.height: 40
            }
          }

          Text {
            Layout.fillWidth: true
            text: qfieldCloudPasswordReset.username
            font: QfTheme.strongFont
            color: QfTheme.mainTextColor
            wrapMode: Text.WordWrap
          }
        }

        Text {
          Layout.fillWidth: true
          Layout.topMargin: 10
          visible: qfieldCloudPasswordReset.isLinkValid
          text: qsTr("Set a new password")
          font: QfTheme.strongTitleFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
        }

        Text {
          Layout.fillWidth: true
          visible: qfieldCloudPasswordReset.isLinkValid
          text: qsTr("Link verified. Pick a password and we will sign you in.")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }

        QfCloudPasswordField {
          id: newPasswordField
          objectName: "newPasswordField"
          Layout.fillWidth: true
          Layout.topMargin: 10
          visible: qfieldCloudPasswordReset.isLinkValid
          username: qfieldCloudPasswordReset.username
          placeholderText: qsTr("New password")

          onAccepted: {
            if (savePasswordButton.enabled) {
              savePasswordButton.clicked();
            }
          }
        }

        QfButton {
          id: savePasswordButton
          objectName: "savePasswordButton"
          Layout.fillWidth: true
          Layout.topMargin: 10
          visible: qfieldCloudPasswordReset.isLinkValid
          text: qsTr("Save and sign in")
          enabled: newPasswordField.satisfiedRequirementCount === 3
          bgcolor: QfTheme.cloudColor
          color: QfTheme.light

          onClicked: qfieldCloudPasswordReset.finished()
        }

        Text {
          Layout.fillWidth: true
          visible: qfieldCloudPasswordReset.isLinkValid
          text: qsTr("Your other devices stay signed in.")
          horizontalAlignment: Text.AlignHCenter
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }

        QfContainerCard {
          Layout.fillWidth: true
          visible: !qfieldCloudPasswordReset.isLinkValid
          accentColor: QfTheme.errorColor
          iconSource: QfTheme.getThemeVectorIcon('ic_error_outline_24dp')
          title: qsTr("This link no longer works")
          description: qsTr("Reset links expire quickly and can only be used once. Ask for a fresh one to continue.")

          QfButton {
            Layout.fillWidth: true
            Layout.topMargin: 4
            text: qsTr("Send a new link")
            bgcolor: QfTheme.cloudColor
            color: QfTheme.light

            onClicked: {
              qfieldCloudPasswordReset.isLinkValid = true;
              stepView.currentIndex = 0;
            }
          }
        }
      }
    }

    Text {
      Layout.fillWidth: true
      Layout.topMargin: 10
      visible: cloudConnection.url !== cloudConnection.defaultUrl
      text: qsTr("Server: %1").arg(cloudConnection.url)
      horizontalAlignment: Text.AlignHCenter
      font: QfTheme.tipFont
      color: QfTheme.secondaryTextColor
      wrapMode: Text.WordWrap
    }
  }

  Timer {
    id: resendCountdownTimer
    interval: 1000
    repeat: true
    running: qfieldCloudPasswordReset.resendCountdown > 0

    onTriggered: qfieldCloudPasswordReset.resendCountdown -= 1
  }

  function goBack() {
    if (stepView.currentIndex === 1) {
      stepView.currentIndex = 0;
    } else {
      cancelled();
    }
  }
}
