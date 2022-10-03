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
    def inner(test_name, image_name):
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
            threshold=0.025,
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

    app.takeScreenshot("mainWindow", os.path.join(screenshot_path, "startup.png"))
    assert process_alive()
    extra.append(extras.html('<img src="images/startup.png"/>'))


@pytest.mark.project_file("test_wms.qgz")
def test_wms_layer(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and check for WMS layer support (including rendering check and message logs).
    This also tests that QField is able to reach QGIS's crucial srs.db.
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    time.sleep(4)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_wms_layer.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_wms_layer.png"/>'))

    assert screenshot_check("test_wms_layer", "test_wms_layer")

    messagesCount = 0
    for i in range(0, 10):
        message = app.getStringProperty(
            "mainWindow/messageLog/messageItem_{}/messageText".format(i), "text"
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
    time.sleep(4)

    app.takeScreenshot(
        "mainWindow", os.path.join(screenshot_path, "test_projection.png")
    )
    assert process_alive()
    extra.append(extras.html('<img src="images/test_projection.png"/>'))

    assert screenshot_check("test_projection", "test_projection")

    messagesCount = 0
    for i in range(0, 10):
        message = app.getStringProperty(
            "mainWindow/messageLog/messageItem_{}/messageText".format(i), "text"
        )
        if message == "":
            break
        extra.append(extras.html("Message logs content: {}".format(message)))
        messagesCount = messagesCount + 1
    extra.append(extras.html("Message logs count: {}".format(messagesCount)))
    assert messagesCount == 0


@pytest.mark.project_file("test_svg.qgz")
def test_svg(app, screenshot_path, screenshot_check, extra, process_alive):
    """
    Starts a test app and check shipped SVG marker files are present
    """
    assert app.existsAndVisible("mainWindow")

    # Arbitrary wait period to insure project fully loaded and rendered
    time.sleep(4)

    app.takeScreenshot("mainWindow", os.path.join(screenshot_path, "test_svg.png"))
    assert process_alive()
    extra.append(extras.html('<img src="images/test_svg.png"/>'))

    assert screenshot_check("test_svg", "test_svg")


if __name__ == "__main__":
    sys.exit(pytest.main([__file__]))
