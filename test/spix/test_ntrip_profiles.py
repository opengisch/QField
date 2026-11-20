import platform
import select
import subprocess
import sys
import time
from pathlib import Path

import pytest


@pytest.fixture
def isolated_app(tmp_path, monkeypatch, request):
    monkeypatch.setenv("XDG_CONFIG_HOME", str(tmp_path))
    return request.getfixturevalue("app")


def click(app, path):
    app.invokeMethod(path, "clicked", [])
    time.sleep(0.2)


def add_profile(app, name, host, port, mount_point):
    click(app, "mainWindow/qfieldSettings/addNtripProfileButton")
    assert app.existsAndVisible("mainWindow/ntripProfileNameTextField")
    app.setStringProperty("mainWindow/ntripProfileNameTextField", "text", name)
    app.setStringProperty("mainWindow/ntripHostTextField", "text", host)
    app.setStringProperty("mainWindow/ntripPortTextField", "text", str(port))
    app.setStringProperty("mainWindow/ntripUsernameTextField", "text", "rover")
    app.setStringProperty("mainWindow/ntripPasswordTextField", "text", "test")
    app.setStringProperty("mainWindow/ntripMountPointComboBox", "editText", mount_point)
    click(app, "mainWindow/positioningNtripSettingsPageHeader/applyButton")
    assert (
        app.getStringProperty("mainWindow/ntripProfileComboBox", "currentText") == name
    )


def test_ntrip_profiles_can_be_selected_and_edited(isolated_app):
    app = isolated_app
    settings = "mainWindow/qfieldSettings"
    combo = "mainWindow/ntripProfileComboBox"
    header = "mainWindow/positioningNtripSettingsPageHeader"

    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    app.setStringProperty(settings, "currentPanel", "1")
    app.setStringProperty(settings, "visible", "true")
    time.sleep(0.5)
    app.setStringProperty("mainWindow/positioningFlickable", "contentY", "220")
    time.sleep(0.2)

    add_profile(app, "Caster A", "a.example.org", 2101, "MOUNT_A")
    add_profile(app, "Caster B", "b.example.org", 2101, "MOUNT_B")

    click(app, f"{settings}/editNtripProfileButton")
    assert (
        app.getStringProperty("mainWindow/ntripHostTextField", "text")
        == "b.example.org"
    )
    app.setStringProperty(
        "mainWindow/ntripHostTextField", "text", "discard.example.org"
    )
    click(app, f"{header}/cancelButton")

    app.setStringProperty(combo, "currentIndex", "0")
    app.invokeMethod(combo, "activated", [0])
    assert not any("InvokeMethod" in error for error in app.getErrors())
    assert app.getStringProperty(combo, "currentText") == "Caster A"

    click(app, f"{settings}/editNtripProfileButton")
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
    click(app, f"{header}/applyButton")
    assert app.getStringProperty(combo, "currentText") == "Caster A updated"

    click(app, f"{settings}/removeNtripProfileButton")
    assert app.getStringProperty(combo, "currentText") == "Caster B"
    click(app, f"{settings}/editNtripProfileButton")
    assert (
        app.getStringProperty("mainWindow/ntripHostTextField", "text")
        == "b.example.org"
    )
    assert (
        app.getStringProperty("mainWindow/ntripMountPointComboBox", "editText")
        == "MOUNT_B"
    )
    click(app, f"{header}/cancelButton")
    click(app, f"{settings}/removeNtripProfileButton")
    assert app.getStringProperty(combo, "currentText") == ""


@pytest.mark.skipif(platform.system() != "Linux", reason="Requires RTKLIB")
def test_ntrip_profiles_switch_casters(isolated_app):
    app = isolated_app
    fixture = Path(__file__).parent.parent / "virtual_rover"
    rover = subprocess.Popen(
        [
            sys.executable,
            str(fixture / "virtual_rover.py"),
            str(fixture / "rtklib-rover-base.tar.gz"),
            "on",
            "fixed",
            "--casters",
            "2",
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        assert select.select([rover.stdout], [], [], 5)[0]
        ports = rover.stdout.readline().split()
        assert len(ports) == 3

        app.invokeMethod("mainWindow/toursController", "blockGuides", [])
        settings = "mainWindow/qfieldSettings"
        combo = "mainWindow/ntripProfileComboBox"
        app.setStringProperty(settings, "currentPanel", "1")
        app.setStringProperty(settings, "visible", "true")
        time.sleep(0.5)
        app.setStringProperty("mainWindow/positioningFlickable", "contentY", "220")
        add_profile(app, "Caster A", "127.0.0.1", ports[1], "RTK1")
        add_profile(app, "Caster B", "127.0.0.1", ports[2], "RTK2")

        app.setStringProperty(combo, "currentIndex", "0")
        app.invokeMethod(combo, "activated", [0])
        click(app, f"{settings}/addPositioningDeviceButton")
        app.setStringProperty("mainWindow/positioningDeviceType", "currentIndex", "1")
        assert (
            app.getStringProperty("mainWindow/positioningDeviceType", "currentText")
            == "TCP (NMEA)"
        )
        app.setStringProperty("mainWindow/tcpDevicePort", "text", ports[0])
        click(app, "mainWindow/positioningDeviceSettingsPageHeader/applyButton")
        click(app, f"{settings}/connectButton")
        switch = f"{settings}/enableNtripClient"
        deadline = time.monotonic() + 10
        while not app.existsAndVisible(switch) and time.monotonic() < deadline:
            time.sleep(0.1)
        assert app.existsAndVisible(switch)
        click(app, switch)
        assert select.select([rover.stdout], [], [], 35)[0]
        assert rover.stdout.readline().strip() == "Caster 1 complete"

        app.setStringProperty(combo, "currentIndex", "1")
        app.invokeMethod(combo, "activated", [1])
        assert app.getStringProperty(combo, "currentText") == "Caster B"
        click(app, switch)
        assert select.select([rover.stdout], [], [], 35)[0]
        assert rover.stdout.readline().strip() == "Caster 2 complete"
        assert rover.wait(timeout=5) == 0
    finally:
        if rover.poll() is None:
            rover.terminate()
            rover.wait(timeout=5)
