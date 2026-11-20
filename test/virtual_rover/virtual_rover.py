#!/usr/bin/env python3
"""Serve recorded RTCM corrections and a position computed from the returned bytes."""

import argparse
import socket
import subprocess
import tarfile
import tempfile
import threading
from pathlib import Path


def listen():
    server = socket.socket()
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("127.0.0.1", 0))
    server.listen(1)
    return server


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("fixture", type=Path)
    parser.add_argument("forward_gga", choices=("on", "off"))
    parser.add_argument("solution", choices=("fixed", "float"))
    parser.add_argument("--casters", type=int, choices=(1, 2), default=1)
    args = parser.parse_args()

    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        with tarfile.open(args.fixture, "r:gz") as archive:
            for name in ("rover.obs", "rover.nav", "0263_20090515c.rtcm3"):
                member = archive.getmember(name)
                (root / name).write_bytes(archive.extractfile(member).read())

        corrections = (root / "0263_20090515c.rtcm3").read_bytes()
        rover = listen()
        casters = [listen() for _ in range(args.casters)]
        print(
            " ".join(str(server.getsockname()[1]) for server in [rover, *casters]),
            flush=True,
        )
        position_sent = [threading.Event() for _ in casters]
        caster_data = [None] * len(casters)
        caster_errors = []

        def serve_caster(index):
            try:
                with casters[index].accept()[0] as connection:
                    request = b""
                    while b"\r\n\r\n" not in request:
                        chunk = connection.recv(4096)
                        if not chunk:
                            raise RuntimeError(
                                "Caster client disconnected during request"
                            )
                        request += chunk
                    mount_point = f"RTK{index + 1}" if args.casters == 2 else "RTK"
                    if not request.startswith(f"GET /{mount_point} ".encode()):
                        raise RuntimeError("Unexpected NTRIP mount point")
                    connection.sendall(
                        b"HTTP/1.1 200 OK\r\nContent-Type: gnss/data\r\n\r\n"
                    )
                    connection.sendall(corrections)
                    if not position_sent[index].wait(30):
                        raise RuntimeError("No position was sent to the virtual rover")
                    connection.settimeout(2 if args.forward_gga == "on" else 1)
                    try:
                        data = connection.recv(4096)
                        if not data:
                            raise RuntimeError(
                                "Caster client disconnected before GGA check"
                            )
                        caster_data[index] = data
                    except socket.timeout:
                        caster_data[index] = b""
            except Exception as error:
                caster_errors.append(error)

        threads = [
            threading.Thread(target=serve_caster, args=(index,), daemon=True)
            for index in range(len(casters))
        ]
        for thread in threads:
            thread.start()

        with rover.accept()[0] as connection:
            connection.settimeout(30)
            for index in range(len(casters)):
                data = b""
                while len(data) < len(corrections):
                    chunk = connection.recv(65536)
                    if not chunk:
                        raise RuntimeError(
                            "Rover disconnected before receiving corrections"
                        )
                    data += chunk
                    if data.startswith(b"?WATCH="):
                        end = data.find(b";")
                        if end >= 0:
                            data = data[end + 1 :]

                if data != corrections:
                    raise RuntimeError(
                        "Corrections returned by QField differ from caster data"
                    )
                if index == 0:
                    (root / "received.rtcm3").write_bytes(data)
                    subprocess.run(
                        [
                            "convbin",
                            "-r",
                            "rtcm3",
                            "-tr",
                            "2009/05/15",
                            "05:16:00",
                            "-o",
                            "base.obs",
                            "received.rtcm3",
                        ],
                        cwd=root,
                        stdout=subprocess.DEVNULL,
                        stderr=subprocess.DEVNULL,
                        check=True,
                    )
                    ambiguity = ["-v", "0.0"] if args.solution == "float" else []
                    result = subprocess.run(
                        [
                            "rnx2rtkp",
                            "-n",
                            *ambiguity,
                            "-o",
                            "solution.nmea",
                            "rover.obs",
                            "base.obs",
                            "rover.nav",
                        ],
                        cwd=root,
                        stdout=subprocess.DEVNULL,
                        stderr=subprocess.PIPE,
                        check=True,
                    )
                    if not (root / "solution.nmea").exists():
                        raise RuntimeError(
                            f"RTKLIB produced no NMEA: {result.stderr[-1000:]!r}"
                        )
                    quality = b"5" if args.solution == "float" else b"4"
                    gga = [
                        line
                        for line in (root / "solution.nmea").read_bytes().splitlines()
                        if b"GGA" in line and line.split(b",")[6] == quality
                    ]
                    if len(gga) < 2:
                        raise RuntimeError(
                            f"RTKLIB did not compute an RTK {args.solution} solution"
                        )
                for sentence in gga[:3]:
                    connection.sendall(sentence + b"\r\n")
                    threading.Event().wait(0.2)
                position_sent[index].set()
                threads[index].join(5)
                if threads[index].is_alive():
                    raise RuntimeError(
                        "Caster did not finish checking position forwarding"
                    )
                if caster_errors:
                    raise caster_errors[0]

                payload = b"GP" + gga[0][3:].split(b"*")[0]
                checksum = 0
                for byte in payload:
                    checksum ^= byte
                expected = b"$" + payload + b"*" + f"{checksum:02X}".encode() + b"\r\n"
                if args.forward_gga == "on" and caster_data[index] != expected:
                    raise RuntimeError(
                        f"Expected forwarded GGA {expected!r}, got {caster_data[index]!r}"
                    )
                if args.forward_gga == "off" and caster_data[index] != b"":
                    raise RuntimeError(
                        f"Caster received position with forwarding disabled: {caster_data[index]!r}"
                    )
                print(f"Caster {index + 1} complete", flush=True)


if __name__ == "__main__":
    main()
