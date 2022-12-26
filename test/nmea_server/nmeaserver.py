#!/usr/bin/env python3

import socket
import threading
import time
import pathlib
import os
import argparse


desc = """\
        nmeaserver.py
        --------------------------------
            This application loads a nmea log file and streams it
            via a tcp server
        """
parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter, description=desc
)

parser.add_argument("logfile", help="Path to the NMEA log file", type=pathlib.Path)
parser.add_argument("-p", "--port", help="Port to operate on", default=1958, type=int)
parser.add_argument(
    "-i", "--interval", help="Pause between sending sentences", default=0.1, type=float
)
args = vars(parser.parse_args())


host = ""  # localhost
port = args["port"]
PACKET_SIZE = 1024

log = os.path.join(args["logfile"])
with open(log, "rb") as f:
    lines = f.readlines()


def work_thread(client):
    i = 0
    while True:  # continuously read and handle data
        i = (i + 1) % len(lines)
        data = lines[i]
        client.sendall(data)
        time.sleep(args["interval"])


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.bind((host, port))
    sock.listen(5)  # we should never have more than one client
    print(f"Listening on port {port}")

    client, addr = sock.accept()  # your script will block here waiting for a connection
    t = threading.Thread(target=work_thread, args=(client,))
    t.start()
