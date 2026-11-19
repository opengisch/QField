import platform
import time

import pyautogui
import pytest


@pytest.fixture
def isolated_app(tmp_path, monkeypatch, request):
    monkeypatch.setenv("XDG_CONFIG_HOME", str(tmp_path))
    return request.getfixturevalue("app")


@pytest.mark.skipif(
    platform.system() == "Darwin", reason="PyAutoGUI does not work on macOS"
)
def test_ntrip_profiles_can_be_selected_and_edited(isolated_app):
    app = isolated_app
    settings = "mainWindow/qfieldSettings"
    combo = "mainWindow/ntripProfileComboBox"
    header = "mainWindow/positioningNtripSettingsPageHeader"

    def click(path):
        bounds = app.getBoundingBox(path)
        fail_safe = pyautogui.FAILSAFE
        pyautogui.FAILSAFE = False
        try:
            pyautogui.click(bounds[0] + bounds[2] / 2, bounds[1] + bounds[3] / 2)
        finally:
            pyautogui.FAILSAFE = fail_safe
        time.sleep(0.2)

    def add_profile(name, host, mount_point):
        click(f"{settings}/addNtripProfileButton")
        assert app.existsAndVisible("mainWindow/ntripProfileNameTextField")
        app.setStringProperty("mainWindow/ntripProfileNameTextField", "text", name)
        app.setStringProperty("mainWindow/ntripHostTextField", "text", host)
        app.setStringProperty("mainWindow/ntripPortTextField", "text", "2101")
        app.setStringProperty(
            "mainWindow/ntripMountPointComboBox", "editText", mount_point
        )
        click(f"{header}/applyButton")
        assert app.getStringProperty(combo, "currentText") == name

    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    app.setStringProperty(settings, "currentPanel", "1")
    app.setStringProperty(settings, "visible", "true")
    time.sleep(0.5)
    app.setStringProperty("mainWindow/positioningFlickable", "contentY", "220")
    time.sleep(0.2)

    add_profile("Caster A", "a.example.org", "MOUNT_A")
    add_profile("Caster B", "b.example.org", "MOUNT_B")

    click(f"{settings}/editNtripProfileButton")
    assert (
        app.getStringProperty("mainWindow/ntripHostTextField", "text")
        == "b.example.org"
    )
    app.setStringProperty(
        "mainWindow/ntripHostTextField", "text", "discard.example.org"
    )
    click(f"{header}/cancelButton")

    app.setStringProperty(combo, "currentIndex", "0")
    app.invokeMethod(combo, "activated", [0])
    assert not any("InvokeMethod" in error for error in app.getErrors())
    assert app.getStringProperty(combo, "currentText") == "Caster A"

    click(f"{settings}/editNtripProfileButton")
    assert (
        app.getStringProperty("mainWindow/ntripHostTextField", "text")
        == "a.example.org"
    )
    assert (
        app.getStringProperty("mainWindow/ntripMountPointComboBox", "editText")
        == "MOUNT_A"
    )
    app.setStringProperty(
        "mainWindow/ntripProfileNameTextField", "text", "Caster A updated"
    )
    click(f"{header}/applyButton")
    assert app.getStringProperty(combo, "currentText") == "Caster A updated"

    click(f"{settings}/removeNtripProfileButton")
    assert app.getStringProperty(combo, "currentText") == "Caster B"
    click(f"{settings}/editNtripProfileButton")
    assert (
        app.getStringProperty("mainWindow/ntripHostTextField", "text")
        == "b.example.org"
    )
    assert (
        app.getStringProperty("mainWindow/ntripMountPointComboBox", "editText")
        == "MOUNT_B"
    )
    click(f"{header}/cancelButton")
    click(f"{settings}/removeNtripProfileButton")
    assert app.getStringProperty(combo, "currentText") == ""
