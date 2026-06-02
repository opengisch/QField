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
    property double sceneRightMargin: 0
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

  Item {
    id: canvasActionsToolbar
    objectName: "canvasMenuActionsToolbar"
  }

  QFieldControls.DashBoard {
    id: dashBoardItem
    objectName: "dashBoard"
  }

  QFieldControls.FeatureListForm {
    id: featureListForm
    objectName: "featureForm"

    model: MultiFeatureListModel {}
  }

  QFieldControls.MapCanvas {
    id: mapCanvas
    objectName: "mapCanvas"
  }

  QfMenu {
    id: canvasMenu
    objectName: "canvasMenu"

    property var point: GeometryUtils.point(0, 0)
  }

  QFieldControls.MapCanvasPointHandler {
    id: pointHandlerItem
    objectName: "pointHandler"
  }

  function init() {
    for (let i = pluginsToolbar.children.length - 1; i >= 0; --i) {
      pluginsToolbar.children[i].parent = null;
    }
    for (let i = canvasActionsToolbar.children.length - 1; i >= 0; --i) {
      canvasActionsToolbar.children[i].parent = null;
    }
    dashBoardItem.activeLayer = null;

    // Drop any registered handlers between tests
    pointHandlerItem.handlers = [];

    // Remove the "data" layer between tests so feature-iteration tests
    // don't see leftovers from prior runs.
    const staleData = qgisProject.mapLayersByName("data");
    for (const layer of staleData) {
      ProjectUtils.removeMapLayer(qgisProject, layer);
    }

    // Remove the "PluginLoadedLayer" between tests so add/remove tests
    // don't see leftovers from prior runs.
    const stalePluginLayers = qgisProject.mapLayersByName("PluginLoadedLayer");
    for (const layer of stalePluginLayers) {
      ProjectUtils.removeMapLayer(qgisProject, layer);
    }
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

  function makeCheckableMemoryLayer(name) {
    const existing = qgisProject.mapLayersByName(name);
    if (existing.length > 0) {
      return existing[0];
    }
    const fields = FeatureUtils.createFields([FeatureUtils.createField("id", FeatureUtils.Int), FeatureUtils.createField("name", FeatureUtils.String), FeatureUtils.createField("check", FeatureUtils.Bool)]);
    const layer = LayerUtils.createMemoryLayer(name, fields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    ProjectUtils.addMapLayer(qgisProject, layer);
    return layer;
  }

  function addCheckableFeature(layer, idValue, nameValue, checkValue) {
    layer.startEditing();
    const feature = FeatureUtils.createBlankFeature(layer.fields);
    feature.setAttribute(layer.fields.indexOf("id"), idValue);
    feature.setAttribute(layer.fields.indexOf("name"), nameValue);
    feature.setAttribute(layer.fields.indexOf("check"), checkValue);
    LayerUtils.addFeature(layer, feature);
    layer.commitChanges();
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

  function findCanvasActionButtonByText(text) {
    for (let i = 0; i < canvasActionsToolbar.children.length; ++i) {
      const child = canvasActionsToolbar.children[i];
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

  // Feature iteration and list population via plugin toolbar buttons

  Component {
    id: featureIterationPlugin

    Item {
      id: plugin

      property var mainWindow: iface.mainWindow()
      property var featureListForm: iface.findItemByObjectName("featureForm")

      Component.onCompleted: {
        iface.addItemToPluginsToolbar(iterateButton);
        iface.addItemToPluginsToolbar(listButton);
      }

      QfToolButton {
        id: iterateButton
        text: "!"
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true
        onClicked: {
          let layer = qgisProject.mapLayersByName("data")[0];
          layer.startEditing();
          let it = LayerUtils.createFeatureIteratorFromExpression(layer, "\"check\" = true");
          if (it.hasNext()) {
            const feature = it.next();
            layer.changeAttributeValue(feature.id, layer.fields.indexOf("name"), "modified!");
          }
          delete it;
          layer.commitChanges();
        }
      }

      QfToolButton {
        id: listButton
        text: "..."
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true
        onClicked: {
          let layer = qgisProject.mapLayersByName("data")[0];
          featureListForm.model.setFeatures(layer, "");
        }
      }
    }
  }

  function test_iterationPluginRegistersBothToolbarButtons() {
    createTemporaryObject(featureIterationPlugin, testCase);
    compare(pluginsToolbar.children.length, 2);
    compare(pluginsToolbar.children[0].text, "!");
    compare(pluginsToolbar.children[1].text, "...");
  }

  function test_ifaceFindItemByObjectNameResolvesFeatureForm() {
    const plugin = createTemporaryObject(featureIterationPlugin, testCase);
    verify(plugin.featureListForm !== null);
    compare(plugin.featureListForm.objectName, "featureForm");
  }

  function test_layerUtilsFeatureIteratorFiltersByExpression() {
    const layer = makeCheckableMemoryLayer("data");
    addCheckableFeature(layer, 1, "FeatureA", true);
    addCheckableFeature(layer, 2, "FeatureB", false);
    addCheckableFeature(layer, 3, "FeatureC", true);

    const it = LayerUtils.createFeatureIteratorFromExpression(layer, "\"check\" = true");
    const collected = [];
    while (it.hasNext()) {
      collected.push(it.next().attribute("name"));
    }
    compare(collected.length, 2);
    verify(collected.indexOf("FeatureA") !== -1);
    verify(collected.indexOf("FeatureC") !== -1);
  }

  function test_iterateButtonClickMutatesMatchingFeatureName() {
    const layer = makeCheckableMemoryLayer("data");
    addCheckableFeature(layer, 1, "FeatureA", true);
    addCheckableFeature(layer, 2, "FeatureB", false);

    createTemporaryObject(featureIterationPlugin, testCase);
    findToolbarButtonByText("!").clicked();

    const it = LayerUtils.createFeatureIteratorFromExpression(layer, "\"id\" = 1");
    verify(it.hasNext());
    compare(it.next().attribute("name"), "modified!");
  }

  function test_listButtonClickPopulatesFeatureFormModel() {
    const layer = makeCheckableMemoryLayer("data");
    addCheckableFeature(layer, 1, "FeatureA", true);
    addCheckableFeature(layer, 2, "FeatureB", false);

    createTemporaryObject(featureIterationPlugin, testCase);
    findToolbarButtonByText("...").clicked();

    compare(featureListForm.model.count, 2);
  }

  // Feature insertion at canvas-menu point via canvas action buttons

  Component {
    id: featureInsertionPlugin

    Item {
      id: plugin

      property var mainWindow: iface.mainWindow()
      property var mapCanvas: iface.mapCanvas()
      property var canvasMenu: iface.findItemByObjectName("canvasMenu")
      property var dashBoard: iface.findItemByObjectName("dashBoard")

      Component.onCompleted: {
        iface.addItemToCanvasActionsToolbar(insertOtherButton);
      }

      QfToolButton {
        id: insertOtherButton
        text: "O"
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true
        onClicked: {
          dashBoard.activeLayer = qgisProject.mapLayersByName("OtherLayer")[0];
          const point = GeometryUtils.reprojectPoint(canvasMenu.point, mapCanvas.mapSettings.destinationCrs, dashBoard.activeLayer.crs);
          const wkt = "POINT(" + point.x + " " + point.y + ")";
          const geom = GeometryUtils.createGeometryFromWkt(wkt);
          featureModel.currentLayer = dashBoard.activeLayer;
          featureModel.resetFeature();
          featureModel.resetAttributes();
          featureModel.changeGeometry(geom);
          featureModel.create();
        }
      }

      FeatureModel {
        id: featureModel
      }
    }
  }

  function test_insertionPluginRegistersCanvasActionButton() {
    createTemporaryObject(featureInsertionPlugin, testCase);
    compare(canvasActionsToolbar.children.length, 1);
    compare(canvasActionsToolbar.children[0].text, "O");
  }

  function test_ifaceMapCanvasResolvesFromContext() {
    const plugin = createTemporaryObject(featureInsertionPlugin, testCase);
    verify(plugin.mapCanvas !== null);
    compare(plugin.mapCanvas.objectName, "mapCanvas");
  }

  function test_insertButtonClickPersistsFeatureAtReprojectedPoint() {
    makeMemoryLayer("OtherLayer");
    canvasMenu.point = GeometryUtils.point(10, 20);
    createTemporaryObject(featureInsertionPlugin, testCase);
    findCanvasActionButtonByText("O").clicked();
    const layer = qgisProject.mapLayersByName("OtherLayer")[0];
    const it = LayerUtils.createFeatureIterator(layer);
    verify(it.hasNext());
    const feature = it.next();
    const geom = feature.geometry;
    compare(geom.asWkt(5), "Point (10 20)");
  }

  // Add and remove a project layer via plugin toolbar buttons

  Component {
    id: layerAddRemovePlugin

    Item {
      id: plugin

      property var mainWindow: iface.mainWindow()
      property var myLayer: undefined

      Component.onCompleted: {
        iface.addItemToPluginsToolbar(addButton);
        iface.addItemToPluginsToolbar(removeButton);
      }

      QfToolButton {
        id: addButton
        text: "+"
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true
        onClicked: {
          if (plugin.myLayer === undefined) {
            const fields = FeatureUtils.createFields([FeatureUtils.createField("id", FeatureUtils.Int)]);
            plugin.myLayer = LayerUtils.createMemoryLayer("PluginLoadedLayer", fields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
            ProjectUtils.addMapLayer(qgisProject, plugin.myLayer);
          }
        }
      }

      QfToolButton {
        id: removeButton
        text: "-"
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundColor
        round: true
        onClicked: {
          if (plugin.myLayer !== undefined) {
            ProjectUtils.removeMapLayer(qgisProject, plugin.myLayer.id);
            plugin.myLayer = undefined;
          }
        }
      }
    }
  }

  function test_addRemovePluginRegistersBothToolbarButtons() {
    createTemporaryObject(layerAddRemovePlugin, testCase);
    compare(pluginsToolbar.children.length, 2);
    compare(pluginsToolbar.children[0].text, "+");
    compare(pluginsToolbar.children[1].text, "-");
  }

  function test_addButtonClickRegistersLayerInProject() {
    createTemporaryObject(layerAddRemovePlugin, testCase);
    findToolbarButtonByText("+").clicked();
    compare(qgisProject.mapLayersByName("PluginLoadedLayer").length, 1);
  }

  function test_removeButtonClickRemovesLayerByIdFromProject() {
    createTemporaryObject(layerAddRemovePlugin, testCase);
    findToolbarButtonByText("+").clicked();
    compare(qgisProject.mapLayersByName("PluginLoadedLayer").length, 1);
    findToolbarButtonByText("-").clicked();
    compare(qgisProject.mapLayersByName("PluginLoadedLayer").length, 0);
  }

  // Feature lookup at canvas click via MapCanvasPointHandler

  Component {
    id: canvasClickFeatureLookupPlugin

    Item {
      id: plugin

      property var mainWindow: iface.mainWindow()
      property var pointHandler: iface.findItemByObjectName("pointHandler")
      property string handlerName: "canvas_click_feature_lookup_plugin"

      Component.onCompleted: {
        pointHandler.registerHandler(plugin.handlerName, (point, type, interactionType) => {
          if (interactionType === "clicked") {
            return true;
          }
          return false;
        });
      }

      Component.onDestruction: {
        if (pointHandler) {
          pointHandler.deregisterHandler(plugin.handlerName);
        }
      }
    }
  }

  function test_pointHandlerPluginRegistersAndDeregistersHandler() {
    compare(pointHandlerItem.handlers.length, 0);
    const plugin = createTemporaryObject(canvasClickFeatureLookupPlugin, testCase);
    compare(pointHandlerItem.handlers.length, 1);
    compare(pointHandlerItem.handlers[0].name, "canvas_click_feature_lookup_plugin");
    plugin.destroy();
    wait(50);
    compare(pointHandlerItem.handlers.length, 0);
  }

  function test_pointHandlerClickedDispatchesToRegisteredHandler() {
    let receivedInteractionType = null;
    pointHandlerItem.registerHandler("dispatch_probe", (point, type, interactionType) => {
      receivedInteractionType = interactionType;
      return true;
    });
    const consumed = pointHandlerItem.clicked(Qt.point(10, 20), "stylus");
    compare(consumed, true);
    compare(receivedInteractionType, "clicked");
  }

  function test_layerUtilsFeatureIteratorFiltersByRectangle() {
    const layer = makeCheckableMemoryLayer("data");

    layer.startEditing();
    for (let i = 1; i <= 2; ++i) {
      const feature = FeatureUtils.createBlankFeature(layer.fields, GeometryUtils.createGeometryFromWkt("POINT(" + i + " " + i + ")"));
      feature.setAttribute(layer.fields.indexOf("id"), i);
      feature.setAttribute(layer.fields.indexOf("name"), "Feature" + String.fromCharCode(64 + i));
      feature.setAttribute(layer.fields.indexOf("check"), true);
      verify(LayerUtils.addFeature(layer, feature));
    }
    layer.commitChanges();

    const rectangle = GeometryUtils.createRectangleFromPoints(GeometryUtils.point(0.5, 0.5), GeometryUtils.point(1.5, 1.5));
    const matches = LayerUtils.createFeatureIteratorFromRectangle(layer, rectangle);
    const collected = [];
    while (matches.hasNext()) {
      collected.push(matches.next().attribute("name"));
    }
    compare(collected.length, 1);
    compare(collected[0], "FeatureA");
  }
}
