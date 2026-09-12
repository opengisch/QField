import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCore
import org.qfield

Item {
    id: authPlugin

    Settings {
        id: settings
        category: "auth"

        property string accessToken: ""
        property string refreshToken: ""
        property double expiresAt: 0
    }

    readonly property string tokenUrl: "https://townscanada.ca/oauth/token"
    readonly property string clientId: "YZ9xYrxcXz2-_xRFAJunQqBhBDQyp4rf4AwR8c01VYo"
    readonly property string clientSecret: "5Pjc5YTIld/kAJZBWyA1crZXU4LxS5d/MEMkMp6VP58="

    property bool loggedIn: settings.accessToken !== "" && settings.expiresAt > (Date.now() / 1000)
    property bool busy: false
    property string errorMessage: ""

    function getValidAccessToken(callback) {
        if (settings.accessToken !== "" && settings.expiresAt > (Date.now() / 1000) + 30) {
            callback(settings.accessToken)
        } else if (settings.refreshToken !== "") {
            refreshAccessToken(function(success) {
                callback(success ? settings.accessToken : "")
            })
        } else {
            callback("")
        }
    }

    function login(username, password) {
        busy = true
        errorMessage = ""

        var xhr = new XMLHttpRequest()
        var body = "grant_type=password"
            + "&client_id=" + encodeURIComponent(clientId)
            + "&client_secret=" + encodeURIComponent(clientSecret)
            + "&username=" + encodeURIComponent(username)
            + "&password=" + encodeURIComponent(password)

        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                busy = false
                if (xhr.status === 200) {
                    try {
                        var resp = JSON.parse(xhr.responseText)
                        storeTokens(resp)
                        loggedIn = true
                        iface.mainWindow().displayToast('Logged in ✓')
                    } catch (e) {
                        errorMessage = "Unexpected response from server."
                    }
                } else {
                    try {
                        var errResp = JSON.parse(xhr.responseText)
                        errorMessage = errResp.error_description || "Login failed."
                    } catch (e) {
                        errorMessage = "Login failed (status " + xhr.status + ")."
                    }
                }
            }
        }

        xhr.open("POST", tokenUrl)
        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded")
        xhr.send(body)
    }

    function refreshAccessToken(callback) {
        var xhr = new XMLHttpRequest()
        var body = "grant_type=refresh_token"
            + "&client_id=" + encodeURIComponent(clientId)
            + "&client_secret=" + encodeURIComponent(clientSecret)
            + "&refresh_token=" + encodeURIComponent(settings.refreshToken)

        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                if (xhr.status === 200) {
                    try {
                        var resp = JSON.parse(xhr.responseText)
                        storeTokens(resp)
                        callback(true)
                    } catch (e) {
                        callback(false)
                    }
                } else {
                    logout()
                    callback(false)
                }
            }
        }

        xhr.open("POST", tokenUrl)
        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded")
        xhr.send(body)
    }

    function storeTokens(resp) {
        settings.accessToken = resp.access_token
        settings.refreshToken = resp.refresh_token
        settings.expiresAt = (Date.now() / 1000) + resp.expires_in
    }

    function logout() {
        settings.accessToken = ""
        settings.refreshToken = ""
        settings.expiresAt = 0
        loggedIn = false
    }

    Rectangle {
        id: loginPanel
        anchors.fill: parent
        visible: !authPlugin.loggedIn
        color: "#ffffff"
        z: 1000

        ColumnLayout {
            anchors.centerIn: parent
            width: Math.min(parent.width * 0.85, 400)
            spacing: 16

            Label {
                text: "Towns Canada Login"
                font.pixelSize: 22
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }

            TextField {
                id: usernameField
                placeholderText: "Username"
                Layout.fillWidth: true
            }

            TextField {
                id: passwordField
                placeholderText: "Password"
                echoMode: TextInput.Password
                Layout.fillWidth: true
            }

            Label {
                text: authPlugin.errorMessage
                color: "red"
                visible: authPlugin.errorMessage !== ""
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            Button {
                text: authPlugin.busy ? "Logging in..." : "Log In"
                enabled: !authPlugin.busy && usernameField.text.length > 0 && passwordField.text.length > 0
                Layout.fillWidth: true
                onClicked: authPlugin.login(usernameField.text, passwordField.text)
            }
        }
    }

    Component.onCompleted: {
        console.log("Towns Canada auth plugin loaded. Logged in: " + loggedIn)
    }
}
