import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.app
import org.qfield.gui

/**
 * \ingroup qml_app
 */
Item {
  id: qfieldCloudLogin

  property bool hasCredentialsAuthentication: true
  property bool isServerUrlEditingActive: false
  property bool isVisible: false
  property bool isRegistrationVisible: false
  property real availableHeight: 0
  property bool isPasswordResetVisible: false
  property QfCloudStatus cloudServiceStatus: null

  width: parent.width
  height: {
    if (isRegistrationVisible) {
      return registrationLoader.height;
    }
    if (isPasswordResetVisible) {
      return passwordResetLoader.height;
    }
    return connectionSettings.childrenRect.height;
  }

  FontMetrics {
    id: fontMetrics
    font: QfTheme.defaultFont
  }

  ColumnLayout {
    id: connectionSettings
    x: 10
    width: parent.width - 20
    spacing: 10
    visible: !qfieldCloudLogin.isRegistrationVisible && !qfieldCloudLogin.isPasswordResetVisible

    Image {
      id: logo
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      Layout.maximumWidth: 210
      Layout.maximumHeight: 210
      fillMode: Image.PreserveAspectFit
      smooth: true
      source: cloudConnection.url != cloudConnection.defaultUrl && cloudConnection.serverInformation.whitelabel.logoMain !== '' ? cloudConnection.serverInformation.whitelabel.logoMain : "qrc:/images/qfieldcloud_logo.svg"

      onStatusChanged: {
        // In case the whitelabel logo fails to load, revert to the default QFieldCloud logo
        if (status == Image.Error) {
          source = "qrc:/images/qfieldcloud_logo.svg";
        }
      }

      MouseArea {
        anchors.fill: parent
        onDoubleClicked: toggleServerUrlEditing()
      }
    }

    Text {
      id: loginFeedbackLabel
      objectName: "loginFeedbackLabel"
      Layout.fillWidth: true
      Layout.bottomMargin: 10
      visible: false
      text: qsTr("Failed to sign in")
      horizontalAlignment: Text.AlignHCenter
      font: QfTheme.defaultFont
      color: QfTheme.errorColor
      wrapMode: Text.Wrap

      Connections {
        target: cloudConnection

        function onLoginFailed(reason) {
          if (!qfieldCloudLogin.parent.visible)
            return;
          loginFeedbackLabel.visible = true;
          loginFeedbackLabel.text = reason;
        }

        function onStatusChanged() {
          if (cloudConnection.status === QfCloudConnection.Connecting) {
            loginFeedbackLabel.visible = false;
            loginFeedbackLabel.text = '';
          } else {
            loginFeedbackLabel.visible = cloudConnection.status === QfCloudConnection.Disconnected && loginFeedbackLabel.text.length;
          }
        }
      }
    }

    Text {
      id: serverUrlLabel
      objectName: "serverUrlLabel"
      Layout.fillWidth: true
      visible: cloudConnection.status === QfCloudConnection.Disconnected && (cloudConnection.url !== cloudConnection.defaultUrl || isServerUrlEditingActive)
      text: qsTr("%1Server URL\n(Leave empty to use the default server)").arg(cloudConnection.serverInformation.whitelabel.siteTitle !== '' ? cloudConnection.serverInformation.whitelabel.siteTitle + ' ' : '')
      horizontalAlignment: Text.AlignHCenter
      font: QfTheme.defaultFont
      color: QfTheme.secondaryTextColor
      wrapMode: Text.WordWrap
    }

    QfComboBox {
      id: serverUrlComboBox
      objectName: "serverUrlComboBox"
      Layout.fillWidth: true
      Layout.bottomMargin: 10
      visible: cloudConnection.status === QfCloudConnection.Disconnected && (prefixUrlWithProtocol(cloudConnection.url) !== cloudConnection.defaultUrl || isServerUrlEditingActive)
      enabled: visible
      font: QfTheme.defaultFont
      editable: true
      model: [''].concat(cloudConnection.urls)

      Component.onCompleted: {
        if (cloudConnection.url != cloudConnection.defaultUrl) {
          currentIndex = find(cloudConnection.url);
        }
      }

      onModelChanged: {
        if (cloudConnection.url != cloudConnection.defaultUrl) {
          currentIndex = find(cloudConnection.url);
        }
      }

      onDisplayTextChanged: {
        serverUrlField.text = displayText;
      }

      contentItem: TextField {
        id: serverUrlField

        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        visible: cloudConnection.status === QfCloudConnection.Disconnected
        enabled: visible
        font: QfTheme.defaultFont
        horizontalAlignment: Text.AlignLeft
        text: parent.displayText
        selectionColor: QfTheme.mainColor
        selectedTextColor: QfTheme.light
        onTextChanged: {
          const cleanedText = text.replace(/\s+/g, '');
          if (cleanedText !== cloudConnection.url) {
            applyUrlTimer.restart();
          } else {
            applyUrlTimer.stop();
          }
          return cleanedText;
        }

        Keys.onReturnPressed: loginFormSumbitHandler()

        background: Rectangle {
          color: "transparent"
        }
      }

      Timer {
        id: applyUrlTimer
        interval: 500
        repeat: false
        running: false

        onTriggered: {
          cloudConnection.url = serverUrlField.text !== '' && prefixUrlWithProtocol(serverUrlField.text) !== cloudConnection.defaultUrl ? prefixUrlWithProtocol(serverUrlField.text) : cloudConnection.defaultUrl;
          cloudConnection.getServerInformation();
          qfieldCloudStatus.refresh();
        }
      }
    }

    TextField {
      id: usernameField
      objectName: "usernameField"
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
      Layout.fillWidth: true
      visible: cloudConnection.status === QfCloudConnection.Disconnected && qfieldCloudLogin.hasCredentialsAuthentication
      enabled: visible
      font: QfTheme.defaultFont
      horizontalAlignment: Text.AlignLeft
      placeholderText: qsTr("Username or email")

      onTextChanged: text = text.replace(/\s+/g, '')
      Keys.onReturnPressed: loginFormSumbitHandler()
    }

    QfTextField {
      id: passwordField
      objectName: "passwordField"
      echoMode: TextInput.Password
      inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
      Layout.fillWidth: true
      visible: cloudConnection.status === QfCloudConnection.Disconnected && qfieldCloudLogin.hasCredentialsAuthentication
      enabled: visible
      horizontalAlignment: Text.AlignLeft
      placeholderText: qsTr("Password")

      Keys.onReturnPressed: loginFormSumbitHandler()
    }

    Text {
      id: forgotPasswordLabel
      objectName: "forgotPasswordLabel"
      Layout.alignment: Qt.AlignRight
      Layout.bottomMargin: 10
      visible: passwordField.visible
      text: qsTr("Forgot your password?")
      font: QfTheme.tipFont
      color: QfTheme.cloudColor

      MouseArea {
        anchors.fill: parent

        onClicked: qfieldCloudLogin.isPasswordResetVisible = true
      }
    }

    QfCloudStatusBanner {
      cloudServiceStatus: qfieldCloudLogin.cloudServiceStatus
    }

    QfButton {
      id: signInButton
      objectName: "signInButton"
      Layout.fillWidth: true
      text: cloudConnection.status == QfCloudConnection.LoggedIn ? qsTr("Sign out") : cloudConnection.status == QfCloudConnection.Connecting ? qsTr("Signing in, please wait") : qsTr("Sign in")
      enabled: cloudConnection.status != QfCloudConnection.Connecting
      visible: qfieldCloudLogin.hasCredentialsAuthentication || cloudConnection.status != QfCloudConnection.Disconnected
      bgcolor: QfTheme.cloudColor
      color: QfTheme.light
      showProgress: cloudConnection.status == QfCloudConnection.Connecting

      onClicked: loginFormSumbitHandler()
    }

    Label {
      Layout.fillWidth: true
      text: "- " + qsTr("or") + " -"
      font: QfTheme.tipFont
      color: QfTheme.secondaryTextColor
      horizontalAlignment: Qt.AlignHCenter
      visible: cloudConnection.status === QfCloudConnection.Disconnected && (qfieldCloudLogin.hasCredentialsAuthentication && availableProvidersRepeater.count >= 2)
    }

    Repeater {
      id: availableProvidersRepeater
      objectName: "availableProvidersRepeater"
      model: []

      QfButton {
        visible: modelData.id !== "credentials" && cloudConnection.status === QfCloudConnection.Disconnected
        Layout.fillWidth: true
        text: qsTr("Sign in using %1").arg(modelData.name)
        height: 48

        bgcolor: modelData.details.styles !== undefined ? QfTheme.darkTheme ? modelData.details.styles.dark.color_fill : modelData.details.styles.light.color_fill : QfTheme.mainColor
        borderColor: modelData.details.styles !== undefined ? QfTheme.darkTheme ? modelData.details.styles.dark.color_stroke : modelData.details.styles.light.color_stroke : QfTheme.mainColor
        color: modelData.details.styles !== undefined ? QfTheme.darkTheme ? modelData.details.styles.dark.color_text : modelData.details.styles.light.color_text : QfTheme.buttonColor
        icon.source: modelData.details.styles !== undefined ? QfTheme.darkTheme ? modelData.details.styles.dark.logo : modelData.details.styles.light.logo : ""
        icon.color: "transparent"

        onClicked: {
          loginFormSubmitProvider(modelData.id);
        }
      }
    }

    Text {
      id: cloudAccountSettingsLabel
      Layout.fillWidth: true
      Layout.topMargin: 16
      text: qsTr('Visit the %1settings page%2 to manage your account.').arg('<a href="' + cloudConnection.defaultUrl + '/settings/' + cloudConnection.username + '">').arg('</a>')
      horizontalAlignment: Text.AlignHCenter
      font: QfTheme.defaultFont
      color: QfTheme.mainTextColor
      textFormat: Text.RichText
      wrapMode: Text.WordWrap
      visible: Qt.platform.os !== "ios" && cloudConnection.status === QfCloudConnection.LoggedIn && cloudConnection.url === cloudConnection.defaultUrl

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

    QfButton {
      id: createAccountButton
      objectName: "createAccountButton"
      Layout.fillWidth: true
      text: qsTr("Create an account")
      visible: Qt.platform.os !== "ios" && cloudConnection.status === QfCloudConnection.Disconnected && cloudConnection.serverInformation.signupUrl !== ''
      bgcolor: "transparent"
      color: QfTheme.cloudColor
      borderColor: QfTheme.cloudColor

      onClicked: qfieldCloudLogin.isRegistrationVisible = true
    }

    Text {
      id: cloudIntroLabel
      Layout.fillWidth: true
      Layout.topMargin: 10
      text: qsTr('The easiest way to transfer you project from QGIS to your devices!') + (Qt.platform.os !== "ios" ? ' <a href="https://qfield.cloud/">' + qsTr('Learn more about QFieldCloud') + '</a>.' : '')
      horizontalAlignment: Text.AlignHCenter
      font: QfTheme.tipFont
      color: QfTheme.secondaryTextColor
      textFormat: Text.RichText
      wrapMode: Text.WordWrap
      visible: cloudConnection.status === QfCloudConnection.Disconnected

      onLinkActivated: link => {
        Qt.openUrlExternally(link);
      }
    }

    Item {
      // spacer item
      Layout.fillWidth: true
      Layout.fillHeight: true
    }
  }

  Loader {
    id: registrationLoader
    width: qfieldCloudLogin.width
    height: item ? Math.max(item.implicitHeight, qfieldCloudLogin.availableHeight) : 0
    active: qfieldCloudLogin.isRegistrationVisible

    sourceComponent: Component {
      QfCloudRegister {
        onCancelled: qfieldCloudLogin.isRegistrationVisible = false
        onFinished: qfieldCloudLogin.isRegistrationVisible = false
      }
    }
  }

  Loader {
    id: passwordResetLoader
    width: qfieldCloudLogin.width
    height: item ? item.implicitHeight : 0
    active: qfieldCloudLogin.isPasswordResetVisible

    sourceComponent: Component {
      QfCloudPasswordReset {
        email: usernameField.text.indexOf('@') > -1 ? usernameField.text : ''

        onCancelled: qfieldCloudLogin.isPasswordResetVisible = false
      }
    }
  }

  Connections {
    target: cloudConnection

    function onStatusChanged() {
      if (cloudConnection.status === QfCloudConnection.LoggedIn) {
        usernameField.text = cloudConnection.username;
      }
    }

    function onAvailableProvidersChanged() {
      let credentialAuthenticationAvailable = true;
      if (cloudConnection.availableProviders.length > 0) {
        credentialAuthenticationAvailable = false;
        for (const availableProvider of cloudConnection.availableProviders) {
          if (availableProvider.id === "credentials") {
            credentialAuthenticationAvailable = true;
            break;
          }
        }
      }
      qfieldCloudLogin.hasCredentialsAuthentication = credentialAuthenticationAvailable;
      availableProvidersRepeater.model = cloudConnection.availableProviders;
    }
  }

  onVisibleChanged: {
    if (visible) {
      usernameField.text = cloudConnection.username;
      passwordField.text = cloudConnection.password;
      const index = serverUrlComboBox.find(cloudConnection.url);
      if (index === -1) {
        serverUrlComboBox.model = serverUrlComboBox.model.concat(cloudConnection.url);
      }
      serverUrlComboBox.currentIndex = serverUrlComboBox.find(cloudConnection.url);
    }
  }

  function goBack() {
    if (isRegistrationVisible) {
      registrationLoader.item.goBack();
    } else if (isPasswordResetVisible) {
      passwordResetLoader.item.goBack();
    }
  }

  function prefixUrlWithProtocol(url) {
    let cleanedUrl = url.trim();
    if (cleanedUrl.endsWith('/')) {
      cleanedUrl = cleanedUrl.slice(0, -1);
    }
    if (!cleanedUrl || cleanedUrl.startsWith('http://') || cleanedUrl.startsWith('https://'))
      return cleanedUrl;
    return 'https://' + cleanedUrl;
  }

  function loginFormSumbitHandler() {
    if (cloudConnection.status == QfCloudConnection.LoggedIn) {
      cloudConnection.logout();
    } else {
      cloudConnection.username = usernameField.text;
      cloudConnection.provider = "";
      cloudConnection.url = serverUrlField.text !== '' && prefixUrlWithProtocol(serverUrlField.text) !== cloudConnection.defaultUrl ? prefixUrlWithProtocol(serverUrlField.text) : cloudConnection.defaultUrl;
      cloudConnection.login(passwordField.text);
    }
  }

  function loginFormSubmitProvider(provider) {
    cloudConnection.provider = provider;
    cloudConnection.url = serverUrlField.text !== '' && prefixUrlWithProtocol(serverUrlField.text) !== cloudConnection.defaultUrl ? prefixUrlWithProtocol(serverUrlField.text) : cloudConnection.defaultUrl;
    cloudConnection.login();
  }

  function toggleServerUrlEditing() {
    if (cloudConnection.url != cloudConnection.defaultUrl) {
      isServerUrlEditingActive = true;
      return;
    }
    isServerUrlEditingActive = !isServerUrlEditingActive;
  }
}
