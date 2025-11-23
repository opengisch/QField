# QField Plugin Code Snippets

Below are useful code snippets that will teach you ways to work within the plugin framework.

## Add emergency phone call action

Mobile devices can handle several useful hyperlinks protocols such as `tel:` and `mailto:`. The code below will add a tool button on the map canvas offering field works a quick way to call an emergency number using the `Qt.openUrlExternally` function:

```qml
import QtQuick
import org.qfield
import Theme

Item {
  QfToolButton {
    id: emergencyCall

    iconSource: Theme.getThemeVectorIcon("ic_info_white_24dp")
    iconColor: Theme.toolButtonColor
    bgcolor: Theme.toolButtonBackgroundColor
    round: true

    onClicked: {
      Qt.openUrlExternally('tel:+1800123456');
    }
  }

  Component.onCompleted: {
    iface.addItemToPluginsToolbar(emergencyCall)
  }
}
```

## Change the map canvas center or extent

This code below will re-center the map canvas around a WGS84 point reprojected to match the current project CRS by using [`GeometryUtils`](QField/classGeometryUtils.md) functions.

```qml
import QtQuick
import org.qfield
import Theme

Item {
  QfToolButton {
    id: gotoPoint

    iconSource: Theme.getThemeVectorIcon("ic_geotag_white_24dp")
    iconColor: Theme.toolButtonColor
    bgcolor: Theme.toolButtonBackgroundColor
    round: true

    onClicked: {
      const point = GeometryUtils.point(174.7656025, -36.8533493);
      const projectedPoint = GeometryUtils.reprojectPoint(point, CoordinateReferenceSystemUtils.wgs84Crs(), qgisProject.crs);
      iface.mapCanvas().mapSettings.center = projectedPoint;
    }
  }

  Component.onCompleted: {
    iface.addItemToPluginsToolbar(gotoPoint)
  }
}
```

It's also possible to use an extent instead of a point to be in control of the scale. The `onClicked` function would be changed to this instead:

```qml
    onClicked: {
      const point1 = GeometryUtils.point(174.7646543, -36.8519053);
      const point2 = GeometryUtils.point(174.7682343, -36.8542590);
      const extent = GeometryUtils.createRectangleFromPoints(point1, point2);
      const projectedExtent = GeometryUtils.reprojectRectangle(extent, CoordinateReferenceSystemUtils.wgs84Crs(), qgisProject.crs);
      iface.mapCanvas().mapSettings.extent = projectedExtent;
    }
```

## Fetch the current position from an active GNSS device

One of the basic functionality you will likely seek is the ability to retrieve the current position from an active GNSS device. This code will copy the current position inside your device's clipboard:

```qml
import QtQuick
import org.qfield
import Theme

Item {
  QfToolButton {
    id: copyPositionToClipboard

    iconSource: Theme.getThemeVectorIcon("ic_copy_black_24dp")
    iconColor: Theme.toolButtonColor
    bgcolor: Theme.toolButtonBackgroundColor
    round: true

    onClicked: {
      let positioning = iface.findItemByObjectName('positionSource');
      if (positioning.active) {
        const info = positioning.positionInformation;
        if (info.longitudeValid && info.latitudeValid) {
          const text = info.latitude + ', ' + info.longitude;
          platformUtilities.copyTextToClipboard(text);
          iface.mainWindow().displayToast('Position copied into the clipboard');
        }
      }
    }
  }

  Component.onCompleted: {
    iface.addItemToPluginsToolbar(copyPositionToClipboard)
  }
}
```

## Search bar integration

The plugin framework empowers you to integrate custom searches into the QField search bar through the [`QFieldLocatorFilter`](QField/classQFieldLocatorFilter.md) item which can be added into a plugin's root item:

```qml
QFieldLocatorFilter {
  id: locatorFilter

  delay: 1000
  name: "unique_filter_string"
  displayName: "Plugin filter"
  prefix: "plug"
  locatorBridge: iface.findItemByObjectName('locatorBridge')

  parameters: { "parameter_1": "value_1" }
  source: Qt.resolvedUrl('search.qml')

  function triggerResult(result) {
    // result is a QgsLocatorResult object with the following properties:
    // - description, displayString, group, groupScore, score, and userData
  }

  function triggerResultFromAction(result, actionId) {
    // additional actions handled here
  }
}
```

The source property refers to a QML source file which will hold the logic to execute searches and pass on results.
It will be executed off the main thread to allow for non-blocking result fetching operations.

Here's a simple search QML source code:

