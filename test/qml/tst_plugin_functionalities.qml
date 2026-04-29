import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
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

  // Active layer switching via plugin toolbar buttons

  Component {
    id: activeLayerSwitcherPlugin

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
        onClicked: plugin.activateLayerByName("ApiaryLayer")
      }

      QfToolButton {
        id: tracksButton
        text: "T"
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true
        onClicked: plugin.activateLayerByName("TracksLayer")
      }
    }
  }

  function test_pluginInstantiatesWithRealIface() {
    const plugin = createTemporaryObject(activeLayerSwitcherPlugin, testCase);
    verify(plugin !== null);
  }

  function test_ifaceMainWindowResolvesFromContext() {
    const plugin = createTemporaryObject(activeLayerSwitcherPlugin, testCase);
    verify(plugin.mainWindow !== null);
  }

  function test_ifaceFindItemByObjectNameResolvesDashBoard() {
    const plugin = createTemporaryObject(activeLayerSwitcherPlugin, testCase);
    verify(plugin.dashBoard !== null);
    compare(plugin.dashBoard.objectName, "dashBoard");
  }

  function test_ifaceAddItemToPluginsToolbarReparentsButtons() {
    createTemporaryObject(activeLayerSwitcherPlugin, testCase);
    compare(pluginsToolbar.children.length, 2);
    compare(pluginsToolbar.children[0].text, "A");
    compare(pluginsToolbar.children[1].text, "T");
  }

  function test_qfToolButtonHonoursPluginConfiguredAppearance() {
    createTemporaryObject(activeLayerSwitcherPlugin, testCase);
    const apiary = findToolbarButtonByText("A");
    compare(apiary.round, true);
    compare(apiary.iconColor, Theme.toolButtonColor);
    compare(apiary.bgcolor, Theme.toolButtonBackgroundColor);
  }

  function test_dashBoardActiveLayerSwitchesAcrossLayers() {
    makeMemoryLayer("ApiaryLayer");
    makeMemoryLayer("TracksLayer");
    const plugin = createTemporaryObject(activeLayerSwitcherPlugin, testCase);

    plugin.activateLayerByName("ApiaryLayer");
    compare(dashBoardItem.activeLayer.name, "ApiaryLayer");
    plugin.activateLayerByName("TracksLayer");
    compare(dashBoardItem.activeLayer.name, "TracksLayer");
  }

  function test_activateLayerByNameNoOpsForUnknownLayer() {
    const plugin = createTemporaryObject(activeLayerSwitcherPlugin, testCase);
    plugin.activateLayerByName("UnknownLayer_xyz");
    compare(dashBoardItem.activeLayer, null);
  }

  function test_qfToolButtonClickActivatesTargetLayer() {
    makeMemoryLayer("ApiaryLayer");
    createTemporaryObject(activeLayerSwitcherPlugin, testCase);
    const apiary = findToolbarButtonByText("A");
    apiary.clicked();
    verify(dashBoardItem.activeLayer !== null);
    compare(dashBoardItem.activeLayer.name, "ApiaryLayer");
  }

  // Bees-Model

  Component {
    id: beesModelLayerNamesProbe

    Instantiator {
      property var collected: []

      delegate: QtObject {
        property string layerName: model.Name
        property var layerPointer: model.LayerPointer
      }

      onObjectAdded: function (index, object) {
        collected.push({
          "name": object.layerName,
          "pointer": object.layerPointer
        });
      }
    }
  }

  Component {
    id: beesModelPlugin

    Item {
      id: plugin

      property var iface
      property var qgisProject
      property var mainWindow: iface ? iface.mainWindow() : null
      property alias layersButton: layersButton
      property alias layersDialog: layersDialog
      property alias layersComboBox: layersComboBox
      property alias layersModel: layersModel

      Component.onCompleted: {
        iface.addItemToPluginsToolbar(layersButton);
      }

      QfToolButton {
        id: layersButton

        text: "?"
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true

        onClicked: layersDialog.open()
      }

      QfDialog {
        id: layersDialog

        x: mainWindow ? (mainWindow.width - width) / 2 : 0
        y: mainWindow ? (mainWindow.height - height) / 2 : 0
        width: 300
        height: layersLayout.height + 100
        parent: mainWindow ? mainWindow.contentItem : plugin

        ColumnLayout {
          id: layersLayout
          width: parent.width

          Label {
            Layout.fillWidth: true
            text: "A combobox full of layers"
          }

          QfComboBox {
            id: layersComboBox

            Layout.fillWidth: true
            model: MapLayerModel {
              id: layersModel
              project: plugin.qgisProject
            }
            textRole: 'Name'
            valueRole: 'LayerPointer'
          }
        }
      }
    }
  }

  function test_beesModel_01_mapLayerModelTracksProjectLayers() {
    makeMemoryLayer("BeesModelLayerA");
    makeMemoryLayer("BeesModelLayerB");
    const plugin = createTemporaryObject(beesModelPlugin, testCase, {
      "iface": ifaceStub,
      "qgisProject": qgisProject
    });
    verify(plugin !== null);
    verify(plugin.layersModel.rowCount() >= 2, "model must include the freshly added layers");
  }

  function test_beesModel_02_mapLayerModelExposesNameAndLayerPointerRoles() {
    const layer = makeMemoryLayer("BeesModelRolesProbe");
    const plugin = createTemporaryObject(beesModelPlugin, testCase, {
      "iface": ifaceStub,
      "qgisProject": qgisProject
    });
    verify(plugin !== null);
    const probe = createTemporaryObject(beesModelLayerNamesProbe, testCase, {
      "model": plugin.layersModel
    });
    verify(probe !== null);
    let probeNames = probe.collected.map(function (e) {
      return e.name;
    });
    verify(probeNames.indexOf("BeesModelRolesProbe") !== -1, "Name role must populate from layer.name");
    const entry = probe.collected.find(function (e) {
      return e.name === "BeesModelRolesProbe";
    });
    verify(entry !== undefined);
    verify(entry.pointer !== null && entry.pointer !== undefined, "LayerPointer role must populate");
    compare(entry.pointer.id, layer.id);
  }

  function test_beesModel_03_qfComboBoxAcceptsModelAndRoles() {
    const plugin = createTemporaryObject(beesModelPlugin, testCase, {
      "iface": ifaceStub,
      "qgisProject": qgisProject
    });
    verify(plugin !== null);
    compare(plugin.layersComboBox.textRole, "Name");
    compare(plugin.layersComboBox.valueRole, "LayerPointer");
    compare(plugin.layersComboBox.model, plugin.layersModel);
  }

  function test_beesModel_04_qfDialogOpensOnButtonClick() {
    const plugin = createTemporaryObject(beesModelPlugin, testCase, {
      "iface": ifaceStub,
      "qgisProject": qgisProject
    });
    verify(plugin !== null);
    compare(plugin.layersDialog.visible, false);
    plugin.layersButton.clicked();
    tryCompare(plugin.layersDialog, "visible", true);
  }

  function test_beesModel_05_pluginCompletionRegistersLayersButton() {
    const plugin = createTemporaryObject(beesModelPlugin, testCase, {
      "iface": ifaceStub,
      "qgisProject": qgisProject
    });
    verify(plugin !== null);
    compare(pluginsToolbarStub.addedItems.length, 1);
    compare(pluginsToolbarStub.addedItems[0].text, "?");
  }
}
