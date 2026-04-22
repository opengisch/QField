#!/usr/bin/env python

import pytest
from pytest_html import extras
import xmlrpc.client
import time
import subprocess
import sys
import time
import os
import shutil
import platform
from pathlib import Path
from PIL import Image
import pyautogui


@pytest.fixture
def screenshot_path():
    """
    Returns the path for a folder to put screenshots into.
    Supposed to be in the path where pytest-html writes its report.
    """
    img_path = os.path.join(os.getcwd(), "report", "images")
    print(f"Images will be written to {img_path}")
    os.makedirs(img_path, exist_ok=True)
    return img_path


@pytest.fixture
def diff_path():
    """
    Returns the path for a folder to put image comparison errors into.
    Supposed to be in the path where pytest-html writes its report.
    """
    img_path = os.path.join(os.getcwd(), "report", "diffs")
    print(f"Images will be written to {img_path}")
    os.makedirs(img_path, exist_ok=True)
    return img_path


@pytest.fixture
def screenshot_check(image_diff, image_diff_dir, screenshot_path, diff_path, extra):
    def inner(test_name, image_name, diff_threshold):
        # insure no alpha channel present in the screenshot being compared
        png = Image.open(
            os.path.join(screenshot_path, "{}.png".format(image_name))
        ).convert("RGB")
        png.save(os.path.join(screenshot_path, "{}.png".format(image_name)))

        expected_name = str(
            Path(__file__).parent.parent
            / "testdata"
            / "control_images"
            / test_name
            / "expected_{}-{}.png".format(image_name, platform.system())
        )
        if not os.path.isfile(expected_name):
            expected_name = str(
                Path(__file__).parent.parent
                / "testdata"
                / "control_images"
                / test_name
                / "expected_{}.png".format(image_name)
            )
        result = image_diff(
            os.path.join(screenshot_path, "{}.png".format(image_name)),
            expected_name,
            threshold=diff_threshold,
            suffix=image_name,
            throw_exception=False,
        )
        if os.path.exists(result.diff_path):
            shutil.copyfile(
                result.diff_path,
                os.path.join(diff_path, "{}_diff.png".format(image_name)),
            )
            extra.append(
                extras.html(
                    "Screenshot check for {}: diff value {}, threshold {}".format(
                        image_name, result.value, result.thresh
                    )
                )
            )
            extra.append(
                extras.html('<img src="diffs/{}_diff.png"/>'.format(image_name))
            )
        return result.value < result.thresh

    return inner


def test_start_app(app, screenshot_path, extra, process_alive):
    """
    Starts a test app to the welcome screen and creates a screenshot.
    """
    assert app.existsAndVisible("mainWindow")

    time.sleep(1)

    app.invokeMethod("mainWindow/toursController", "blockGuides", [])

    time.sleep(1)

    app.takeScreenshot("mainWindow", os.path.join(screenshot_path, "startup.png"))
    assert process_alive()
    extra.append(extras.html('<img src="images/startup.png"/>'))


