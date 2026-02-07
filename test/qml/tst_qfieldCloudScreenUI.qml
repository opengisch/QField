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

  SignalSpy {
    id: currentProjectIdSpy
    target: cloudProjectsModel
    signalName: "currentProjectIdChanged"
  }

  SignalSpy {
    id: currentProjectSpy
    target: cloudProjectsModel
    signalName: "currentProjectChanged"
  }

  function cleanup() {
    if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
      cloudConnection.logout();
      tryCompare(cloudConnection, "status", QFieldCloudConnection.Disconnected, 5000);
    }
    cloudConnection.username = "";
    cloudConnection.url = "";
    wait(500);
    cloudProjectsModel.currentProjectId = "";
    connectionStatusSpy.clear();
    projectsRefreshSpy.clear();
    currentProjectIdSpy.clear();
    currentProjectSpy.clear();
  }

  // Helper: Login and refresh projects list
  function loginAndRefresh() {
    cloudConnection.url = qfcTestServerUrl;
    cloudConnection.username = qfcTestUsername;
    cloudConnection.login(qfcTestPassword);
    tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
    cloudProjectsModel.refreshProjectsList(true, false, 0);
    tryCompare(cloudProjectsModel, "isRefreshing", true, 5000);
    tryCompare(cloudProjectsModel, "isRefreshing", false, 30000);
    wait(500);
  }

  // Helper: Prepare project for download by clearing local data
  function prepareProjectForDownload(projectName) {
    let projectInfo = null;
    for (let i = 0; i < cloudProjectsModel.rowCount(); i++) {
      const index = cloudProjectsModel.index(i, 0);
      const name = cloudProjectsModel.data(index, QFieldCloudProjectsModel.NameRole);
      if (name === projectName) {
        projectInfo = {
          id: cloudProjectsModel.data(index, QFieldCloudProjectsModel.IdRole),
          rowIndex: i
        };
        break;
      }
    }
    verify(projectInfo !== null);
    const project = cloudProjectsModel.findProject(projectInfo.id);
    if (project.localPath !== "") {
      cloudProjectsModel.removeLocalProject(projectInfo.id);
      wait(1000);
    }
    return projectInfo;
  }

  /**
   * Tests view visibility based on connection status.
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
    verify(projectsSwipeView.visible);
    compare(connectionSettings.visible, false);
    verify(connectionInformation.visible);
  }

  /**
   * Tests filter bar tab switching.
   *
   * Scenario: Switching between 'My Projects' and 'Community' tabs changes the table filter.
   */
  function test_02_filterBarTabSwitching() {
    cloudConnection.url = qfcTestServerUrl;
    cloudConnection.username = qfcTestUsername;
    cloudConnection.login(qfcTestPassword);
    tryCompare(cloudConnection, "status", QFieldCloudConnection.LoggedIn, 15000);
    wait(500);
    compare(filterBar.currentIndex, 0);
    compare(table.model.filter, QFieldCloudProjectsFilterModel.PrivateProjects);
    filterBar.currentIndex = 1;
    wait(200);
    compare(filterBar.currentIndex, 1);
    compare(table.model.filter, QFieldCloudProjectsFilterModel.PublicProjects);
    filterBar.currentIndex = 0;
    wait(200);
    compare(table.model.filter, QFieldCloudProjectsFilterModel.PrivateProjects);
  }

  /**
   * Tests search bar filtering functionality.
   *
   * Scenario: Entering search text filters the projects list and clearing it restores full list.
   */
  function test_03_searchBarFiltering() {
    compare(table.count, 0);
    loginAndRefresh();
    const initialCount = table.count;
    searchBarTextArea.text = "---";
    wait(300);
    const filteredCount = table.count;
    verify(filteredCount < initialCount);
    searchBarTextArea.text = "";
    wait(300);
    compare(table.count, initialCount);
  }

  /**
   * Tests view transitions during login and logout.
   *
   * Scenario: Login shows projects view, logout returns to login form and clears project list.
   */
  function test_04_loginLogoutViewTransitions() {
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

  /**
   * Tests that test projects are visible in UI.
   *
   * Scenario: TestCloudLargeProject and QFieldCloudTesting appear as visible delegates in table.
   */
  function test_05_verifyTestProjectsExist() {
    loginAndRefresh();
    verify(table.count > 0);
    let foundLargeProject = false;
    let foundTestingProject = false;
    for (let i = 0; i < table.count; i++) {
      table.positionViewAtIndex(i, ListView.Center);
      wait(100);
      const delegate = table.itemAtIndex(i);
      if (delegate !== null) {
        const projectName = delegate.projectName;
        if (projectName === "TestCloudLargeProject") {
          foundLargeProject = true;
          verify(delegate.visible);
        }
        if (projectName === "QFieldCloudTesting") {
          foundTestingProject = true;
          verify(delegate.visible);
        }
      }
    }
    verify(foundLargeProject);
    verify(foundTestingProject);
  }

  /**
   * Tests complete project download workflow.
   *
   * Scenario: Click download button, wait for download to complete, verify project is locally available.
   */
  function test_06_completeDownloadWorkflow() {
    loginAndRefresh();
    const projectInfo = prepareProjectForDownload("QFieldCloudTesting");
    let project = cloudProjectsModel.findProject(projectInfo.id);
    compare(project.localPath, "");
    compare(project.status, QFieldCloudProject.Idle);
    compare(project.downloadProgress, 0);
    table.positionViewAtIndex(projectInfo.rowIndex, ListView.Center);
    wait(500);
    const delegate = table.itemAtIndex(projectInfo.rowIndex);
    verify(delegate !== null);
    const downloadButton = delegate.children[1].children[2].children[0];
    verify(downloadButton !== null);
    verify(downloadButton.visible);
    downloadButton.clicked();
    wait(3000);
    project = cloudProjectsModel.findProject(projectInfo.id);
    verify(project !== null);
    tryCompare(project, "status", QFieldCloudProject.Idle, 180000);
    verify(project.localPath !== "");
    compare(project.downloadProgress, 1);
    cloudProjectsModel.removeLocalProject(projectInfo.id);
    wait(500);
  }

  /**
   * Tests download cancellation.
   *
   * Scenario: Start download then cancel by clicking the button again during download.
   */
  function test_07_cancelDownload() {
    loginAndRefresh();
    const projectInfo = prepareProjectForDownload("TestCloudLargeProject");
    let project = cloudProjectsModel.findProject(projectInfo.id);
    compare(project.localPath, "");
    compare(project.status, QFieldCloudProject.Idle);
    table.positionViewAtIndex(projectInfo.rowIndex, ListView.Center);
    wait(500);
    const delegate = table.itemAtIndex(projectInfo.rowIndex);
    verify(delegate !== null);
    const downloadButton = delegate.children[1].children[2].children[0];
    verify(downloadButton !== null);
    verify(downloadButton.visible);
    downloadButton.clicked();
    wait(1000);
    project = cloudProjectsModel.findProject(projectInfo.id);
    verify(project !== null);
    tryCompare(project, "status", QFieldCloudProject.Downloading, 5000);
    downloadButton.clicked();
    wait(1000);
    project = cloudProjectsModel.findProject(projectInfo.id);
    compare(project.status, QFieldCloudProject.Idle);
    compare(project.localPath, "");
  }

  /**
   * Tests concurrent download of multiple projects.
   *
   * Scenario: Start downloads for two projects simultaneously and verify both complete successfully.
   */
  function test_08_concurrentDownloads() {
    loginAndRefresh();
    const project1Info = prepareProjectForDownload("QFieldCloudTesting");
    const project2Info = prepareProjectForDownload("TestCloudLargeProject");

    let project1 = cloudProjectsModel.findProject(project1Info.id);
    let project2 = cloudProjectsModel.findProject(project2Info.id);
    compare(project1.localPath, "");
    compare(project2.localPath, "");
    table.positionViewAtIndex(project1Info.rowIndex, ListView.Center);
    wait(300);
    const delegate1 = table.itemAtIndex(project1Info.rowIndex);
    verify(delegate1 !== null);
    const downloadButton1 = delegate1.children[1].children[2].children[0];
    verify(downloadButton1 !== null);
    downloadButton1.clicked();
    wait(500);

    table.positionViewAtIndex(project2Info.rowIndex, ListView.Center);
    wait(300);
    const delegate2 = table.itemAtIndex(project2Info.rowIndex);
    verify(delegate2 !== null);
    const downloadButton2 = delegate2.children[1].children[2].children[0];
    verify(downloadButton2 !== null);
    downloadButton2.clicked();
    wait(3000);

    project1 = cloudProjectsModel.findProject(project1Info.id);
    project2 = cloudProjectsModel.findProject(project2Info.id);
    tryCompare(project1, "status", QFieldCloudProject.Idle, 180000);
    tryCompare(project2, "status", QFieldCloudProject.Idle, 180000);
    verify(project1.localPath !== "");
    verify(project2.localPath !== "");
    compare(project1.downloadProgress, 1);
    compare(project2.downloadProgress, 1);

    cloudProjectsModel.removeLocalProject(project1Info.id);
    cloudProjectsModel.removeLocalProject(project2Info.id);
    wait(500);
  }

  /**
   * Tests repeated download cancellation cycles.
   *
   * Scenario: Download and cancel same project multiple times, then complete final download successfully.
   */
  function test_09_repeatedDownloadCancel() {
    loginAndRefresh();
    const projectInfo = prepareProjectForDownload("TestCloudLargeProject");
    let project = cloudProjectsModel.findProject(projectInfo.id);

    compare(project.localPath, "");

    for (let i = 0; i < 3; i++) {
      table.positionViewAtIndex(projectInfo.rowIndex, ListView.Center);
      wait(500);
      let delegate = table.itemAtIndex(projectInfo.rowIndex);
      verify(delegate !== null);
      let downloadButton = delegate.children[1].children[2].children[0];
      verify(downloadButton !== null);
      downloadButton.clicked();
      wait(1000);
      project = cloudProjectsModel.findProject(projectInfo.id);
      tryCompare(project, "status", QFieldCloudProject.Downloading, 5000);
      table.positionViewAtIndex(projectInfo.rowIndex, ListView.Center);
      wait(200);
      delegate = table.itemAtIndex(projectInfo.rowIndex);
      verify(delegate !== null);
      downloadButton = delegate.children[1].children[2].children[0];
      verify(downloadButton !== null);
      downloadButton.clicked();
      wait(1000);
      project = cloudProjectsModel.findProject(projectInfo.id);
      compare(project.status, QFieldCloudProject.Idle);
      compare(project.localPath, "");
      wait(500);
    }

    table.positionViewAtIndex(projectInfo.rowIndex, ListView.Center);
    wait(500);
    const finalDelegate = table.itemAtIndex(projectInfo.rowIndex);
    verify(finalDelegate !== null);
    const finalDownloadButton = finalDelegate.children[1].children[2].children[0];
    verify(finalDownloadButton !== null);
    finalDownloadButton.clicked();
    wait(3000);
    project = cloudProjectsModel.findProject(projectInfo.id);
    verify(project !== null);
    tryCompare(project, "status", QFieldCloudProject.Idle, 180000);
    verify(project.localPath !== "");
    compare(project.downloadProgress, 1);
    cloudProjectsModel.removeLocalProject(projectInfo.id);
    wait(500);
  }

  /**
   * Tests that setCurrentProjectId triggers correct signals and binds the right project.
   *
   * Scenario: Setting currentProjectId emits both currentProjectIdChanged and
   * currentProjectChanged, and currentProject matches the expected project.
   */
  function test_10_setCurrentProjectIdSignalsAndBinding() {
    loginAndRefresh();
    verify(cloudProjectsModel.rowCount() > 0);
    const index = cloudProjectsModel.index(0, 0);
    const projectId = cloudProjectsModel.data(index, QFieldCloudProjectsModel.IdRole);
    const projectName = cloudProjectsModel.data(index, QFieldCloudProjectsModel.NameRole);
    verify(projectId !== "");
    currentProjectIdSpy.clear();
    currentProjectSpy.clear();
    cloudProjectsModel.currentProjectId = projectId;
    wait(500);
    verify(currentProjectIdSpy.count > 0, "currentProjectIdChanged signal was not emitted");
    verify(currentProjectSpy.count > 0, "currentProjectChanged signal was not emitted");
    compare(cloudProjectsModel.currentProjectId, projectId);
    verify(cloudProjectsModel.currentProject !== null, "currentProject should not be null");
    compare(cloudProjectsModel.currentProject.id, projectId);
    compare(cloudProjectsModel.currentProject.name, projectName);
  }
}
