import QtQuick
import QtQuick.Controls
import QtTest
import org.qfield.core
import org.qfield.app

TestCase {
  name: "QFieldCloudRegisterUI"
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
    id: registerContainer
    width: 400
    height: 800

    QfCloudRegister {
      id: qfieldCloudRegister
      width: parent.width
    }
  }

  property var stepView: findChild(qfieldCloudRegister, "stepView")
  property var emailField: findChild(qfieldCloudRegister, "emailField")
  property var usernameField: findChild(qfieldCloudRegister, "usernameField")
  property var passwordField: findChild(qfieldCloudRegister, "passwordField")
  property var repeatPasswordField: findChild(qfieldCloudRegister, "repeatPasswordField")
  property var termsCheckBox: findChild(qfieldCloudRegister, "termsCheckBox")
  property var captchaAnswerField: findChild(qfieldCloudRegister, "captchaAnswerField")
  property var continueButton: findChild(qfieldCloudRegister, "continueButton")

  // This function is called after each test function that is executed in the TestCase type.
  function cleanup() {
    stepView.currentIndex = 0;
    emailField.text = "";
    usernameField.text = "";
    passwordField.password = "";
    repeatPasswordField.text = "";
    termsCheckBox.checked = false;
    captchaAnswerField.text = "";
    qfieldCloudRegister.captchaKey = "";
    qfieldCloudRegister.captchaImageUrl = "";
    qfieldCloudRegister.registrationError = "";
    qfieldCloudRegister.isRegistering = false;
  }

  /**
   * Tests the identity step gating.
   *
   * Scenario: the step only continues once a plausible email and a username are given
   */
  function test_01_identityStepRequiresEmailAndUsername() {
    compare(continueButton.enabled, false);
    emailField.text = "mohsen";
    usernameField.text = "mohsen";
    compare(continueButton.enabled, false);
    emailField.text = "mohsen@opengis.ch";
    verify(continueButton.enabled);
    continueButton.clicked();
    compare(stepView.currentIndex, 1);
  }

  /**
   * Tests the password requirements advertised on the password step.
   *
   * Scenario: too short, entirely numeric and username lookalike passwords are all rejected
   */
  function test_02_passwordRequirements() {
    usernameField.text = "mohsen";
    passwordField.password = "short";
    compare(passwordField.hasMinimumLength, false);
    passwordField.password = "1234567890";
    verify(passwordField.hasMinimumLength);
    compare(passwordField.isNotEntirelyNumeric, false);
    passwordField.password = "mohsen-in-the-field";
    compare(passwordField.isDistinctFromUsername, false);
    passwordField.password = "chogha-zanbil-1979";
    compare(passwordField.satisfiedRequirementCount, 3);
  }

  /**
   * Tests the password repeat gating.
   *
   * Scenario: the step only continues once both password fields match
   */
  function test_03_passwordStepRequiresMatchingRepeat() {
    stepView.currentIndex = 1;
    usernameField.text = "mohsen";
    passwordField.password = "chogha-zanbil-1979";
    repeatPasswordField.text = "chogha-zanbil-197";
    compare(continueButton.enabled, false);
    repeatPasswordField.text = "chogha-zanbil-1979";
    verify(continueButton.enabled);
    continueButton.clicked();
    compare(stepView.currentIndex, 2);
  }

  /**
   * Tests the agreement step gating.
   *
   * Scenario: accepting the terms alone does not allow creating the account
   */
  function test_04_agreementStepRequiresTerms() {
    stepView.currentIndex = 2;
    usernameField.text = "mohsen";
    compare(continueButton.enabled, false);
    termsCheckBox.checked = true;
    compare(continueButton.enabled, false);
  }

  /**
   * Tests the username requirements advertised on the identity step.
   *
   * Scenario: short, digit-first, and names with characters outside the server's set are all rejected
   */
  function test_05_usernameRequirements() {
    emailField.text = "mohsen@opengis.ch";
    usernameField.text = "m";
    compare(qfieldCloudRegister.hasUsernameMinimumLength, false);
    compare(continueButton.enabled, false);
    usernameField.text = "1mohsen";
    compare(qfieldCloudRegister.isUsernameStartingWithLetter, false);
    compare(continueButton.enabled, false);
    usernameField.text = "mohsen.dehghanzadeh";
    compare(qfieldCloudRegister.hasUsernameAllowedCharacters, false);
    compare(continueButton.enabled, false);
    usernameField.text = "محسن";
    compare(qfieldCloudRegister.hasUsernameAllowedCharacters, false);
    compare(continueButton.enabled, false);
    usernameField.text = "moh";
    verify(continueButton.enabled);
    usernameField.text = "mohsen_field-crew";
    verify(continueButton.enabled);
  }

  /**
   * Tests how server validation messages are presented.
   *
   * Scenario: messages are shown and the flow returns to the earliest step holding a rejected field
   */
  function test_06_registrationErrorsReturnToTheirStep() {
    stepView.currentIndex = 2;
    qfieldCloudRegister.isRegistering = true;
    captchaAnswerField.text = "gkfc";
    cloudConnection.registrationFinished({
      "password2": "You must type the same password each time."
    });
    compare(qfieldCloudRegister.isRegistering, false);
    compare(stepView.currentIndex, 1);
    compare(qfieldCloudRegister.registrationError, "You must type the same password each time.");
    compare(captchaAnswerField.text, "");
    stepView.currentIndex = 2;
    cloudConnection.registrationFinished({
      "username": "A user with that username already exists.",
      "captcha": "Invalid CAPTCHA"
    });
    compare(stepView.currentIndex, 0);
    verify(qfieldCloudRegister.registrationError.indexOf("A user with that username already exists.") >= 0);
    verify(qfieldCloudRegister.registrationError.indexOf("Invalid CAPTCHA") >= 0);
    stepView.currentIndex = 2;
    cloudConnection.registrationFinished({
      "captcha": "Invalid CAPTCHA"
    });
    compare(stepView.currentIndex, 2);
  }
}
