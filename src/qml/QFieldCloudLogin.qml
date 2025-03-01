import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Item {
  id: qfieldCloudLogin

  property bool isServerUrlEditingActive: false
  property bool isVisible: false

  height: connectionSettings.childrenRect.height

  ColumnLayout {
    id: connectionSettings
    width: parent.width
    Layout.minimumHeight: (logo.height + fontMetrics.height * 9) * 2
    Layout.bottomMargin: 10
    spacing: 10

    Image {
      id: logo
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      fillMode: Image.PreserveAspectFit
      smooth: true
      source: "qrc:/images/qfieldcloud_logo.svg"
      sourceSize.width: 124
      sourceSize.height: 124

      MouseArea {
        anchors.fill: parent
        onDoubleClicked: toggleServerUrlEditing()
      }
    }

    Text {
      Layout.fillWidth: true
      Layout.bottomMargin: 10
      horizontalAlignment: Text.AlignHCenter
      font.pointSize: Theme.titleFont.pointSize
      font.bold: true
      color: Theme.cloudColor
      wrapMode: Text.WordWrap
      text: qsTr("QFieldCloud")
    }

    Text {
      id: loginFeedbackLabel
      Layout.fillWidth: true
      Layout.bottomMargin: 10
      visible: false
      text: qsTr("Failed to sign in")
      horizontalAlignment: Text.AlignHCenter
      font: Theme.defaultFont
      color: Theme.errorColor
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
          if (cloudConnection.status === QFieldCloudConnection.Connecting) {
            loginFeedbackLabel.visible = false;
            loginFeedbackLabel.text = '';
          } else {
            loginFeedbackLabel.visible = cloudConnection.status === QFieldCloudConnection.Disconnected && loginFeedbackLabel.text.length;
          }
        }
      }
    }

    Text {
      id: serverUrlLabel
      Layout.fillWidth: true
      visible: cloudConnection.status === QFieldCloudConnection.Disconnected && (cloudConnection.url !== cloudConnection.defaultUrl || isServerUrlEditingActive)
      text: qsTr("Server URL\n(Leave empty to use the default server)")
      horizontalAlignment: Text.AlignHCenter
      font: Theme.defaultFont
      color: Theme.secondaryTextColor
      wrapMode: Text.WordWrap
    }

    ComboBox {
      id: serverUrlComboBox
      Layout.preferredWidth: parent.width / 1.3
      Layout.alignment: Qt.AlignHCenter
      Layout.bottomMargin: 10
      visible: cloudConnection.status === QFieldCloudConnection.Disconnected && (prefixUrlWithProtocol(cloudConnection.url) !== cloudConnection.defaultUrl || isServerUrlEditingActive)
      enabled: visible
      font: Theme.defaultFont
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
        visible: cloudConnection.status === QFieldCloudConnection.Disconnected
        enabled: visible
        font: Theme.defaultFont
        horizontalAlignment: Text.AlignHCenter
        text: parent.displayText

        onTextChanged: text = text.replace(/\s+/g, '')
        Keys.onReturnPressed: loginFormSumbitHandler()

        background: Rectangle {
          color: "transparent"
        }
      }
    }

    TextField {
      id: usernameField
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
      Layout.preferredWidth: parent.width - showPasswordButton.width * 2
      Layout.alignment: Qt.AlignHCenter
      visible: cloudConnection.status === QFieldCloudConnection.Disconnected
      enabled: visible
      font: Theme.defaultFont
      horizontalAlignment: Text.AlignHCenter
      placeholderText: qsTr("Username or email")

      onTextChanged: text = text.replace(/\s+/g, '')
      Keys.onReturnPressed: loginFormSumbitHandler()
    }

    TextField {
      id: passwordField
      echoMode: TextInput.Password
      inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
      Layout.preferredWidth: parent.width - showPasswordButton.width * 2
      Layout.alignment: Qt.AlignHCenter
      Layout.bottomMargin: 10
      visible: cloudConnection.status === QFieldCloudConnection.Disconnected
      enabled: visible
      font: Theme.defaultFont
      horizontalAlignment: Text.AlignHCenter
      placeholderText: qsTr("Password")

      Keys.onReturnPressed: loginFormSumbitHandler()

      QfToolButton {
        id: showPasswordButton

        property var linkedField: passwordField
        property int originalEchoMode: TextInput.Normal

        visible: (!!linkedField.echoMode && linkedField.echoMode !== TextInput.Normal) || originalEchoMode !== TextInput.Normal
        iconSource: linkedField.echoMode === TextInput.Normal ? Theme.getThemeVectorIcon('ic_hide_green_48dp') : Theme.getThemeVectorIcon('ic_show_green_48dp')
        iconColor: Theme.mainColor
        anchors.left: linkedField.right
        anchors.verticalCenter: linkedField.verticalCenter
        opacity: linkedField.text.length > 0 ? 1 : 0.25

        onClicked: {
          if (linkedField.echoMode !== TextInput.Normal) {
            originalEchoMode = linkedField.echoMode;
            linkedField.echoMode = TextInput.Normal;
          } else {
            linkedField.echoMode = originalEchoMode;
          }
        }
      }
    }

    FontMetrics {
      id: fontMetrics
      font: Theme.defaultFont
    }

    QfButton {
      Layout.fillWidth: true
      Layout.bottomMargin: 10
      text: cloudConnection.status == QFieldCloudConnection.LoggedIn ? qsTr("Sign out") : cloudConnection.status == QFieldCloudConnection.Connecting ? qsTr("Signing in, please wait") : qsTr("Sign in")
      enabled: cloudConnection.status != QFieldCloudConnection.Connecting

      onClicked: loginFormSumbitHandler()
    }

    Text {
      id: cloudRegisterLabel
      Layout.fillWidth: true
      Layout.topMargin: 6
      text: qsTr('New user?') + ' <a href="https://app.qfield.cloud/accounts/signup/">' + qsTr('Register an account') + '</a>.'
      horizontalAlignment: Text.AlignHCenter
      font: Theme.defaultFont
      color: Theme.mainTextColor
      textFormat: Text.RichText
      wrapMode: Text.WordWrap
      visible: cloudConnection.status === QFieldCloudConnection.Disconnected

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

    Text {
      id: cloudIntroLabel
      Layout.fillWidth: true
      text: qsTr('The easiest way to transfer you project from QGIS to your devices!') + ' <a href="https://qfield.cloud/">' + qsTr('Learn more about QFieldCloud') + '</a>.'
      horizontalAlignment: Text.AlignHCenter
      font: Theme.defaultFont
      color: Theme.mainTextColor
      textFormat: Text.RichText
      wrapMode: Text.WordWrap

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

  Connections {
    target: cloudConnection

    function onStatusChanged() {
      if (cloudConnection.status === QFieldCloudConnection.LoggedIn)
        usernameField.text = cloudConnection.username;
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
    if (cloudConnection.status == QFieldCloudConnection.LoggedIn) {
      cloudConnection.logout();
    } else {
      cloudConnection.username = usernameField.text;
      cloudConnection.password = passwordField.text;
      cloudConnection.url = serverUrlField.text !== '' && prefixUrlWithProtocol(serverUrlField.text) !== cloudConnection.defaultUrl ? prefixUrlWithProtocol(serverUrlField.text) : cloudConnection.defaultUrl;
      cloudConnection.login();
    }
  }

  function toggleServerUrlEditing() {
    if (cloudConnection.url != cloudConnection.defaultUrl) {
      isServerUrlEditingActive = true;
      return;
    }
    isServerUrlEditingActive = !isServerUrlEditingActive;
  }
}
