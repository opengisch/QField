import QtQuick
import QtTest
import org.qfield
import org.qgis
import Theme
import "../../src/qml/" as QFieldControls

TestCase {
  id: testCase
  name: "PluginFunctionalities"

  // DashBoard reads mainWindow, stateMachine and displayToast from its
  // surrounding scope, mirroring how qgismobileapp.qml exposes them in production
  Item {
    id: mainWindow

    width: 800
    height: 600
    property double sceneLeftMargin: 0
    property double sceneTopMargin: 0
    property double sceneBottomMargin: 0
    property var contentItem: testCase

    function displayToast(message) {
    }
  }

  QtObject {
    id: stateMachine

    property string state: "browse"
  }

  Item {
    id: pluginsToolbar
    objectName: "pluginsToolbar"
  }

  QFieldControls.DashBoard {
    id: dashBoardItem
    objectName: "dashBoard"
  }

  function init() {
    for (let i = pluginsToolbar.children.length - 1; i >= 0; --i) {
      pluginsToolbar.children[i].parent = null;
    }
    dashBoardItem.activeLayer = null;
  }

  function makeMemoryLayer(name) {
    const existing = qgisProject.mapLayersByName(name);
    if (existing.length > 0) {
      return existing[0];
    }
    const fields = FeatureUtils.createFields([FeatureUtils.createField("id", FeatureUtils.Int)]);
    const layer = LayerUtils.createMemoryLayer(name, fields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    ProjectUtils.addMapLayer(qgisProject, layer);
    return layer;
  }

  function findToolbarButtonByText(text) {
    for (let i = 0; i < pluginsToolbar.children.length; ++i) {
      const child = pluginsToolbar.children[i];
      if (child.text === text) {
        return child;
      }
    }
    return null;
  }

  // Bees-Focus

  Component {
    id: beesFocusPlugin

    Item {
      id: plugin

      property var mainWindow: iface.mainWindow()
      property var dashBoard: iface.findItemByObjectName("dashBoard")

      function activateLayerByName(name) {
        const layers = ProjectUtils.mapLayers(qgisProject);
        for (const layerId in layers) {
          if (layers[layerId].name === name) {
            dashBoard.activeLayer = layers[layerId];
            break;
          }
        }
      }

      Component.onCompleted: {
        iface.addItemToPluginsToolbar(apiaryButton);
        iface.addItemToPluginsToolbar(tracksButton);
      }

      QfToolButton {
        id: apiaryButton
        text: "A"
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true
        onClicked: plugin.activateLayerByName("BeesFocusApiary")
      }

      QfToolButton {
        id: tracksButton
        text: "T"
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true
        onClicked: plugin.activateLayerByName("BeesFocusTracks")
      }
    }
  }

  function test_beesFocus_01_pluginInstantiatesWithRealIface() {
    const plugin = createTemporaryObject(beesFocusPlugin, testCase);
    verify(plugin !== null);
  }

  function test_beesFocus_02_pluginResolvesMainWindowFromIface() {
    const plugin = createTemporaryObject(beesFocusPlugin, testCase);
    verify(plugin.mainWindow !== null);
  }

  function test_beesFocus_03_pluginResolvesDashBoardFromIface() {
    const plugin = createTemporaryObject(beesFocusPlugin, testCase);
    verify(plugin.dashBoard !== null);
    compare(plugin.dashBoard.objectName, "dashBoard");
  }

  function test_beesFocus_04_pluginRegistersBothButtonsToToolbar() {
    createTemporaryObject(beesFocusPlugin, testCase);
    compare(pluginsToolbar.children.length, 2);
    compare(pluginsToolbar.children[0].text, "A");
    compare(pluginsToolbar.children[1].text, "T");
  }

  function test_beesFocus_05_buttonsHaveExpectedAppearance() {
    createTemporaryObject(beesFocusPlugin, testCase);
    const apiary = findToolbarButtonByText("A");
    compare(apiary.round, true);
    compare(apiary.iconColor, Theme.toolButtonColor);
    compare(apiary.bgcolor, Theme.toolButtonBackgroundColor);
  }

  function test_beesFocus_06_switchingBetweenLayersUpdatesActiveLayer() {
    makeMemoryLayer("BeesFocusApiary");
    makeMemoryLayer("BeesFocusTracks");
    const plugin = createTemporaryObject(beesFocusPlugin, testCase);

    plugin.activateLayerByName("BeesFocusApiary");
    compare(dashBoardItem.activeLayer.name, "BeesFocusApiary");
    plugin.activateLayerByName("BeesFocusTracks");
    compare(dashBoardItem.activeLayer.name, "BeesFocusTracks");
  }

  function test_beesFocus_07_missingLayerNameLeavesActiveLayerUntouched() {
    const plugin = createTemporaryObject(beesFocusPlugin, testCase);
    plugin.activateLayerByName("BeesFocusDoesNotExist_xyz");
    compare(dashBoardItem.activeLayer, null);
  }

  function test_beesFocus_08_buttonClickActivatesMatchingLayer() {
    makeMemoryLayer("BeesFocusApiary");
    createTemporaryObject(beesFocusPlugin, testCase);
    const apiary = findToolbarButtonByText("A");
    apiary.clicked();
    verify(dashBoardItem.activeLayer !== null);
    compare(dashBoardItem.activeLayer.name, "BeesFocusApiary");
  }
}
