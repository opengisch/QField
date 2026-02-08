import QtQuick
import QtQuick.Controls
import QtTest
import org.qfield
import Theme
import "../../src/qml/" as QFieldControls

TestCase {
  name: "QFieldCloudLoginUI"
  when: windowShown

  // QFieldCloud test credentials — injected via context properties.
  property string qfcTestServerUrl: typeof (qfcTestServerUrl) !== "undefined" ? qfcTestServerUrl : ""
  property string qfcTestUsername: typeof (qfcTestUsername) !== "undefined" ? qfcTestUsername : ""
  property string qfcTestPassword: typeof (qfcTestPassword) !== "undefined" ? qfcTestPassword : ""

  // Dummy mainWindow required by some components
  Item {
    id: mainWindow
    width: 400
    height: 800
    property int sceneTopMargin: 0
    property int sceneBottomMargin: 0
    property int sceneLeftMargin: 0
    property int sceneRightMargin: 0
  }

  QFieldCloudConnection {
    id: cloudConnection
  }

  Window {
    id: loginContainer
    width: 400
    height: 600

    QFieldControls.QFieldCloudLogin {
      id: qfieldCloudLogin
      isVisible: true
      width: parent.width
    }
  }

  property var connectionSettings: qfieldCloudLogin.children[0]
  property var loginFeedbackLabel: connectionSettings.children[2]
  property var serverUrlLabel: connectionSettings.children[3]
  property var serverUrlComboBox: connectionSettings.children[4]
  property var usernameField: connectionSettings.children[5]
  property var passwordField: connectionSettings.children[6]
  property var showPasswordButton: passwordField.children[1]
  property var availableProvidersRepeater: connectionSettings.children[9]
  property var cloudRegisterLabel: connectionSettings.children[10]

  SignalSpy {
    id: loginFailedSpy
    target: cloudConnection
    signalName: "loginFailed"
  }

  SignalSpy {
    id: availableProvidersChangedSpy
    target: cloudConnection
    signalName: "availableProvidersChanged"
  }

  // Skip tests if QFieldCloud credentials are not available
  function init() {
    if (!qfcTestServerUrl || !qfcTestUsername || !qfcTestPassword) {
      skip("QFieldCloud test credentials not available, skipping");
    }
  }

  // This function is called after each test function that is executed in the TestCase type.
  function cleanup() {
    // Logout after each test
    if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
      cloudConnection.logout();
      tryCompare(cloudConnection, "status", QFieldCloudConnection.Disconnected, 5000);
    }

    // Reset component state
    qfieldCloudLogin.hasCredentialsAuthentication = true;
    qfieldCloudLogin.isServerUrlEditingActive = false;

    // Clear spies
    loginFailedSpy.clear();
    availableProvidersChangedSpy.clear();
  }

  /**
   * Tests username/password fields visibility based on connection status.
   *
   * Scenario: Fields should be visible when disconnected, hidden when logged in
   */
  function test_01_fieldsVisibilityByConnectionStatus() {
    compare(cloudConnection.status, QFieldCloudConnection.Disconnected);
    verify(usernameField.visible);
    verify(passwordField.visible);
    verify(cloudRegisterLabel.visible);
    cloudConnection.url = qfcTestServerUrl;
    cloudConnection.username = qfcTestUsername;
    cloudConnection.login(qfcTestPassword);
    wait(5000); // Give it some time
    tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
    compare(usernameField.visible, false);
    compare(passwordField.visible, false);
    compare(cloudRegisterLabel.visible, false);
  }

  /**
   * Tests username/password fields visibility based on hasCredentialsAuthentication.
   *
   * Scenario: Fields should hide when credentials auth is disabled
   */
  function test_02_fieldsVisibilityByCredentialsAuthFlag() {
    compare(qfieldCloudLogin.hasCredentialsAuthentication, true);
    verify(usernameField.visible, "Username field should be visible with credentials auth");
    verify(passwordField.visible, "Password field should be visible with credentials auth");
    qfieldCloudLogin.hasCredentialsAuthentication = false;
    compare(usernameField.visible, false);
    compare(passwordField.visible, false);
    qfieldCloudLogin.hasCredentialsAuthentication = true;
    verify(usernameField.visible);
    verify(passwordField.visible);
  }

  /**
   * Tests login feedback label visibility and content on login failure.
   *
   * Scenario: Error message should appear when login fails and Error message should clear when attempting new login
   */
  function test_03_loginFeedbackOnFailure() {
    compare(loginFeedbackLabel.visible, false);
    cloudConnection.url = qfcTestServerUrl;
    cloudConnection.username = "wrong_user_name";
    cloudConnection.login("wrong_password_12345");
    tryCompare(loginFailedSpy, "count", 1, 15000);
    wait(200);
    verify(loginFeedbackLabel.visible);
    verify(loginFeedbackLabel.text.length > 0);
    loginFailedSpy.clear();
    cloudConnection.username = qfcTestUsername;
    cloudConnection.login(qfcTestPassword);
    wait(2000);
    tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
    compare(loginFeedbackLabel.visible, false);
  }

  /**
   * Tests server URL field visibility based on isServerUrlEditingActive.
   *
   * Scenario: Server URL field appears when editing is activated
   */
  function test_04_serverUrlVisibilityOnEditingToggle() {
    compare(qfieldCloudLogin.isServerUrlEditingActive, false);
    qfieldCloudLogin.isServerUrlEditingActive = true;
    verify(serverUrlComboBox.visible);
    verify(serverUrlLabel.visible);
    qfieldCloudLogin.isServerUrlEditingActive = false;
  }

  /**
   * Tests password show/hide toggle functionality.
   *
   * Scenario: Password visibility toggles when show button is clicked
   */
  function test_05_passwordVisibilityToggle() {
    passwordField.text = "qfcTestPassword";
    compare(passwordField.echoMode, TextInput.Password);
    showPasswordButton.clicked();
    wait(50);
    compare(passwordField.echoMode, TextInput.Normal);
    showPasswordButton.clicked();
    wait(50);
    compare(passwordField.echoMode, TextInput.Password);
  }

  /**
   * Tests authentication providers repeater model updates from server.
   *
   * Scenario: availableProvidersRepeater.model updates when providers are fetched
   * and hasCredentialsAuthentication is set based on whether credentials provider exists
   */
  function test_06_authProvidersRepeaterModelUpdate() {
    var initialCount = availableProvidersRepeater.model.length;
    availableProvidersChangedSpy.clear();
    cloudConnection.url = qfcTestServerUrl;
    cloudConnection.getAuthenticationProviders();
    tryCompare(availableProvidersChangedSpy, "count", 1, 10000);
    wait(200);
    verify(availableProvidersRepeater.model.length > 0);
    compare(availableProvidersRepeater.model.length, cloudConnection.availableProviders.length);
    var hasCredentialsProvider = false;
    for (var i = 0; i < cloudConnection.availableProviders.length; i++) {
      if (cloudConnection.availableProviders[i].id === "credentials") {
        hasCredentialsProvider = true;
        break;
      }
    }
    compare(qfieldCloudLogin.hasCredentialsAuthentication, hasCredentialsProvider);
  }

  /**
   * Tests username field whitespace removal behavior.
   *
   * Scenario: Whitespace should be automatically removed from username
   *
   * onTextChanged: text = text.replace(/\s+/g, '')
   */
  function test_07_usernameWhitespaceRemoval() {
    usernameField.text = "test user name";
    compare(usernameField.text, "testusername");
    usernameField.text = "  hello   world  ";
    compare(usernameField.text, "helloworld");
  }

  /**
   * Tests username field syncs with cloudConnection after login.
   *
   * Scenario: After login, username field should show the logged-in username
   */
  function test_08_usernameFieldSyncsAfterLogin() {
    usernameField.text = "";
    cloudConnection.url = qfcTestServerUrl;
    cloudConnection.username = qfcTestUsername;
    cloudConnection.login(qfcTestPassword);
    tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
    compare(usernameField.text, cloudConnection.username);
  }
}