@pytest.mark.skipif(
    platform.system() != "Linux",
    reason="PostGIS test requires a docker linux container",
)
@pytest.mark.project_file("test_wms.qgz")
def test_wms_layer(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and check for WMS layer support (including rendering check and message logs).
    This also tests that QField is able to reach QGIS's crucial srs.db.
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    time.sleep(4)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_wms_layer.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_wms_layer.png"/>'))

    assert screenshot_check("test_wms_layer", "test_wms_layer", 0.025)

    messagesCount = 0
    for i in range(0, 10):
        message = app.getStringProperty(
            f"mainWindow/messageLog/messageItem_{i}/messageText", "text"
        )
        if message == "":
            break
        extra.append(extras.html("Message logs content: {}".format(message)))
        messagesCount = messagesCount + 1
    extra.append(extras.html("Message logs count: {}".format(messagesCount)))
    assert messagesCount == 0


@pytest.mark.project_file("projection_dataset.gpkg")
def test_projection(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and check for proper reprojection support (including rendering check and message logs).
    This also tests that QField is able to reach proj's crucial proj.db
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    time.sleep(4)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_projection.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_projection.png"/>'))

    assert screenshot_check("test_projection", "test_projection", 0.062)

    messagesCount = 0
    for i in range(0, 10):
        message = app.getStringProperty(
            f"mainWindow/messageLog/messageItem_{i}/messageText", "text"
        )
        if message == "":
            break
        extra.append(extras.html("Message logs content: {}".format(message)))
        messagesCount = messagesCount + 1
    extra.append(extras.html("Message logs count: {}".format(messagesCount)))
    assert messagesCount == 0


@pytest.mark.project_file("test_image_attachment.qgz")
def test_image_attachment(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and check for proper reprojection support (including rendering check and message logs).
    This also tests that QField is able to reach proj's crucial proj.db
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    time.sleep(4)

    messagesCount = 0
    for i in range(0, 10):
        message = app.getStringProperty(
            f"mainWindow/messageLog/messageItem_{i}/messageText", "text"
        )
        if message == "":
            break
        extra.append(extras.html("Message logs content: {}".format(message)))
        messagesCount = messagesCount + 1
    extra.append(extras.html("Message logs count: {}".format(messagesCount)))
    assert messagesCount == 0

    bounds = app.getBoundingBox("mainWindow/mapCanvas")
    move_x = bounds[0] + bounds[2] / 2
    move_y = bounds[1] + bounds[3] / 3

    pyautogui.moveTo(move_x, move_y, duration=0.5)
    pyautogui.click(interval=0.5)

    bounds = app.getBoundingBox("mainWindow/featureForm")
    move_x = bounds[0] + bounds[2] / 2
    move_y = bounds[1] + 100

    pyautogui.moveTo(move_x, move_y, duration=0.5)
    pyautogui.click(interval=0.5)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_image_attachment.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_image_attachment.png"/>'))

    assert screenshot_check("test_image_attachment", "test_image_attachment", 0.025)


@pytest.mark.project_file("test_svg.qgz")
def test_svg(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and check shipped SVG marker files are present
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    time.sleep(4)

    app.takeScreenshot("mainWindow", os.path.join(screenshot_path, "test_svg.png"))
    assert process_alive()
    extra.append(extras.html('<img src="images/test_svg.png"/>'))

    assert screenshot_check("test_svg", "test_svg", 0.025)


@pytest.mark.project_file("test_offline_project.qgs")
def test_offline_project(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and check if offlined data.gpkg alongside basemap.mbtiles render properly
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    time.sleep(8)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_offline_project.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_offline_project.png"/>'))

    assert screenshot_check("test_offline_project", "test_offline_project", 0.025)


@pytest.mark.project_file("test_decorations.qgz")
def test_decorations(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and check if the title, copyright, grid, and image decorations are
    displayed in QField
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    time.sleep(8)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_decorations.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_decorations.png"/>'))

    assert screenshot_check("test_decorations", "test_decorations", 0.025)


@pytest.mark.skipif(
    platform.system() != "Linux",
    reason="PostGIS test requires a docker linux container",
)
@pytest.mark.project_file("test_postgis_ssl.qgz")
def test_postgis_ssl(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and check that a SSL-enabled postgis layer loads properly
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    time.sleep(4)
    assert process_alive()

    # Insure layer has loaded properly by checking for error messages
    messagesCount = 0
    for i in range(0, 10):
        message = app.getStringProperty(
            f"mainWindow/messageLog/messageItem_{i}/messageText", "text"
        )
        if message == "":
            break
        extra.append(extras.html("Message logs content: {}".format(message)))
        messagesCount = messagesCount + 1
    extra.append(extras.html("Message logs count: {}".format(messagesCount)))
    assert messagesCount == 0


@pytest.mark.project_file("test_3d.qgz")
def test_3d_rendering(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app with a DEM-based project and activates the 3D view to check
    that terrain rendering works properly (including rendering check and message logs).
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    time.sleep(4)

    # Activate 3D mode by directly setting the state machine state
    app.setStringProperty("mainWindow/stateMachine", "state", "3d")
    time.sleep(8)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_3d_rendering.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_3d_rendering.png"/>'))

    assert screenshot_check("test_3d_rendering", "test_3d_rendering", 0.025)

    messagesCount = 0
    for i in range(0, 10):
        message = app.getStringProperty(
            f"mainWindow/messageLog/messageItem_{i}/messageText", "text"
        )
        if message == "":
            break
        extra.append(extras.html("Message logs content: {}".format(message)))
        messagesCount = messagesCount + 1
    extra.append(extras.html("Message logs count: {}".format(messagesCount)))
    assert messagesCount == 0


@pytest.mark.project_file("test_gallery_editor.qgz")
def test_gallery_editor(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and checks the gallery relation editor when a parent/child
    relation's referencing layer has an ExternalResource field, covering grid
    view rendering, sort-order toggling, child feature form opening, and list
    view rendering.
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    app.invokeMethod("mainWindow/toursController", "blockGuides", [])
    time.sleep(8)

    assert app.getStringProperty("mainWindow/stateMachine", "state") == "digitize"
    app.setStringProperty("mainWindow/stateMachine", "state", "browse")
    time.sleep(1)

    # Insure layer has loaded properly by checking for error messages
    messagesCount = 0
    for i in range(0, 10):
        message = app.getStringProperty(
            f"mainWindow/messageLog/messageItem_{i}/messageText", "text"
        )
        if message == "":
            break
        extra.append(extras.html("Message logs content: {}".format(message)))
        messagesCount = messagesCount + 1
    extra.append(extras.html("Message logs count: {}".format(messagesCount)))
    assert messagesCount == 0

    # Click the polygon on the canvas to identify it
    bounds = app.getBoundingBox("mainWindow/mapCanvas")
    move_x = bounds[0] + bounds[2] / 2
    move_y = bounds[1] + bounds[3] / 3
    pyautogui.moveTo(move_x, move_y, duration=0.5)
    pyautogui.click(interval=0.5)
    time.sleep(2)

    # Click the identified feature row to open its form
    bounds = app.getBoundingBox("mainWindow/featureForm")
    move_x = bounds[0] + bounds[2] / 2
    move_y = bounds[1] + 100
    pyautogui.moveTo(move_x, move_y, duration=0.5)
    pyautogui.click(interval=0.5)
    time.sleep(4)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_gallery_editor_grid.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_gallery_editor_grid.png"/>'))
    assert screenshot_check("test_gallery_editor", "test_gallery_editor_grid", 0.025)

    # Click the sort button in the gallery editor header to reverse card order
    bounds = app.getBoundingBox(
        "mainWindow/featureForm/attributeEditorLoaderAttachments"
    )
    move_x = bounds[0] + bounds[2] - 34
    move_y = bounds[1] + 24
    pyautogui.moveTo(move_x, move_y, duration=0.5)
    pyautogui.click(interval=0.5)
    time.sleep(2)

    app.takeScreenshot(
        "mainWindow",
        os.path.join(screenshot_path, "test_gallery_editor_sorted.png"),
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_gallery_editor_sorted.png"/>'))
    assert screenshot_check("test_gallery_editor", "test_gallery_editor_sorted", 0.025)

    # Click sort again to restore original card order before tapping a specific card
    pyautogui.click(interval=0.5)
    time.sleep(2)

    # Tap the reserve image card to open its child feature form
    bounds = app.getBoundingBox(
        "mainWindow/featureForm/attributeEditorLoaderAttachments"
    )
    move_x = bounds[0] + bounds[2] * 3 / 4
    move_y = bounds[1] + 80
    pyautogui.moveTo(move_x, move_y, duration=0.5)
    pyautogui.click(interval=0.5)
    time.sleep(3)

    app.takeScreenshot(
        "mainWindow",
        os.path.join(screenshot_path, "test_gallery_editor_child_form.png"),
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_gallery_editor_child_form.png"/>'))
    assert screenshot_check(
        "test_gallery_editor", "test_gallery_editor_child_form", 0.025
    )

    # Close the child feature form (X close button at top right of the form)
    bounds = app.getBoundingBox("mainWindow/featureForm")
    move_x = bounds[0] + bounds[2] - 50
    move_y = bounds[1] - 18
    pyautogui.moveTo(move_x, move_y, duration=0.5)
    pyautogui.click(interval=0.5)
    time.sleep(2)

    # Scroll to bring the view switch (qfSwitch) into view
    bounds = app.getBoundingBox("mainWindow/featureForm")
    pyautogui.moveTo(bounds[0] + bounds[2] / 2, bounds[1] + bounds[3] / 2, duration=0.3)
    pyautogui.drag(0, -150, duration=0.5, button="left")
    time.sleep(1)

    # Click the view switch to toggle from grid to list
    bounds = app.getBoundingBox(
        "mainWindow/featureForm/attributeEditorLoaderAttachments"
    )
    move_x = bounds[0] + bounds[2] - 40
    move_y = bounds[1] + bounds[3] - 22
    pyautogui.moveTo(move_x, move_y, duration=0.5)
    pyautogui.click(interval=0.5)
    time.sleep(4)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_gallery_editor_list.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_gallery_editor_list.png"/>'))
    assert screenshot_check("test_gallery_editor", "test_gallery_editor_list", 0.025)


if __name__ == "__main__":
    sys.exit(pytest.main([__file__]))
