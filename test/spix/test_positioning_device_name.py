import platform
import time
import xmlrpc.client

import pyautogui
import pytest


@pytest.mark.skipif(
    platform.system() == "Darwin", reason="PyAutoGUI does not work on macOS"
)
def test_tcp_device_name_updates_after_edit(app):
    def click(path):
        bounds = app.getBoundingBox(path)
        fail_safe = pyautogui.FAILSAFE
        pyautogui.FAILSAFE = False
        try:
            pyautogui.click(bounds[0] + bounds[2] / 2, bounds[1] + bounds[3] / 2)
        finally:
            pyautogui.FAILSAFE = fail_safe
        time.sleep(0.2)

    settings = "mainWindow/qfieldSettings"
    device_name = "mainWindow/positioningDeviceName"
    device_type = "mainWindow/positioningDeviceType"
    device_address = "mainWindow/tcpDeviceAddress"
    device_port = "mainWindow/tcpDevicePort"
    device_combo = "mainWindow/positioningDeviceComboBox"
    header = "mainWindow/positioningDeviceSettingsPageHeader"
    old_name = "127.0.0.1 (9001)"
    new_name = "127.0.0.2 (9002)"

    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    app.setStringProperty("mainWindow", "width", "800")
    app.setStringProperty("mainWindow", "height", "700")
    app.setStringProperty(settings, "currentPanel", "1")
    app.setStringProperty(settings, "visible", "true")
    time.sleep(0.5)

    try:
        click(f"{settings}/addPositioningDeviceButton")
        assert app.existsAndVisible(device_name)

        for index in range(7):
            app.setStringProperty(device_type, "currentIndex", str(index))
            if app.getStringProperty(device_type, "currentText").startswith("TCP"):
                break
        else:
            assert False, "TCP device type not available"

        app.setStringProperty(device_address, "text", "127.0.0.1")
        app.setStringProperty(device_port, "text", "9001")
        click(f"{header}/applyButton")

        assert app.getStringProperty(device_combo, "currentText") == old_name
        assert (
            app.getStringProperty("mainWindow/connectButton", "text")
            == f"Connect to {old_name}"
        )

        click(f"{settings}/editPositioningDeviceButton")
        assert app.getStringProperty(device_name, "text") == ""
        app.setStringProperty(device_address, "text", "127.0.0.2")
        app.setStringProperty(device_port, "text", "9002")
        click(f"{header}/applyButton")

        assert app.getStringProperty(device_combo, "currentText") == new_name
        assert (
            app.getStringProperty("mainWindow/connectButton", "text")
            == f"Connect to {new_name}"
        )

        click(f"{settings}/editPositioningDeviceButton")
        assert app.getStringProperty(device_address, "text") == "127.0.0.2"
        assert app.getStringProperty(device_port, "text") == "9002"
    finally:
        if app.existsAndVisible(f"{header}/cancelButton"):
            click(f"{header}/cancelButton")
        if app.getStringProperty(device_combo, "currentText") in (old_name, new_name):
            click(f"{settings}/removePositioningDeviceButton")
