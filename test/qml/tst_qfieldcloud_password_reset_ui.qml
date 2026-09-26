import QtQuick
import QtQuick.Controls
import QtTest
import org.qfield.core
import org.qfield.app

TestCase {
  name: "QFieldCloudPasswordResetUI"
  when: windowShown

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

  QfCloudConnection {
    id: cloudConnection
    url: "http://127.0.0.1:9"
  }

  Window {
    id: passwordResetContainer
    width: 400
    height: 800

    QfCloudPasswordReset {
      id: qfieldCloudPasswordReset
      width: parent.width
    }
  }

  property var stepView: findChild(qfieldCloudPasswordReset, "stepView")
  property var emailField: findChild(qfieldCloudPasswordReset, "emailField")
  property var sendResetLinkButton: findChild(qfieldCloudPasswordReset, "sendResetLinkButton")
  property var resendResetLinkButton: findChild(qfieldCloudPasswordReset, "resendResetLinkButton")

  SignalSpy {
    id: cancelledSpy
    target: qfieldCloudPasswordReset
    signalName: "cancelled"
  }

  // This function is called after each test function that is executed in the TestCase type.
  function cleanup() {
    stepView.currentIndex = 0;
    qfieldCloudPasswordReset.resendCountdown = 0;
    qfieldCloudPasswordReset.isSendingResetLink = false;
    qfieldCloudPasswordReset.resetError = "";
    emailField.text = "";
    cancelledSpy.clear();
  }

  /**
   * Tests the reset link request gating.
   *
   * Scenario: a plausible email is required, the request locks the button, and the server taking it moves on to the inbox step
   */
  function test_01_requestStepRequiresEmail() {
    compare(sendResetLinkButton.enabled, false);
    emailField.text = "mohsen";
    compare(sendResetLinkButton.enabled, false);
    emailField.text = "mohsen@opengis.ch";
    verify(sendResetLinkButton.enabled);
    sendResetLinkButton.clicked();
    verify(qfieldCloudPasswordReset.isSendingResetLink);
    compare(sendResetLinkButton.enabled, false);
    tryCompare(qfieldCloudPasswordReset, "isSendingResetLink", false, 10000);
    verify(qfieldCloudPasswordReset.resetError !== "");
    compare(stepView.currentIndex, 0);
    cloudConnection.passwordRequestFinished("");
    compare(stepView.currentIndex, 1);
    compare(qfieldCloudPasswordReset.resendCountdown, 45);
  }

  /**
   * Tests the resend countdown.
   *
   * Scenario: resending is blocked while the countdown runs and ticks down on its own
   */
  function test_02_resendIsHeldBackByCountdown() {
    emailField.text = "mohsen@opengis.ch";
    cloudConnection.passwordRequestFinished("");
    compare(resendResetLinkButton.enabled, false);
    qfieldCloudPasswordReset.resendCountdown = 2;
    tryCompare(qfieldCloudPasswordReset, "resendCountdown", 0, 5000);
    verify(resendResetLinkButton.enabled);
  }

  /**
   * Tests going back from the inbox step.
   *
   * Scenario: back returns to the request step with the email kept, and leaves the flow from there
   */
  function test_03_goBackFromInboxStep() {
    emailField.text = "mohsen@opengis.ch";
    cloudConnection.passwordRequestFinished("");
    qfieldCloudPasswordReset.goBack();
    compare(stepView.currentIndex, 0);
    compare(emailField.text, "mohsen@opengis.ch");
    compare(cancelledSpy.count, 0);
    qfieldCloudPasswordReset.goBack();
    compare(cancelledSpy.count, 1);
  }

  /**
   * Tests a request the server turns down.
   *
   * Scenario: the message is shown on the request step and the button is given back
   */
  function test_04_rejectedRequestShowsTheServerMessage() {
    emailField.text = "mohsen@opengis.ch";
    qfieldCloudPasswordReset.isSendingResetLink = true;
    cloudConnection.passwordRequestFinished("The e-mail address is not assigned to any user account");
    compare(qfieldCloudPasswordReset.isSendingResetLink, false);
    compare(qfieldCloudPasswordReset.resetError, "The e-mail address is not assigned to any user account");
    compare(stepView.currentIndex, 0);
    verify(sendResetLinkButton.enabled);
  }
}
