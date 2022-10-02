from py.xml import html
from pathlib import Path

import pytest
import subprocess
import xmlrpc
import time
import os


stderr = list()
stdout = list()
apperrs = list()


def pytest_html_results_summary(prefix, summary, postfix):
    prefix.extend([html.h3("stderr")])
    prefix.extend([html.pre("".join([line for line in stderr]))])
    prefix.extend([html.h3("stdout")])
    prefix.extend([html.pre("".join([line for line in stdout]))])
    prefix.extend([html.h3("App errors")])
    prefix.extend([html.pre("".join([line for line in apperrs]))])


@pytest.fixture
def process_alive(app, process, process_communicate):
    def func():
        try:
            global apperrs
            # We need to call something on the rpc server
            # the process might still be running but frozen or only almost dead
            apperrs += app.getErrors()
        except Exception as e:
            # Process probably died
            pass
        finally:
            process_communicate()
            exit_code = process.poll()
            if exit_code is not None:
                print(f"Process exited with {exit_code}")
            return exit_code is None

    yield func


@pytest.fixture
def process_communicate(process, report_summary):
    def func():
        try:
            output_b, errors_b = process.communicate(timeout=0.1)
            output = output_b.decode("utf-8")
            errors = errors_b.decode("utf-8")
            print(errors)
            print(output)
            report_summary[0].append(output)
            report_summary[1].append(errors)
        except subprocess.TimeoutExpired:
            pass

    yield func


@pytest.fixture
def report_summary():
    yield stdout, stderr


@pytest.fixture
def process(request):
    filenames = [
        "./output/bin/qfield_spix",
        "./output/bin/Release/qfield_spix.exe",
        "./output/bin/RelWithDebInfo/qfield_spix.exe",
        "./output/bin/Debug/qfield_spix.exe",
        "./output/bin/qfield.app/qfield_spix.exe",
        "./output/bin/qfield.app/Contents/MacOS/qfield_spix",
    ]

    marker = request.node.get_closest_marker("project_file")
    if marker is None:
        data = None
        projectpath = ""
    else:
        data = marker.args[0]
        projectpath = str(Path(__file__).parent.parent / "testdata" / marker.args[0])

    os.environ["QFIELD_FONT_TTF"] = str(
        Path(__file__).parent.parent / "testdata" / "FiraSansCondensed-Regular.ttf"
    )
    os.environ["QFIELD_FONT_NAME"] = "Fira Sans Condensed"
    os.environ["QFIELD_FONT_SIZE"] = "14"

    for filename in filenames:
        try:
            process = subprocess.Popen(
                [filename, projectpath], stdout=subprocess.PIPE, stderr=subprocess.PIPE
            )
            yield process
            break
        except FileNotFoundError:
            pass
    else:
        assert False, f"No qfield executable found in {filenames}"


@pytest.fixture
def app(request, process, process_communicate):
    """
    Starts a qfield process and connects an xmlrpc client to it.
    Returns the xmlrpc client that can send commands to the running process.

    Will wait up to 30 seconds for the process to start and return as soon
    as either the process is started and QML initialized, the process exits
    or the timeout occurs.

    Also makes sure it cleans up properly after running the app.
    """
    app = xmlrpc.client.ServerProxy("http://localhost:9000")
    app.process = process

    start = time.time()
    while True:
        try:
            exit_code = process.poll()
            if exit_code is not None:
                process_communicate(process)
                assert exit_code is None
            app.existsAndVisible("mainWindow")
            yield app
            break
        except (ConnectionRefusedError, OSError) as e:
            if time.time() - start > 30:
                process_communicate(process)
                assert False  # Could not start app after 30 seconds
            print(str(e))
            time.sleep(0.2)
        except Exception as e:
            process_communicate(process)
            assert False  # Unexcpected exception while starting up app

    try:
        app.quit()
        app.quit()
    except Exception:
        print("Exception while trying to exit app. The process probably died.")
        process_communicate()
        raise
    timeout = 5
    try:
        process.wait(timeout)
    except subprocess.TimeoutExpired:
        print(f"Process did not quit after {timeout} second. Killing it.")
        process.kill()
    process_communicate()
