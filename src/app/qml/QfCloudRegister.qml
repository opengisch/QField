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
  id: qfieldCloudRegister

  property bool isRegistering: false
  property bool isReferralCodeVisible: false
  property string registrationError: ""
  property string captchaKey: ""
  property string captchaImageUrl: ""

  readonly property bool isEmailValid: /^[^@\s]+@[^@\s]+\.[^@\s]+$/.test(emailField.text)
  readonly property bool hasUsernameMinimumLength: usernameField.text.length >= 3
  readonly property bool isUsernameStartingWithLetter: /^[a-zA-Z]/.test(usernameField.text)
  readonly property bool hasUsernameAllowedCharacters: /^[-a-zA-Z0-9_]+$/.test(usernameField.text)
  readonly property bool isUsernameValid: hasUsernameMinimumLength && isUsernameStartingWithLetter && hasUsernameAllowedCharacters
  readonly property bool isPasswordRepeated: repeatPasswordField.text.length > 0 && repeatPasswordField.text === passwordField.password

  signal cancelled
  signal finished

  implicitHeight: registrationLayout.implicitHeight

  Material.accent: QfTheme.cloudColor

  ColumnLayout {
    id: registrationLayout
    x: 10
    width: parent.width - 20
    height: qfieldCloudRegister.height
    spacing: 10

    Text {
      Layout.topMargin: 10
      Layout.fillWidth: true
      visible: qfieldCloudRegister.registrationError !== ""
      text: qfieldCloudRegister.registrationError
      font: QfTheme.defaultFont
      color: QfTheme.errorColor
      wrapMode: Text.WordWrap
    }

    SwipeView {
      id: stepView
      objectName: "stepView"
      Layout.fillWidth: true
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
        id: identityStep
        spacing: 10

        Text {
          Layout.fillWidth: true
          text: qsTr("Provide your email and desired username")
          font: QfTheme.strongTitleFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
        }

        QfTextField {
          id: emailField
          objectName: "emailField"
          Layout.fillWidth: true
          Layout.topMargin: 10
          inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
          placeholderText: qsTr("Email")

          onTextChanged: text = text.replace(/\s+/g, '')
          Keys.onReturnPressed: usernameField.forceActiveFocus()
        }

        Text {
          Layout.fillWidth: true
          text: qsTr("Your email is used for verification and password recovery")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }

        QfTextField {
          id: usernameField
          objectName: "usernameField"
          Layout.fillWidth: true
          Layout.topMargin: 10
          inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
          placeholderText: qsTr("Username")
          maximumLength: 150

          onTextChanged: text = text.replace(/\s+/g, '')
          Keys.onReturnPressed: {
            if (continueButton.enabled) {
              continueButton.clicked();
            }
          }
        }

        Text {
          Layout.fillWidth: true
          text: qsTr("Your username must meet the following criteria:")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }

        Text {
          Layout.fillWidth: true
          text: qsTr("Have at least 3 characters")
          font: QfTheme.tipFont
          color: qfieldCloudRegister.hasUsernameMinimumLength ? QfTheme.goodColor : QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }

        Text {
          Layout.fillWidth: true
          text: qsTr("Start with a letter")
          font: QfTheme.tipFont
          color: qfieldCloudRegister.isUsernameStartingWithLetter ? QfTheme.goodColor : QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }

        Text {
          Layout.fillWidth: true
          text: qsTr("Contain only a-z letters, digits, underscores or hyphens")
          font: QfTheme.tipFont
          color: qfieldCloudRegister.hasUsernameAllowedCharacters ? QfTheme.goodColor : QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }
      }

      ColumnLayout {
        id: passwordStep
        spacing: 10

        Text {
          Layout.fillWidth: true
          text: qsTr("Pick a password")
          font: QfTheme.strongTitleFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
        }

        QfCloudPasswordField {
          id: passwordField
          objectName: "passwordField"
          Layout.fillWidth: true
          Layout.topMargin: 10
          username: usernameField.text

          onAccepted: repeatPasswordField.forceActiveFocus()
        }

        QfTextField {
          id: repeatPasswordField
          objectName: "repeatPasswordField"
          Layout.fillWidth: true
          Layout.topMargin: 10
          echoMode: TextInput.Password
          inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
          placeholderText: qsTr("Repeat password")

          Keys.onReturnPressed: {
            if (continueButton.enabled) {
              continueButton.clicked();
            }
          }
        }

        Text {
          Layout.fillWidth: true
          visible: repeatPasswordField.text.length > 0 && !qfieldCloudRegister.isPasswordRepeated
          text: qsTr("The two passwords do not match yet.")
          font: QfTheme.tipFont
          color: QfTheme.errorColor
          wrapMode: Text.WordWrap
        }
      }

      ColumnLayout {
        id: agreementStep
        spacing: 10

        Text {
          Layout.fillWidth: true
          text: qsTr("One last thing")
          font: QfTheme.strongTitleFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
        }

        RowLayout {
          Layout.fillWidth: true
          Layout.topMargin: 10
          spacing: 0

          CheckBox {
            id: newsletterCheckBox
            objectName: "newsletterCheckBox"
            Layout.alignment: Qt.AlignTop
          }

          ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.topMargin: 10
            spacing: 4

            Text {
              Layout.fillWidth: true
              text: qsTr("Subscribe to the QFieldCloud newsletter")
              font: QfTheme.defaultFont
              color: QfTheme.mainTextColor
              wrapMode: Text.WordWrap

              MouseArea {
                anchors.fill: parent

                onClicked: newsletterCheckBox.toggle()
              }
            }

            Text {
              Layout.fillWidth: true
              text: qsTr("When checking this, you agree to receive our newsletter covering latest changes and important updates.")
              font: QfTheme.tipFont
              color: QfTheme.secondaryTextColor
              wrapMode: Text.WordWrap
            }
          }
        }

        RowLayout {
          Layout.fillWidth: true
          spacing: 0

          CheckBox {
            id: termsCheckBox
            objectName: "termsCheckBox"
            Layout.alignment: Qt.AlignTop
          }

          ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.topMargin: 10
            spacing: 4

            Text {
              Layout.fillWidth: true
              text: qsTr("Accept ToS and DPA")
              font: QfTheme.defaultFont
              color: QfTheme.mainTextColor
              wrapMode: Text.WordWrap

              MouseArea {
                anchors.fill: parent

                onClicked: termsCheckBox.toggle()
              }
            }

            Text {
              Layout.fillWidth: true
              text: qsTr("By checking this, you agree with the QFieldCloud %1Terms of Services (ToS)%2 and the %3Data Processing Agreement (DPA)%4.").arg('<a href="https://qfield.cloud/tos">').arg('</a>').arg('<a href="https://qfield.cloud/dpa">').arg('</a>')
              font: QfTheme.tipFont
              color: QfTheme.secondaryTextColor
              linkColor: QfTheme.cloudColor
              textFormat: Text.RichText
              wrapMode: Text.WordWrap

              onLinkActivated: link => {
                if (Qt.platform.os === "ios" || Qt.platform.os === "android") {
                  browserPopup.url = link;
                  browserPopup.fullscreen = true;
                  browserPopup.open();
                } else {
                  Qt.openUrlExternally(link);
                }
              }
            }
          }
        }

        Text {
          Layout.alignment: Qt.AlignLeft
          visible: !qfieldCloudRegister.isReferralCodeVisible
          text: qsTr("Have a referral code?")
          font: QfTheme.tipFont
          color: QfTheme.cloudColor
          wrapMode: Text.WordWrap

          MouseArea {
            anchors.fill: parent

            onClicked: {
              qfieldCloudRegister.isReferralCodeVisible = true;
              referralCodeField.forceActiveFocus();
            }
          }
        }

        QfTextField {
          id: referralCodeField
          objectName: "referralCodeField"
          Layout.fillWidth: true
          visible: qfieldCloudRegister.isReferralCodeVisible
          inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
          placeholderText: qsTr("Referral code")
        }

        RowLayout {
          Layout.fillWidth: true
          Layout.topMargin: 10
          spacing: 10

          Rectangle {
            Layout.preferredWidth: captchaImage.sourceSize.width
            Layout.preferredHeight: captchaImage.sourceSize.height
            radius: 4
            color: QfTheme.light

            Image {
              id: captchaImage
              objectName: "captchaImage"
              anchors.fill: parent
              fillMode: Image.PreserveAspectFit
              sourceSize.width: 200
              sourceSize.height: 50
              source: qfieldCloudRegister.captchaImageUrl
            }
          }

          QfToolButton {
            iconSource: QfTheme.getThemeVectorIcon('refresh_24dp')
            iconColor: QfTheme.mainTextColor
            bgcolor: "transparent"

            onClicked: {
              captchaAnswerField.text = "";
              cloudConnection.getSignupCaptcha();
            }
          }
        }

        QfTextField {
          id: captchaAnswerField
          objectName: "captchaAnswerField"
          Layout.fillWidth: true
          inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
          placeholderText: qsTr("Characters shown above")

          Keys.onReturnPressed: {
            if (continueButton.enabled) {
              continueButton.clicked();
            }
          }
        }

        Text {
          Layout.fillWidth: true
          text: qsTr("You will be signed in right away.")
          horizontalAlignment: Text.AlignHCenter
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
        }
      }

      ColumnLayout {
        id: welcomeStep
        spacing: 10

        Image {
          Layout.alignment: Qt.AlignHCenter
          Layout.topMargin: 10
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
          text: qsTr("Welcome, %1").arg(usernameField.text)
          horizontalAlignment: Text.AlignHCenter
          font: QfTheme.strongTitleFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
        }

        QfContainerCard {
          Layout.fillWidth: true
          Layout.topMargin: 10
          accentColor: QfTheme.cloudColor
          iconSource: QfTheme.getThemeVectorIcon('ic_info_white_24dp')
          title: qsTr("Verify your email")
          description: qsTr("Open the link we sent to %1 to confirm your address.").arg(emailField.text)
        }

        QfButton {
          Layout.fillWidth: true
          Layout.topMargin: 10
          text: qsTr("Open my cloud projects")
          bgcolor: QfTheme.cloudColor
          color: QfTheme.light

          onClicked: qfieldCloudRegister.finished()
        }

        Text {
          Layout.alignment: Qt.AlignHCenter
          text: qsTr("Not now")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap

          MouseArea {
            anchors.fill: parent

            onClicked: qfieldCloudRegister.finished()
          }
        }
      }
    }

    Item {
      Layout.fillWidth: true
      Layout.fillHeight: true
    }

    RowLayout {
      Layout.fillWidth: true
      Layout.bottomMargin: 10
      spacing: 10
      visible: stepView.currentIndex < 3

      QfButton {
        id: backButton
        objectName: "backButton"
        Layout.fillWidth: true
        Layout.preferredWidth: 1
        visible: !qfieldCloudRegister.isRegistering
        text: qsTr("Back")
        bgcolor: "transparent"
        color: QfTheme.mainTextColor
        borderColor: QfTheme.secondaryTextColor

        onClicked: qfieldCloudRegister.goBack()
      }

      QfProgressRing {
        id: stepRing
        Layout.preferredWidth: size
        Layout.preferredHeight: size
        visible: !qfieldCloudRegister.isRegistering
        size: 24
        value: (stepView.currentIndex + 1) / 3
        color: QfTheme.cloudColor
        backgroundColor: Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, 0.2)
      }

      QfButton {
        id: continueButton
        objectName: "continueButton"
        Layout.fillWidth: true
        Layout.preferredWidth: 1
        text: {
          if (stepView.currentIndex < 2) {
            return qsTr("Continue");
          }
          return qfieldCloudRegister.isRegistering ? qsTr("Creating your account, please wait") : qsTr("Create account");
        }
        enabled: {
          switch (stepView.currentIndex) {
          case 0:
            return qfieldCloudRegister.isEmailValid && qfieldCloudRegister.isUsernameValid;
          case 1:
            return passwordField.satisfiedRequirementCount === 3 && qfieldCloudRegister.isPasswordRepeated;
          default:
            return termsCheckBox.checked && qfieldCloudRegister.captchaKey !== "" && captchaAnswerField.text.length > 0 && !qfieldCloudRegister.isRegistering;
          }
        }
        bgcolor: QfTheme.cloudColor
        color: QfTheme.light
        showProgress: qfieldCloudRegister.isRegistering

        onClicked: {
          if (stepView.currentIndex === 0) {
            stepView.currentIndex = 1;
            return;
          }
          if (stepView.currentIndex === 1) {
            stepView.currentIndex = 2;
            captchaAnswerField.text = "";
            cloudConnection.getSignupCaptcha();
            return;
          }
          qfieldCloudRegister.registrationError = "";
          qfieldCloudRegister.isRegistering = true;
          cloudConnection.registerAccount(emailField.text, usernameField.text, passwordField.password, termsCheckBox.checked, newsletterCheckBox.checked, referralCodeField.text, qfieldCloudRegister.captchaKey, captchaAnswerField.text);
        }
      }
    }
  }

  Connections {
    target: cloudConnection

    function onSignupCaptchaReceived(key, imageUrl) {
      qfieldCloudRegister.captchaKey = key;
      qfieldCloudRegister.captchaImageUrl = imageUrl;
    }

    function onSignupCaptchaFailed(reason) {
      qfieldCloudRegister.registrationError = reason;
    }

    function onRegistered() {
      qfieldCloudRegister.isRegistering = false;
      stepView.currentIndex = 3;
    }

    function onRegistrationFailed(errors) {
      qfieldCloudRegister.isRegistering = false;
      qfieldCloudRegister.registrationError = Object.keys(errors).map(field => errors[field]).join("\n");
      if (errors.email !== undefined || errors.username !== undefined) {
        stepView.currentIndex = 0;
      } else if (errors.password1 !== undefined || errors.password2 !== undefined) {
        stepView.currentIndex = 1;
      }
      captchaAnswerField.text = "";
      cloudConnection.getSignupCaptcha();
    }
  }

  function goBack() {
    if (isRegistering) {
      return;
    }
    if (stepView.currentIndex === 0) {
      cancelled();
    } else if (stepView.currentIndex === 3) {
      finished();
    } else {
      stepView.currentIndex -= 1;
    }
  }
}
