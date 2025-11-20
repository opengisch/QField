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

- Placed next to the project file.
- The plugin must have the same filename as the project but use the `.qml` extension.
  - Example:
    - `birdwatcher.qgz`
    - `birdwatcher.qml`
- Automatically discovered and included when packaging a project with QFieldSync or QFieldCloud.
- Ideal for project-based workflows and field campaigns.

### **App-Wide Plugins**

- Installed through QField’s plugin manager using a ZIP file URL.
- A minimal plugin requires:
  - `main.qml`
  - optional `metadata.txt`
- ZIP file naming can include versions, e.g.:
  - `myplugin-0.2.1.zip`
- Used for enterprise-wide or reusable functionality independent of any single project.

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
