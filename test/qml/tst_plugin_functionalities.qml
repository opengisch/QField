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

  // Layer combobox dialog backed by MapLayerModel

  Component {
    id: layerComboBoxDialogPlugin

    Item {
      id: plugin

      property var mainWindow: iface.mainWindow()
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
              project: qgisProject
            }
            textRole: 'Name'
            valueRole: 'LayerPointer'
          }
        }
      }
    }
  }

  function test_dialogPluginInstantiatesWithRealIface() {
    const plugin = createTemporaryObject(layerComboBoxDialogPlugin, testCase);
    verify(plugin !== null);
  }

  function test_dialogPluginRegistersToolbarButton() {
    createTemporaryObject(layerComboBoxDialogPlugin, testCase);
    compare(pluginsToolbar.children.length, 1);
    compare(pluginsToolbar.children[0].text, "?");
  }

  function test_mapLayerModelTracksProjectLayers() {
    makeMemoryLayer("ComboLayerA");
    makeMemoryLayer("ComboLayerB");
    const plugin = createTemporaryObject(layerComboBoxDialogPlugin, testCase);
    verify(plugin.layersModel.rowCount() >= 2);
  }

  function test_mapLayerModelGetExposesNameAndLayerPointer() {
    const layer = makeMemoryLayer("ComboRolesProbe");
    const plugin = createTemporaryObject(layerComboBoxDialogPlugin, testCase);

    let foundEntry = null;
    for (let row = 0; row < plugin.layersModel.rowCount(); ++row) {
      const entry = plugin.layersModel.get(row);
      if (entry.Name === "ComboRolesProbe") {
        foundEntry = entry;
        break;
      }
    }
    verify(foundEntry !== null);
    compare(foundEntry.Name, "ComboRolesProbe");
    verify(foundEntry.LayerPointer !== null);
    compare(foundEntry.LayerPointer.id, layer.id);
  }

  function test_qfComboBoxBindsToMapLayerModelWithExpectedRoles() {
    const plugin = createTemporaryObject(layerComboBoxDialogPlugin, testCase);
    compare(plugin.layersComboBox.textRole, "Name");
    compare(plugin.layersComboBox.valueRole, "LayerPointer");
    compare(plugin.layersComboBox.model, plugin.layersModel);
  }

  function test_qfDialogOpensOnToolbarButtonClick() {
    const plugin = createTemporaryObject(layerComboBoxDialogPlugin, testCase);
    compare(plugin.layersDialog.visible, false);
    plugin.layersButton.clicked();
    tryCompare(plugin.layersDialog, "visible", true);
  }
}
