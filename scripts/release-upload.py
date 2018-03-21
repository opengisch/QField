#!/usr/bin/env python3
# coding=utf-8

"""This script creates a new release on github releases
        Author: Matthias Kuhn (matthias@opengis.ch)
        git sha              : $TemplateVCSFormat
"""
from __future__ import print_function
from future import standard_library

import sys
import json
import http.client
import os
from optparse import OptionParser


def main(parameters, arguments):
    """Main entry point.

    :param parameters: Command line parameters.
    :param arguments: Command line arguments.
    """

    conn = http.client.HTTPSConnection('api.github.com')
    headers = {
        'User-Agent': 'Deploy-Script',
        'Authorization': 'token {}'.format(parameters.oauth_token)
    }

    raw_data = {
        "tag_name": parameters.release
    }
    if parameters.changelog:
        with open(parameters.changelog, 'r') as cl:
            raw_data['body'] = cl.read()
    data = json.dumps(raw_data)
    conn.request('POST', '/repos/{repo_slug}/releases'.format(
        repo_slug=os.environ['TRAVIS_REPO_SLUG']), body=data, headers=headers)
    response = conn.getresponse()
    release = json.loads(response.read().decode())
    print(release)

    conn = http.client.HTTPSConnection('uploads.github.com')
    headers['Content-Type'] = 'application/octet-stream'
    for filename in arguments:
        url = '{}?name={}'.format(release['upload_url'][:-13], filename)
        print('Upload to {}'.format(url))

        with open(filename, 'rb') as f:
            conn.request('POST', url, f, headers)

        print(conn.getresponse().read())


if __name__ == "__main__":
    parser = OptionParser(usage="%prog [options] file [file file]")
    parser.add_option(
        "-r", "--release", dest="release",
        help="Specify the release name", metavar="v1.0.0")
    parser.add_option(
        "-a", "--oauth-token", dest="oauth_token",
        help="Specify the github oauth_token to use")
    parser.add_option(
        "-c", "--changelog", dest="changelog",
        help="Specify the changelog file", metavar="/tmp/changelog")
    options, args = parser.parse_args()
    if len(args) != 1:
        print("Please specify at least a release file.\n")
        parser.print_help()
        sys.exit(1)

    main(options, args)
