#!/usr/bin/env python

import pytest
import xmlrpc.client
import time
import subprocess
import sys
import time
import os


# TODO: use setup and teardown for this infrastructure work
def get_test_app():
    process = subprocess.Popen('./output/bin/qfield', stdout=subprocess.PIPE)

    s = xmlrpc.client.ServerProxy('http://localhost:9000')
    
    while True:
        try:
            s.existsAndVisible('mainWindow')
            return process, s
        except ConnectionRefusedError:
            time.sleep(0.2)

    # TODO print output of process
    assert False # Test app could not be started

def test_load_project():
    process, app = get_test_app()
    assert app.existsAndVisible('mainWindow')

    assert app.existsAndVisible('mainWindow/welcomeScreen')
    first_title = app.getStringProperty('mainWindow/welcomeScreen/loadProjectItem_1', 'title')
    app.mouseClick('mainWindow/welcomeScreen/loadProjectItem_1')
    time.sleep(3)

    assert first_title == 'Test'

    process.terminate()
    process.terminate()


if __name__ == "__main__":
    sys.exit(pytest.main([__file__]))
