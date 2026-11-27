import QtQuick
import QtTest
import org.qfield.core
import org.qfield.gui

TestCase {
  name: "QfPositioningNtripSettings"
  when: windowShown

  Item {
    id: mainWindow
    width: 400
    height: 800
    property Item contentItem: mainWindowContentItem
    property int sceneTopMargin: 0
    property int sceneBottomMargin: 0
    property int sceneLeftMargin: 0
    property int sceneRightMargin: 0

    Item {
      id: mainWindowContentItem
      anchors.fill: parent
    }
  }

  QfPositioningNtripSettings {
    id: ntripSettings
  }

  SignalSpy {
    id: applySpy
    target: ntripSettings
    signalName: "apply"
  }

  function init() {
    applySpy.clear();
    ntripSettings.updateFromNtripPresets([
      {
        "name": "Alpha",
        "settings": {
          "host": "alpha.example.com",
          "port": 2101,
          "mountPoint": "ALPHA"
        }
      },
      {
        "name": "Beta",
        "settings": {
          "host": "beta.example.com",
          "port": 2102,
          "mountPoint": "BETA"
        }
      }
    ], 0);
  }

  function editHost(host) {
    let settings = ntripSettings.createSettingsMap();
    settings.host = host;
    ntripSettings.updateFromNtripSettings(QfPositioningUtils.createNtripSettings(settings));
  }

  function test_01_loadsSelectedPreset() {
    compare(ntripSettings.createSettingsMap().host, "alpha.example.com");
    ntripSettings.updateFromNtripPresets(ntripSettings.presets, 1);
    compare(ntripSettings.presetIndex, 1);
    compare(ntripSettings.createSettingsMap().host, "beta.example.com");
    compare(ntripSettings.createSettingsMap().port, "2102");
    compare(ntripSettings.createSettingsMap().mountPoint, "BETA");
  }

  function test_02_switchingPresetsSavesEdits() {
    editHost("edited.example.com");
    ntripSettings.switchToPreset(1);
    compare(ntripSettings.presetIndex, 1);
    compare(ntripSettings.createSettingsMap().host, "beta.example.com");
    compare(ntripSettings.presets[0].settings.host, "edited.example.com");
    ntripSettings.switchToPreset(0);
    compare(ntripSettings.createSettingsMap().host, "edited.example.com");
  }

  function test_03_addedPresetStartsBlank() {
    editHost("edited.example.com");
    ntripSettings.addPreset("Gamma");
    compare(ntripSettings.presets.length, 3);
    compare(ntripSettings.presetIndex, 2);
    compare(ntripSettings.presets[2].name, "Gamma");
    compare(ntripSettings.presets[0].settings.host, "edited.example.com");
    compare(ntripSettings.createSettingsMap().host, "");
    compare(ntripSettings.createSettingsMap().port, "2101");
    compare(ntripSettings.createSettingsMap().mountPoint, "");
  }

  function test_04_renamingKeepsSettings() {
    ntripSettings.switchToPreset(1);
    ntripSettings.renamePreset("Renamed");
    compare(ntripSettings.presets.length, 2);
    compare(ntripSettings.presetIndex, 1);
    compare(ntripSettings.presets[1].name, "Renamed");
    compare(ntripSettings.presets[1].settings.host, "beta.example.com");
    compare(ntripSettings.createSettingsMap().host, "beta.example.com");
  }

  function test_05_removingPresetSelectsRemainingPreset() {
    ntripSettings.switchToPreset(1);
    ntripSettings.removePreset();
    compare(ntripSettings.presets.length, 1);
    compare(ntripSettings.presetIndex, 0);
    compare(ntripSettings.presets[0].name, "Alpha");
    compare(ntripSettings.createSettingsMap().host, "alpha.example.com");
  }

  function test_06_presetNameAvailability() {
    verify(!ntripSettings.isPresetNameAvailable("", -1));
    verify(!ntripSettings.isPresetNameAvailable("Beta", -1));
    verify(!ntripSettings.isPresetNameAvailable("Beta", 0));
    verify(ntripSettings.isPresetNameAvailable("Beta", 1));
    verify(ntripSettings.isPresetNameAvailable("Gamma", -1));
  }

  function test_07_closingSavesIntoSelectedPreset() {
    ntripSettings.switchToPreset(1);
    ntripSettings.open();
    tryCompare(ntripSettings, "opened", true);
    editHost("closed.example.com");
    ntripSettings.close();
    compare(applySpy.count, 1);
    compare(ntripSettings.presetIndex, 1);
    compare(ntripSettings.presets[1].settings.host, "closed.example.com");
    compare(ntripSettings.presets[0].settings.host, "alpha.example.com");
    tryCompare(ntripSettings, "visible", false);
  }

  function test_08_renameAndRemovingLastPresetRecreatesDefaultServer() {
    ntripSettings.switchToPreset(0);
    ntripSettings.renamePreset("Renamed");
    compare(ntripSettings.presets[0].name, "Renamed");
    const presetsLength = ntripSettings.presets.length;
    for (let i = presetsLength - 1; i >= 0; i--) {
      ntripSettings.switchToPreset(i);
      ntripSettings.removePreset();
    }
    compare(ntripSettings.presets[0].name, "Default server");
    compare(ntripSettings.createSettingsMap().host, "");
    compare(ntripSettings.createSettingsMap().port, "2101");
  }
}
