import QtQuick
import QtTest
import org.qfield
import org.qgis
import Theme

TestCase {
  id: testCase
  name: "PluginFunctionalities"

  Item {
    id: dashBoardStub
    objectName: "dashBoard"

    property var activeLayer: null
  }

  Item {
    id: pluginsToolbarStub

    property var addedItems: []
  }

  property var ifaceStub: ({
      "mainWindow": function () {
        return testCase;
      },
      "findItemByObjectName": function (name) {
        return name === "dashBoard" ? dashBoardStub : null;
      },
      "addItemToPluginsToolbar": function (item) {
        pluginsToolbarStub.addedItems.push(item);
      }
    })

  function init() {
    dashBoardStub.activeLayer = null;
    pluginsToolbarStub.addedItems = [];
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

  // Bees-Focus

  Component {
    id: beesFocusToolButton

    QfToolButton {
      text: "A"
      iconColor: Theme.toolButtonColor
      bgcolor: Theme.toolButtonBackgroundColor
      round: true
    }
  }

  Component {
    id: beesFocusPlugin

    Item {
      id: plugin

      property var iface
      property var qgisProject
      property var mainWindow: iface ? iface.mainWindow() : null
      property var dashBoard: iface ? iface.findItemByObjectName("dashBoard") : null
      property string apiaryLayerName: "BeesFocusApiary"
      property string tracksLayerName: "BeesFocusTracks"

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

        onClicked: plugin.activateLayerByName(plugin.apiaryLayerName)
      }

      QfToolButton {
        id: tracksButton

        text: "T"
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true

        onClicked: plugin.activateLayerByName(plugin.tracksLayerName)
      }
    }
  }

  function test_beesFocus_01_projectUtilsMapLayersReturnsLayer() {
    const layer = makeMemoryLayer("BeesFocusMapLayersTest");
    const layers = ProjectUtils.mapLayers(qgisProject);
    verify(typeof layers === "object" && layers !== null);
    verify(layer.id in layers);
    compare(layers[layer.id].id, layer.id);
  }

  function test_beesFocus_02_mapLayerExposesNameProperty() {
    const layer = makeMemoryLayer("BeesFocusNameTest");
    const layers = ProjectUtils.mapLayers(qgisProject);
    compare(layers[layer.id].name, "BeesFocusNameTest");
  }

  function test_beesFocus_03_qfToolButtonAcceptsPluginProperties() {
    const btn = createTemporaryObject(beesFocusToolButton, testCase);
    verify(btn !== null);
    compare(btn.text, "A");
    compare(btn.round, true);
    compare(btn.iconColor, Theme.toolButtonColor);
    compare(btn.bgcolor, Theme.toolButtonBackgroundColor);
  }

  function test_beesFocus_04_qfToolButtonClickedTriggersHandler() {
    const btn = createTemporaryObject(beesFocusToolButton, testCase);
    let fired = 0;
    btn.clicked.connect(function () {
      fired += 1;
    });
    btn.clicked();
    compare(fired, 1);
  }

  function test_beesFocus_05_pluginCompletionRegistersBothButtons() {
    const plugin = createTemporaryObject(beesFocusPlugin, testCase, {
      "iface": ifaceStub,
      "qgisProject": qgisProject
    });
    verify(plugin !== null);
    compare(pluginsToolbarStub.addedItems.length, 2);
    compare(pluginsToolbarStub.addedItems[0].text, "A");
    compare(pluginsToolbarStub.addedItems[1].text, "T");
  }

  function test_beesFocus_06_pluginResolvesIfaceProperties() {
    const plugin = createTemporaryObject(beesFocusPlugin, testCase, {
      "iface": ifaceStub,
      "qgisProject": qgisProject
    });
    verify(plugin !== null);
    compare(plugin.mainWindow, testCase);
    compare(plugin.dashBoard, dashBoardStub);
  }

  function test_beesFocus_07_buttonClickActivatesMatchingLayer() {
    makeMemoryLayer("BeesFocusApiary");
    makeMemoryLayer("BeesFocusTracks");
    const plugin = createTemporaryObject(beesFocusPlugin, testCase, {
      "iface": ifaceStub,
      "qgisProject": qgisProject
    });
    verify(plugin !== null);
    pluginsToolbarStub.addedItems[0].clicked();
    verify(dashBoardStub.activeLayer !== null);
    compare(dashBoardStub.activeLayer.name, "BeesFocusApiary");
    pluginsToolbarStub.addedItems[1].clicked();
    compare(dashBoardStub.activeLayer.name, "BeesFocusTracks");
  }

  function test_beesFocus_08_missingLayerNameLeavesActiveLayerUntouched() {
    const plugin = createTemporaryObject(beesFocusPlugin, testCase, {
      "iface": ifaceStub,
      "qgisProject": qgisProject
    });
    verify(plugin !== null);
    plugin.activateLayerByName("BeesFocusDoesNotExist_xyz");
    compare(dashBoardStub.activeLayer, null);
  }
}
