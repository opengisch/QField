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
    print(f'Images will be written to {img_path}')
    os.makedirs(img_path, exist_ok=True)
    print("screenshot path exists" if os.path.exists(img_path) else "screenshot path missing")
    return img_path


@pytest.fixture
def app():
    """
    Starts a qfield process and connects an xmlrpc client to it.
    Returns the xmlrpc client that can send commands to the running process.

    Will wait up to 30 seconds for the process to start and return as soon
    as either the process is started and QML initialized, the process exits
    or the timeout occurs.

    Also makes sure it cleans up properly after running the app.
    """
    process = subprocess.Popen(
        "./output/bin/qfield", stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    app = xmlrpc.client.ServerProxy("http://localhost:9000")

    start = time.time()
    while True:
        try:
            exit_code = process.poll()
            if exit_code is not None:
                output, errors = process.communicate()
                print("== OUTPUT ==")
                print(output.decode("utf-8"))
                print("== ERRORS ==")
                print(errors.decode("utf-8"))

                assert exit_code is None
            app.existsAndVisible("mainWindow")
            yield app
            break
        except (ConnectionRefusedError, OSError) as e:
            if time.time() - start > 30:
                output, errors = process.communicate()
                print(output)
                print(errors)
                assert False  # Could not start app after 30 seconds
            print(str(e))
            time.sleep(0.2)
        except Exception as e:
            output, errors = process.communicate()
            assert False  # Unexcpected exception while starting up app

    app.quit()
    app.quit()
    try:
        process.wait(1)
    except subprocess.TimeoutExpired:
        print("Process did not quit after 1 second. Killing it.")
        process.kill()


def test_start_app(app, screenshot_path, extra):
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
    app.takeScreenshot("mainWindow", os.path.join('/tmp', "startup.png"))
    time.sleep(5) # Debug missing images
    print()
    print("screenshot exists" if os.path.exists(os.path.join(screenshot_path, "startup.png")) else "screenshot missing")
    print("tmp screenshot exists" if os.path.exists(os.path.join('/tmp', "startup.png")) else "screenshot missing")
    extra.append(extras.html('<img src="images/startup.png"/>'))


# @pytest.mark.parametrize('project', ['some_project.qgz'])
# def test_load_project(project):
#   app.mouseClick() ...
#   time.sleep(3)
#
#   assert first_title == 'Test'


if __name__ == "__main__":
    sys.exit(pytest.main([__file__]))
