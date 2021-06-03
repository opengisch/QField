from py.xml import html
import pytest
import subprocess
import xmlrpc
import time


stderr = list()
stdout = list()

def pytest_html_results_summary(prefix, summary, postfix):
    prefix.extend([html.h3("stderr")])
    prefix.extend([html.pre(''.join([line for line in stderr]))])
    prefix.extend([html.h3("stdout")])
    prefix.extend([html.pre(''.join([line for line in stdout]))])


@pytest.fixture
def process_alive(process, process_communicate):
    def func():
        process_communicate()
        exit_code = process.poll()
        return exit_code is None
    yield func

@pytest.fixture
def process_communicate(process, report_summary):
    def func():
        try:
            output, errors = process.communicate(timeout=0.1)
            print(errors)
            print(output)
            report_summary[0].append(output.decode('utf-8'))
            report_summary[1].append(errors.decode('utf-8'))
        except subprocess.TimeoutExpired:
            pass
    yield func

@pytest.fixture
def report_summary():
    yield stdout, stderr

@pytest.fixture
def process():
    process = subprocess.Popen(
        "./output/bin/qfield", stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    yield process

@pytest.fixture
def app(process, process_communicate):
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
        print('Exception while trying to exit app. The process probably died.')
        pass
    timeout = 5
    try:
        process.wait(timeout)
    except subprocess.TimeoutExpired:
        print(f"Process did not quit after {timeout} second. Killing it.")
        process.kill()
    process_communicate()
