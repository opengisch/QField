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
  property var newPasswordField: findChild(qfieldCloudPasswordReset, "newPasswordField")
  property var savePasswordButton: findChild(qfieldCloudPasswordReset, "savePasswordButton")

  SignalSpy {
    id: finishedSpy
    target: qfieldCloudPasswordReset
    signalName: "finished"
  }

  SignalSpy {
    id: cancelledSpy
    target: qfieldCloudPasswordReset
    signalName: "cancelled"
  }

  // This function is called after each test function that is executed in the TestCase type.
  function cleanup() {
    qfieldCloudPasswordReset.currentStepIndex = 0;
    qfieldCloudPasswordReset.sentLinkCount = 0;
    qfieldCloudPasswordReset.resendCountdown = 0;
    qfieldCloudPasswordReset.isLinkValid = true;
    qfieldCloudPasswordReset.username = "";
    emailField.text = "";
    newPasswordField.password = "";
    finishedSpy.clear();
    cancelledSpy.clear();
  }

  /**
   * Tests the reset link request gating.
   *
   * Scenario: a plausible email is required, and sending moves on to the inbox step
   */
  function test_01_requestStepRequiresEmail() {
    compare(sendResetLinkButton.enabled, false);
    emailField.text = "mohsen";
    compare(sendResetLinkButton.enabled, false);
    emailField.text = "mohsen@opengis.ch";
    verify(sendResetLinkButton.enabled);
    sendResetLinkButton.clicked();
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
    sendResetLinkButton.clicked();
    compare(resendResetLinkButton.enabled, false);
    qfieldCloudPasswordReset.resendCountdown = 2;
    tryCompare(qfieldCloudPasswordReset, "resendCountdown", 0, 5000);
    verify(resendResetLinkButton.enabled);
  }

  /**
   * Tests the rate limit after repeated requests.
   *
   * Scenario: a third request locks both the send and the resend buttons
   */
  function test_03_repeatedRequestsAreRateLimited() {
    emailField.text = "mohsen@opengis.ch";
    qfieldCloudPasswordReset.sentLinkCount = 3;
    verify(qfieldCloudPasswordReset.isRateLimited);
    compare(sendResetLinkButton.enabled, false);
    qfieldCloudPasswordReset.resendCountdown = 0;
    compare(resendResetLinkButton.enabled, false);
  }

  /**
   * Tests the new password step reached through the emailed link.
   *
   * Scenario: the password requirements gate the save, which ends the flow signed in
   */
  function test_04_newPasswordStepEndsSignedIn() {
    qfieldCloudPasswordReset.username = "mohsen";
    qfieldCloudPasswordReset.currentStepIndex = 2;
    compare(savePasswordButton.enabled, false);
    newPasswordField.password = "1234567890";
    compare(savePasswordButton.enabled, false);
    newPasswordField.password = "chogha-zanbil-1979";
    verify(savePasswordButton.enabled);
    savePasswordButton.clicked();
    compare(finishedSpy.count, 1);
  }

  /**
   * Tests the expired link state.
   *
   * Scenario: an invalid link hides the form and offers a fresh request instead
   */
  function test_05_expiredLinkOffersANewOne() {
    qfieldCloudPasswordReset.currentStepIndex = 2;
    qfieldCloudPasswordReset.isLinkValid = false;
    compare(newPasswordField.visible, false);
    compare(savePasswordButton.visible, false);
  }

  /**
   * Tests going back from the inbox step.
   *
   * Scenario: back returns to the request step with the email kept, and leaves the flow from there
   */
  function test_06_goBackFromInboxStep() {
    emailField.text = "mohsen@opengis.ch";
    sendResetLinkButton.clicked();
    qfieldCloudPasswordReset.goBack();
    compare(stepView.currentIndex, 0);
    compare(emailField.text, "mohsen@opengis.ch");
    compare(cancelledSpy.count, 0);
    qfieldCloudPasswordReset.goBack();
    compare(cancelledSpy.count, 1);
  }
}
