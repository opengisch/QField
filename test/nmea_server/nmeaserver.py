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
            via a tcp or udp server
        """
parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter, description=desc
)

parser.add_argument("logfile", help="Path to the NMEA log file", type=pathlib.Path)
parser.add_argument(
    "-t",
    "--type",
    help="Server type to operate on",
    choices=["tcp", "udp"],
    default="tcp",
    type=str,
)
parser.add_argument("-p", "--port", help="Port to operate on", default=1958, type=int)
parser.add_argument(
    "-i",
    "--interval",
    help="Pause between sending sentences in seconds",
    default=0.05,
    type=float,
)
args = vars(parser.parse_args())


host = ""  # localhost
port = args["port"]

log = os.path.join(args["logfile"])
with open(log, "rb") as f:
    lines = f.readlines()


def work_thread(client):
    i = 0
    while True:  # continuously read and handle data
        i = (i + 1) % len(lines)
        data = lines[i]
        try:
            client.sendall(data)
            print(data)
        except:
            break
        time.sleep(args["interval"])


if args["type"] == "tcp":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind((host, port))
        sock.listen()
        print(f"Listening on port {port}")
        while True:
            (client, addr) = sock.accept()
            print(f"Accepting new connection with address {addr}")
            t = threading.Thread(target=work_thread, args=(client,))
            t.start()
else:
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP) as sock:
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        sock.settimeout(0.2)
        i = 0
        while True:
            i = (i + 1) % len(lines)
            data = lines[i]
            sock.sendto(data, ("<broadcast>", port))
            print(data)
            time.sleep(args["interval"])
