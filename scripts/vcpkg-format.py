#!/usr/bin/env python3

import subprocess
import sys
import os


def main():
    if sys.platform == "win32":
        cmd = ("vcpkg", "format-manifest", "--x-wait-for-lock", *sys.argv[1:])
    else:
        vcpkg_exe = os.path.join(os.path.expanduser("~"), ".vcpkg", "vcpkg")
        if not os.path.isfile(vcpkg_exe):
            print(
                f"{vcpkg_exe} not found: bootstrap it by running \n\n    . <(curl https://aka.ms/vcpkg-init.sh -L)\n\nSee https://github.com/microsoft/vcpkg-tool/blob/main/README.md#installuseremove"
            )
            return 1
        cmd = (vcpkg_exe, "format-manifest", "--x-wait-for-lock", *sys.argv[1:])
    return subprocess.call(cmd)


if __name__ == "__main__":
    exit(main())
