#!/usr/bin/env python

import pytest
from pytest_html import extras
import xmlrpc.client
import time
import subprocess
import sys
import time
import os


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


def test_start_app(app, screenshot_path, extra, process_alive):
    """
    Starts a test app to the welcome screen and creates a screenshot.
    """
    assert app.existsAndVisible("mainWindow")

    assert app.existsAndVisible("mainWindow/welcomeScreen")

    first_title = app.getStringProperty(
        "mainWindow/welcomeScreen/loadProjectItem_1", "title"
    )
    app.mouseClick("mainWindow/welcomeScreen/loadProjectItem_1")
    app.takeScreenshot("mainWindow", os.path.join(screenshot_path, "startup.png"))
    assert process_alive()
    extra.append(extras.html('<img src="images/startup.png"/>'))


# @pytest.mark.parametrize('project', ['some_project.qgz'])
# def test_load_project(project):
#   app.mouseClick() ...
#   time.sleep(3)
#
#   assert first_title == 'Test'


if __name__ == "__main__":
    sys.exit(pytest.main([__file__]))
