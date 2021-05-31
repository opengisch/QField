#!/usr/bin/env python

import pytest
import xmlrpc.client
import time
import subprocess
import sys
import time
import os


@pytest.fixture
def app():
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


def test_load_project(app):
    assert app.existsAndVisible("mainWindow")

    assert app.existsAndVisible("mainWindow/welcomeScreen")

    first_title = app.getStringProperty(
        "mainWindow/welcomeScreen/loadProjectItem_1", "title"
    )
    app.mouseClick("mainWindow/welcomeScreen/loadProjectItem_1")
    app.takeScreenshot(
        "mainWindow", os.path.join(os.environ["QFIELD_SCREENSHOT_PATH"], "startup.png")
    )


# @pytest.mark.parametrize('project', ['some_project.qgz'])
# def test_load_project(project):
#   app.mouseClick() ...
#   time.sleep(3)
#
#   assert first_title == 'Test'


if __name__ == "__main__":
    sys.exit(pytest.main([__file__]))
