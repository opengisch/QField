import QtQuick
import QtQuick.Controls
import QtTest
import org.qfield
import Theme
import "../../src/qml/" as QFieldControls

TestCase {
  name: "QFieldCloudScreenUI"
  when: windowShown

  // Dummy mainWindow required by QFieldCloudScreen
  Item {
    id: mainWindow
    width: 400
    height: 800
    property int sceneTopMargin: 0
    property int sceneBottomMargin: 0
    property int sceneLeftMargin: 0
    property int sceneRightMargin: 0

    Item {
      id: contentItem
      anchors.fill: parent
    }
  }

  // Settings mock
  QtObject {
    id: settings
    function valueBool(key, defaultValue) {
      return defaultValue;
    }
    function setValue(key, value) {
    }
  }

  QFieldCloudConnection {
    id: cloudConnection
  }

  QFieldCloudProjectsModel {
    id: cloudProjectsModel
    cloudConnection: cloudConnection
  }

  Window {
    id: qFieldCloudScreenContainer
    width: 400
    height: 600

    QFieldControls.QFieldCloudScreen {
      id: qfieldCloudScreen
      width: mainWindow.width
      height: mainWindow.height
      visible: true
    }
  }

  property var mainColumnLayout: qfieldCloudScreen.contentItem.children[0]
  property var connectionInformation: mainColumnLayout.children[0]
  property var connectionSettings: mainColumnLayout.children[1]
  property var projectsSwipeView: mainColumnLayout.children[2]
  property var projectsColumnLayout: projectsSwipeView.contentChildren[0]
  property var filterBar: projectsColumnLayout.children[0]
  property var searchBarTextArea: projectsColumnLayout.children[1].children[0].children[2]
  property var tableContainer: projectsColumnLayout.children[2]
  property var table: tableContainer.children[0]
  property var projectDetails: projectsSwipeView.contentChildren[1]

  SignalSpy {
    id: connectionStatusSpy
    target: cloudConnection
    signalName: "statusChanged"
  }

  SignalSpy {
    id: projectsRefreshSpy
    target: cloudProjectsModel
    signalName: "isRefreshingChanged"
  }

  // This function is called after each test function that is executed in the TestCase type.
  function cleanup() {
    // Logout after each test
    if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
      cloudConnection.logout();
      tryCompare(cloudConnection, "status", QFieldCloudConnection.Disconnected, 5000);
    }

    // Clear spies
    connectionStatusSpy.clear();
    projectsRefreshSpy.clear();
  }

  /**
   * Tests connectionSettings vs projectsSwipeView visibility based on login status.
   *
   * Scenario: When disconnected, show login form. When logged in, show projects list.
   */
  function test_01_viewVisibilityByConnectionStatus() {
    compare(cloudConnection.status, QFieldCloudConnection.Disconnected);
    verify(connectionSettings.visible);
    compare(projectsSwipeView.visible, false);
    cloudConnection.url = qfcTestServerUrl;
    cloudConnection.username = qfcTestUsername;
    cloudConnection.login(qfcTestPassword);
    tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
    wait(500);

    // Projects view should be visible after login
    verify(projectsSwipeView.visible);
    compare(connectionSettings.visible, false);
    verify(connectionInformation.visible);
  }

  /**
   * Tests filterBar tab switching and its effect on table filter.
   *
   * Scenario: Switching tabs changes the filter model's filter property
   */
  function test_02_filterBarTabSwitching() {
    cloudConnection.url = qfcTestServerUrl;
    cloudConnection.username = qfcTestUsername;
    cloudConnection.login(qfcTestPassword);
    tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
    wait(500);

    // Initial tab should be 'My Projects' (index 0)
    compare(filterBar.currentIndex, 0);
    compare(table.model.filter, QFieldCloudProjectsFilterModel.PrivateProjects);
    filterBar.currentIndex = 1;
    wait(200);

    // Should switch to 'Community' tab (index 1)
    compare(filterBar.currentIndex, 1);
    compare(table.model.filter, QFieldCloudProjectsFilterModel.PublicProjects);
    filterBar.currentIndex = 0;
    wait(200);

    // Filter should return to PrivateProjects
    compare(table.model.filter, QFieldCloudProjectsFilterModel.PrivateProjects);
  }

  /**
   * Tests searchBar text filter affecting table results.
   *
   * Scenario: Setting searchTerm filters the projects in the table
   */
  // function test_03_searchBarFiltering() {
  //   // No project without login
  //   compare(table.count, 0);
  //   cloudConnection.url = qfcTestServerUrl;
  //   cloudConnection.username = qfcTestUsername;
  //   cloudConnection.login(qfcTestPassword);
  //   tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
  //   cloudProjectsModel.refreshProjectsList(true, false, 0);
  //   tryCompare(cloudProjectsModel, "isRefreshing", true, 5000);
  //   tryCompare(cloudProjectsModel, "isRefreshing", false, 30000);
  //   wait(500);
  //   var initialCount = table.count;
  //   searchBarTextArea.text = "---";
  //   wait(300);

  //   // Filtered count should not exceed original count
  //   var filteredCount = table.count;
  //   verify(filteredCount < initialCount);
  //   searchBarTextArea.text = "";
  //   wait(300);

  //   // Count should return to original after clearing filter
  //   compare(table.count, initialCount);
  // }

  /**
   * Tests projectsSwipeView navigation between projects list and project details.
   *
   * Scenario: SwipeView index changes when navigating to/from project details
   */
  // function test_04_swipeViewNavigation() {
  //   cloudConnection.url = qfcTestServerUrl;
  //   cloudConnection.username = qfcTestUsername;
  //   cloudConnection.login(qfcTestPassword);
  //   tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
  //   cloudProjectsModel.refreshProjectsList(true, false, 0);
  //   tryCompare(cloudProjectsModel, "isRefreshing", true, 5000);
  //   tryCompare(cloudProjectsModel, "isRefreshing", false, 30000);
  //   wait(500);
  //   compare(projectsSwipeView.currentIndex, 0);
  //   var projectId = cloudProjectsModel.data(cloudProjectsModel.index(0, 0), QFieldCloudProjectsModel.IdRole);
  //   var project = cloudProjectsModel.findProject(projectId);
  //   verify(project);
  //   projectDetails.cloudProject = project;
  //   projectsSwipeView.currentIndex = 1;
  //   compare(projectsSwipeView.currentIndex, 1);
  //   compare(connectionInformation.visible, false);
  //   projectsSwipeView.currentIndex = 0;
  //   compare(projectsSwipeView.currentIndex, 0);
  // }

  /**
   * Tests UI state changes during login/logout cycle.
   *
   * Scenario: Views switch correctly when logging in and out
   */
  function test_05_loginLogoutViewTransitions() {
    verify(connectionSettings.visible);
    compare(projectsSwipeView.visible, false);
    cloudConnection.url = qfcTestServerUrl;
    cloudConnection.username = qfcTestUsername;
    cloudConnection.login(qfcTestPassword);
    tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
    wait(500);
    verify(projectsSwipeView.visible);
    compare(connectionSettings.visible, false);
    cloudConnection.logout();
    tryCompare(cloudConnection, "status", QFieldCloudConnection.Disconnected, 5000);
    wait(500);
    verify(table.count === 0);
    verify(connectionSettings.visible);
  }
}
