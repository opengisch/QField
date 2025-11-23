# QField API Documentation

Welcome to the QField API documentation page. QField brings the power of QGIS into the field and offers a flexible plugin framework that allows you to extend, customise, and automate your field workflows using QML and JavaScript.

---

## 🧩 Overview

QField offers two types of plugins:

- **Project Plugins** — Activated when a specific QGIS project is opened in QField.
- **App-Wide Plugins** — Loaded at application runtime and active across all projects.

The plugin system is based on Qt’s QML engine, making it fully cross-platform.

---

## ✅ Why Use QField Plugins

- Build **custom UI elements**, map tools, overlays, and automation.
- Implement **project-specific workflows** by bundling plugins with your `.qgs` / `.qgz` project.
- Deploy **organisation-wide extensions** through app-wide plugins.
- Automate field tasks such as:
  - taking a photo and automatically creating a feature,
  - running custom validity checks,
  - adding guided workflows to the map canvas.
- Extend QField without recompiling the app.

---

## 📁 Plugin Types & Deployment

### **Project Plugins**

Project plugins are placed next to the project file. The plugin must have the same filename as the project but use the `.qml` extension. Example:

- `birdwatcher.qgz`
- `birdwatcher.qml`

These project plugins are automatically discovered and included when packaging a project with QFieldSync or QFieldCloud. They are easy to distrubte through QFieldCloud and are ideal for project-based workflows supporting field campaigns.

### **App-Wide Plugins**

To develop an app-wide plugin, add a new folder in the QField's app directory's plugins subfolder. Its exact location can be discovered by opening the _About QField_ popup in QField and clicking on the QField app location label at the bottom of the screen.

The created plugin folder must contain a main.qml file which will be used by QField to activate the plugin.

An optional metadata.txt can also be used to provide basic details such as the plugin name, brief description, and author name.
A sample typical `metadata.txt` content would look like this:

```ini
[general]
name=Hello World Plugin
description=This is simple and brief description.
author=OPENGIS.ch
icon=logo.svg
```

To ship the plugin, compress the content of your plugin folder into a ZIP file. The main.qml must reside at the root of your ZIP file, while the ZIP file name itself should mirror your plugin folder name. The ZIP file name can contain a version suffix - such as 'my-plugin-v1.0.zip' - which QField will remove from the installed plugin folder name. The compressed ZIP file can then be installed in QField's plugin manager via the _Install plugin from URL_ button.

This [QField template plugin](https://github.com/opengisch/qfield-template-plugin) offers a simple skeleton from which you can build plugins from scratch.

---

## 🔎 What you'll find here

- **C++ Classes**
  Low-level APIs that implement QField's core functionality: map rendering, feature editing, project handling, position tracking, and more. Look for properties and invokable functions to know what is exposed to plugins.

- **QML Items**
  QML components used to build QField's user interface and tools. These are especially relevant if you are creating custom QML plugins, prototypes, or tailored builds of QField.

---

## 🧠 Core Interfaces

QField exposes an [`iface`](QField/classAppInterface.md) object and a set of utility modules.

### **Key `iface` functions**

- `iface.mainWindow()` – access to the main application window
- `iface.mapCanvas()` – retrieve the main [map canvas](QField/classMapCanvas.md) and [map settings](QField/classQgsQuickMapSettings.md)
- `iface.positioning()` – retrieve the [positioning](QField/classPositioning.md) object
- `iface.addItemToPluginsToolbar(item)` – add custom toolbar buttons overlays in the main window
- `iface.addItemToDashboardActionsToolbar(item)` – add actions to the side dashboard
- `iface.addItemToCanvasActionsToolbar(item)` – add actions to the map canvas long-press menu
- `iface.findItemByObjectName("name")` – access internal QField components using their objectName property

### **Utility objects**

Examples:

- `GeometryUtils.createGeometryFromWkt()`
- `FeatureUtils.createFeature()`

These allow plugins to create features, manipulate geometry, work with the camera, and more.

---

## 🛠 Getting Started

Minimal plugin example:

```qml
import QtQuick
import org.qfield

Item {
  Component.onCompleted: {
    iface.mainWindow().displayToast("Hello QField Plugin!")
  }
}
```

## 📘 Resources

- [Example plugins](https://github.com/topics/qfield-plugin){:target="_blank"}
- [Supercharge your fieldwork with QField’s project and app-wide plugins](https://www.opengis.ch/2024/06/18/supercharge-your-fieldwork-with-qfields-project-and-app-wide-plugins/){:target="_blank"}


## 🎯 Best Practices

- Use versioned ZIP filenames for app-wide plugins.
- Ensure project plugins are named exactly like the project file.
- Test plugins on desktop before deployment to mobile. PluginReloader plugin is your friend.
- Use plugins for workflow extensions, not for overriding QField core behaviour.
