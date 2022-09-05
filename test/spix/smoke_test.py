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

    app.setStringProperty("mainWindow", "width", "500")
    app.setStringProperty("mainWindow", "height", "500")

    time.sleep(2)

    app.takeScreenshot("mainWindow", os.path.join(screenshot_path, "startup.png"))
    assert process_alive()
    extra.append(extras.html('<img src="images/startup.png"/>'))

    messagesCount = 0
    for i in range(0, 10):
        message = app.getStringProperty(
            "mainWindow/messageLog/messageItem_{}/messageText".format(i), "text"
        )
        if message == "":
            break
        extra.append(extras.html("Message logs content: {}".format(message)))
        messagesCount = messagesCount + 1
    assert messagesCount == 0


# @pytest.mark.parametrize('project', ['some_project.qgz'])
# def test_load_project(project):
#   app.mouseClick() ...
#   time.sleep(3)
#
#   assert first_title == 'Test'


if __name__ == "__main__":
    sys.exit(pytest.main([__file__]))