```qml
import QtQuick
import org.qfield

Item {
  signal prepareResult(var details)
  signal fetchResultsEnded()

  function fetchResults(string, context, parameters) {
    // string is the search term(s) typed into the search bar
    // context is a QgsLocatorContext object with the following properties:
    // - targetExtent, targetExtentCrs, and transformContext
    // parameters is a map of keys and values attached to the QFieldLocatorFilter parameters properties

    // sample details object
    let result_details = {
      "userData": { "key": "value" },
      "displayString": "display string",
      "description": "description",
      "score": 1,
      "group": "group name",
      "groupScore":1,
      "actions":[{"id": 2, "name": "action", "icon": "icon.svg"}]
    }

    // prepareResult is the signal needed to pass on a result
    // you can trigger the signal as many times as you need to pass on all available results
    prepareResult(result_details);
  }
}
```

This [QField OpenStreetMap Nomination plugin](https://github.com/opengisch/qfield-nominatim-locator) is a good example to learn more about search bar integration.

## Configuration button within the plugin manager

For plugins requiring user configuration, QField allows for these to add a configuration button within its plugin manager.

To do so, you can simply add a `function configure()` invocable function attached to the plugin's root item:

```qml
import QtQuick
import org.qfield

Item {
  // ...

  function configure()
  {
    optionDialog.open();
  }

  Dialog {
    id: optionDialog
    parent: iface.mainWindow().contentItem
    visible: false
    modal: true
    title: "Configuration"

    // ...
  }
}
```

## Flash geometries on top of the map canvas

Plugins can make use of QField's [`GeometryHighlighter`](QField/classGeometryHighlighter.md) item to flash created or fetched geometries through the following code:

```qml
import QtQuick
import org.qfield

Item {
  // ...

  property var geometryHighlighter: iface.findItemByObjectName('geometryHighlighter')

  function demo() {
    // Flash Null Island geometry
    let geom = GeometryUtils.createGeometryFromWkt("POINT(0 0)")
    let crs = CoordinateReferenceSystemUtils.fromDescription("EPSG:4326")

    geometryHighlighter.geometryWrapper.qgsGeometry = geometry
    geometryHighlighter.geometryWrapper.crs = crs;
  }
}
```

## Add a project variable through dialog input

This example demonstrates how to check for a given project variable on project load and asks the user for a value if not present or empty through [`ExpressionContextUtils`](QField/classExpressionContextUtils.md) functions. The plugin also uses the projectInfo object to save the variable across sessions.

```qml
import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

Item {
  Connections {
    target: iface

    function onLoadProjectEnded() {
      const variables = ExpressionContextUtils.projectVariables(qgisProject);
      if (variables["favorite_icecream"] === undefined || variables["favorite_icecream"] === "") {
        favoriteIceCreamDialog.open();
      }
    }
  }

  QfDialog {
    id: favoriteIceCreamDialog
    parent: iface.mainWindow().contentItem

    width: 300
    height: 180
    x: (iface.mainWindow().width - width) / 2
    y: (iface.mainWindow().height - height) / 2
    title: "What is your favorite ice cream?"

    Column {
      id: columnLayout
      width: parent.width

      TextField {
        id: favoriteIceCreamField
        width: parent.width
        text: ""
      }
    }

    onAccepted: {
      ExpressionContextUtils.setProjectVariable(qgisProject, "favorite_icecream", favoriteIceCreamField.text);
      let projectInfo = iface.findItemByObjectName("projectInfo");
      projectInfo.saveVariable("favorite_icecream", favoriteIceCreamField.text);
    }
  }
}
```

## Register a map canvas point handler to take over taps

Plugins can take over map canvas taps behavior by registering an handler with the [`MapCanvasPointHandler`](QField/classMapCanvasPointHandler.md) object. The code snippet below will look for features within a given layer and return an attribute instead of showing the features list. Feature iteration is done using functions provided by the [`LayerUtils`](QField/classLayerUtils.md) class.

```qml
pointHandler.registerHandler("feature_iteration_example", (point, type, interactionType) => {
  if (interactionType === "clicked") {
    let mapCanvas = iface.mapCanvas();
    const tl = mapCanvas.mapSettings.screenToCoordinate(Qt.point(point.x - 4, point.y - 4));
    const br = mapCanvas.mapSettings.screenToCoordinate(Qt.point(point.x + 4, point.y + 4));

    const rectangle = GeometryUtils.createRectangleFromPoints(tl, br);
    let it = LayerUtils.createFeatureIteratorFromRectangle(qgisProject.mapLayersByName("my_layer")[0], rectangle);
    if (it.hasNext()) {
      const feature = it.next()
      mainWindow.displayToast("Feature found! It's name is " + feature.attribute("name"))
    }
    return true;
  }
  return false;
});
```

By returning true, we are telling QField that we have taken over the handling and provided the desired action.
