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

  property string email: ""
  property int resendCountdown: 0
  property bool isSendingResetLink: false
  // Message sent back by the server, e.g. an email it does not know
  property string resetError: ""

  readonly property bool isEmailValid: /^[^@\s]+@[^@\s]+\.[^@\s]+$/.test(emailField.text)

  signal cancelled

  implicitHeight: passwordResetLayout.implicitHeight

  Material.accent: QfTheme.cloudColor

  ColumnLayout {
    id: passwordResetLayout
    x: 10
    width: parent.width - 20
    spacing: 10

    Text {
      Layout.topMargin: 10
      Layout.fillWidth: true
      visible: qfieldCloudPasswordReset.resetError !== ""
      text: qfieldCloudPasswordReset.resetError
      font: QfTheme.defaultFont
      color: QfTheme.errorColor
      wrapMode: Text.WordWrap
    }

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

        QfButton {
          id: sendResetLinkButton
          objectName: "sendResetLinkButton"
          Layout.fillWidth: true
          Layout.topMargin: 10
          text: qsTr("Send reset link")
          enabled: qfieldCloudPasswordReset.isEmailValid && !qfieldCloudPasswordReset.isSendingResetLink
          bgcolor: QfTheme.cloudColor
          color: QfTheme.light
          showProgress: qfieldCloudPasswordReset.isSendingResetLink

          onClicked: {
            qfieldCloudPasswordReset.resetError = "";
            qfieldCloudPasswordReset.isSendingResetLink = true;
            cloudConnection.requestPasswordReset(emailField.text);
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
          text: qsTr("We sent a reset link to %1. Open it to pick a new password.").arg(emailField.text)
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
          linkColor: QfTheme.cloudColor
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
          enabled: qfieldCloudPasswordReset.resendCountdown === 0 && !qfieldCloudPasswordReset.isSendingResetLink
          bgcolor: QfTheme.cloudColor
          color: QfTheme.light
          showProgress: qfieldCloudPasswordReset.isSendingResetLink

          onClicked: {
            qfieldCloudPasswordReset.resetError = "";
            qfieldCloudPasswordReset.isSendingResetLink = true;
            cloudConnection.requestPasswordReset(emailField.text);
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

  Connections {
    target: cloudConnection

    function onPasswordRequestFinished(error) {
      qfieldCloudPasswordReset.isSendingResetLink = false;
      if (error !== "") {
        qfieldCloudPasswordReset.resetError = error;
        return;
      }
      qfieldCloudPasswordReset.resendCountdown = 45;
      stepView.currentIndex = 1;
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
