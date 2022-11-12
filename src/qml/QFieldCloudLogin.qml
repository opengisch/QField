import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0
import Theme 1.0

Item {
  id: qfieldcloudLogin

  property bool isServerUrlEditingActive: false
  property bool isVisible: false

  Image {
    id: sourceImg
    fillMode: Image.PreserveAspectFit
    visible: false
    width: 0
    height: 0
    smooth: true
    opacity: 0.25
    source: "qrc:/images/qfieldcloud_background.svg"
    sourceSize.width: 1024
    sourceSize.height: 1024
  }

  ShaderEffect {
      anchors.top: parent.top
      width: parent.width
      height: 200
      visible: sourceImg.status === Image.Ready
      property variant source: sourceImg
      property real frequency: 1
      property real amplitude: 0.1
      property real time: 0.0
      NumberAnimation on time {
          from: 0
          to: Math.PI*2
          duration: 5000
          loops: Animation.Infinite
          running: qfieldcloudLogin.isVisible
      }
      fragmentShader: "
                      varying highp vec2 qt_TexCoord0;
                      uniform sampler2D source;
                      uniform lowp float qt_Opacity;
                      uniform highp float frequency;
                      uniform highp float amplitude;
                      uniform highp float time;
                      void main() {
                          highp vec2 texCoord = qt_TexCoord0;
                          texCoord.y = amplitude * sin(time * frequency + texCoord.x * 6.283185) + texCoord.y;
                          gl_FragColor = texture2D(source, texCoord) * qt_Opacity;
                      }"
  }

  ColumnLayout {
    id: connectionSettings
    width: parent.width
    Layout.maximumWidth: 410
    spacing: 0

    ColumnLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.maximumWidth: 410
      Layout.minimumHeight: (logo.height + fontMetrics.height * 9) * 2
      Layout.alignment: Qt.AlignHCenter
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
        id: loginFeedbackLabel
        Layout.fillWidth: true
        visible: false
        text: qsTr( "Failed to sign in" )
        horizontalAlignment: Text.AlignHCenter
        font: Theme.defaultFont
        color: Theme.errorColor
        wrapMode: Text.Wrap

        Connections {
          target: cloudConnection

          function onLoginFailed(reason) {
            if (!qfieldCloudLogin.parent.visible)
              return

            loginFeedbackLabel.visible = true
            loginFeedbackLabel.text = reason
          }

          function onStatusChanged() {
             if (cloudConnection.status === QFieldCloudConnection.Connecting) {
              loginFeedbackLabel.visible = false
              loginFeedbackLabel.text = ''
            } else {
              loginFeedbackLabel.visible = cloudConnection.status === QFieldCloudConnection.Disconnected && loginFeedbackLabel.text.length
            }
          }
        }
      }

      Text {
        id: serverUrlLabel
        Layout.fillWidth: true
        visible: cloudConnection.status === QFieldCloudConnection.Disconnected
                 && ( cloudConnection.url !== cloudConnection.defaultUrl || isServerUrlEditingActive )
        text: qsTr( "Server URL\n(Leave empty to use the default server)" )
        horizontalAlignment: Text.AlignHCenter
        font: Theme.defaultFont
        color: 'gray'
        wrapMode: Text.WordWrap
      }

      QfTextField {
        id: serverUrlField
        Layout.preferredWidth: parent.width / 1.3
        Layout.alignment: Qt.AlignHCenter
        visible: cloudConnection.status === QFieldCloudConnection.Disconnected
                 && ( prefixUrlWithProtocol(cloudConnection.url) !== cloudConnection.defaultUrl || isServerUrlEditingActive )
        enabled: visible
        height: Math.max(fontMetrics.height, fontMetrics.boundingRect(text).height) + 34
        font: Theme.defaultFont
        horizontalAlignment: Text.AlignHCenter
        text: prefixUrlWithProtocol(cloudConnection.url )=== cloudConnection.defaultUrl ? '' : cloudConnection.url

        onTextChanged: text = text.replace(/\s+/g, '')
        onEditingFinished: cloudConnection.url = text ? prefixUrlWithProtocol(text) : cloudConnection.defaultUrl
        onReturnPressed: loginFormSumbitHandler()

        function prefixUrlWithProtocol(url) {
          if (!url || url.startsWith('http://')  || url.startsWith('https://') )
            return url

          return 'https://' + url
        }
      }

      Text {
        id: usernamelabel
        Layout.fillWidth: true
        visible: cloudConnection.status === QFieldCloudConnection.Disconnected
        text: qsTr( "Username or email" )
        horizontalAlignment: Text.AlignHCenter
        font: Theme.defaultFont
        color: 'gray'
      }

      QfTextField {
        id: usernameField
        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        Layout.preferredWidth: parent.width / 1.3
        Layout.alignment: Qt.AlignHCenter
        visible: cloudConnection.status === QFieldCloudConnection.Disconnected
        enabled: visible
        height: Math.max(fontMetrics.height, fontMetrics.boundingRect(text).height) + 34
        font: Theme.defaultFont
        horizontalAlignment: Text.AlignHCenter

        onTextChanged: text = text.replace(/\s+/g, '')
        onReturnPressed: loginFormSumbitHandler();
      }

      Text {
        id: passwordlabel
        Layout.fillWidth: true
        visible: cloudConnection.status === QFieldCloudConnection.Disconnected
        text: qsTr( "Password" )
        horizontalAlignment: Text.AlignHCenter
        font: Theme.defaultFont
        color: 'gray'
      }

      QfTextField {
        id: passwordField
        echoMode: TextInput.Password
        inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        Layout.preferredWidth: parent.width / 1.3
        Layout.alignment: Qt.AlignHCenter
        visible: cloudConnection.status === QFieldCloudConnection.Disconnected
        enabled: visible
        height: Math.max(fontMetrics.height, fontMetrics.boundingRect(text).height) + 34
        font: Theme.defaultFont
        horizontalAlignment: Text.AlignHCenter

        onReturnPressed: loginFormSumbitHandler()
      }

      FontMetrics {
        id: fontMetrics
        font: Theme.defaultFont
      }

      QfButton {
        Layout.fillWidth: true
        text: cloudConnection.status == QFieldCloudConnection.LoggedIn ? qsTr( "Sign out" ) : cloudConnection.status == QFieldCloudConnection.Connecting ? qsTr( "Signing in, please wait" ) : qsTr( "Sign in" )
        enabled: cloudConnection.status != QFieldCloudConnection.Connecting

        onClicked: loginFormSumbitHandler()
      }

      Text {
        id: cloudRegisterLabel
        Layout.fillWidth: true
        Layout.topMargin:6
        text: qsTr( 'New user?') + ' <a href="https://app.qfield.cloud/accounts/signup/">' + qsTr( 'Register an account' ) + '</a>.'
        horizontalAlignment: Text.AlignHCenter
        font: Theme.defaultFont
        textFormat: Text.RichText
        wrapMode: Text.WordWrap
        visible: cloudConnection.status === QFieldCloudConnection.Disconnected

        onLinkActivated: {
          browserPopup.url = link
          browserPopup.fullscreen = true
          browserPopup.open()
        }
      }

      Text {
        id: cloudIntroLabel
        Layout.fillWidth: true
        text: qsTr( 'The easiest way to transfer you project from QGIS to your devices!' ) +
              ' <a href="https://qfield.cloud/">' + qsTr( 'Learn more about QFieldCloud' ) + '</a>.'
        horizontalAlignment: Text.AlignHCenter
        font: Theme.defaultFont
        textFormat: Text.RichText
        wrapMode: Text.WordWrap

        onLinkActivated: Qt.openUrlExternally(link)
      }

      Item {
          // spacer item
          Layout.fillWidth: true
          Layout.fillHeight: true
      }
    }
  }

  Connections {
    target: cloudConnection

    function onStatusChanged() {
      if ( cloudConnection.status === QFieldCloudConnection.LoggedIn )
        usernameField.text = cloudConnection.username
    }
  }

  function loginFormSumbitHandler() {
    if (cloudConnection.status == QFieldCloudConnection.LoggedIn) {
      cloudConnection.logout()
    } else {
      cloudConnection.username = usernameField.text
      cloudConnection.password = passwordField.text
      cloudConnection.login()
    }
  }

  function toggleServerUrlEditing() {
    if ( cloudConnection.url != cloudConnection.defaultUrl ) {
      isServerUrlEditingActive = true
      return
    }

    isServerUrlEditingActive = !isServerUrlEditingActive
  }
}
