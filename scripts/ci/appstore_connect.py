#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import requests
import jwt
import argparse
import time
import json
import sys

parser = argparse.ArgumentParser(description="Interact with appstoreconnect")
parser.add_argument("--key_id", type=str, help="The key id")
parser.add_argument("--issuer_id", type=str, help="The issuer id")
parser.add_argument("--private_key", type=str, help="The private key")

subparsers = parser.add_subparsers(dest="command")
add_to_beta_group_parser = subparsers.add_parser("add_to_beta_group")
add_to_beta_group_parser.add_argument("group_id", type=str, nargs="?", help="Group ID")
add_to_beta_group_parser.add_argument("build_id", type=str, nargs="?", help="Build ID")
build_id_by_version_parser = subparsers.add_parser("build_id_by_version")
build_id_by_version_parser.add_argument("version", type=int, nargs="?", help="Version")
publish_on_app_store_parser = subparsers.add_parser("publish_on_app_store")
publish_on_app_store_parser.add_argument("version", type=int, nargs="?", help="Version")
publish_on_app_store_parser.add_argument("appid", type=str, nargs="?", help="Version")

args = parser.parse_args()

# 20 minutes timestamp
expiration_time = round(time.time() + (20 * 60))

# prepare jwt token
token = jwt.encode(
    {"iss": args.issuer_id, "exp": expiration_time, "aud": "appstoreconnect-v1"},
    args.private_key,
    headers={"alg": "ES256", "kid": args.key_id, "typ": "JWT"},
)

headers = {"Authorization": f"Bearer {token}", "Content-Type": "application/json"}


def add_to_beta_group(build_id, group_id):
    url = f"https://api.appstoreconnect.apple.com/v1/betaGroups/{group_id}/relationships/builds"

    post_data = {"data": [{"id": build_id, "type": "builds"}]}
    r = requests.post(url, data=json.dumps(post_data), headers=headers)
    print(r.status_code)

    if r.status_code == 204:
        print("add_to_beta_group: success")
    else:
        print("add_to_beta_group: failed")
        print(json.dumps(r.json(), indent=4))
        exit(-1)


def build_id_by_version(version):
    url = f"https://api.appstoreconnect.apple.com/v1/builds?filter[version]={version}"

    r = requests.get(url, headers=headers)
    if r.status_code == 200:
        print(r.json()["data"][0]["id"])
    else:
        raise RuntimeError(
            f"Error getting build. Status code {r.status_code} \n\n{json.dumps(r.json(), indent=4)}"
        )


def publish_on_app_store(version, appid):
    url = f"https://api.appstoreconnect.apple.com/v1/appStoreVersions"

    post_data = {
        "data": {
            "attributes": {"platform": "IOS", "versionString": version},
            "relationships": {"app": {"data": {"id": appid, "type": "apps"}}},
            "type": "appStoreVersions",
        }
    }
    r = requests.post(url, data=json.dumps(post_data), headers=headers)
    if r.status_code == 200:
        print(r.json()["data"][0]["id"])
    else:
        raise RuntimeError(
            f"Error getting build. Status code {r.status_code} \n\n{json.dumps(r.json(), indent=4)}"
        )


if args.command == "add_to_beta_group":
    add_to_beta_group(args.build_id, args.group_id)
elif args.command == "build_id_by_version":
    build_id_by_version(args.version)
else:
    parser.print_usage()
