#!/usr/bin/env python
#
# Copyright 2014 Marta Rodriguez.
#
# Licensed under the Apache License, Version 2.0 (the 'License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Uploads an apk to the alpha track."""

import sys
# required on OSX to prevent the default 'six' library from interfering with the needed newer package,
# as of the 1.4.1 google-api library
sys.path.insert(1, '/Library/Python/2.7/site-packages')

import argparse
import httplib2
import json

from oauth2client.service_account import ServiceAccountCredentials
from oauth2client.client import AccessTokenRefreshError
from apiclient.discovery import build


def upload(package, service, apk, track):

    # Create an httplib2.Http object to handle our HTTP requests and authorize it
    # with the Credentials. Note that the first parameter, service_account_name,
    # is the Email address created for the Service account. It must be the email
    # address associated with the key that was created.
    credentials = ServiceAccountCredentials.from_json_keyfile_name(
        service, ['https://www.googleapis.com/auth/androidpublisher'])

    http_auth = credentials.authorize(http=httplib2.Http())
    service = build('androidpublisher', 'v2', http=http_auth)

    try:
        edit_request = service.edits().insert(body={}, packageName=package)
        result = edit_request.execute()
        edit_id = result['id']

        apk_response = service.edits().apks().upload(
            editId=edit_id,
            packageName=package,
            media_body=apk).execute()

        print('Version code {} has been uploaded'.format(apk_response['versionCode']))

        track_response = service.edits().tracks().update(
            editId=edit_id,
            track=track,
            packageName=package,
            body={u'versionCodes': [apk_response['versionCode']]}).execute()

        print('Track {} is set for version code(s) {}'.format(
            track_response['track'], track_response['versionCodes']))

        commit_request = service.edits().commit(
            editId=edit_id, packageName=package).execute()

        print('Edit "{}" has been committed'.format(commit_request['id']))

    except AccessTokenRefreshError as e:
        print('The credentials have been revoked or expired, please re-run the '
              'application to re-authorize')
        raise e


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--package', required=True,
                        help='The package name. Example: com.android.sample')
    parser.add_argument('-s', '--service', required=True,
                        help='The service account json file.')
    parser.add_argument('-a', '--apk', required=True,
                        help='The path to the APK file to upload.')
    parser.add_argument(
        '-t', '--track', choices=['alpha', 'beta', 'production', 'rollout'], default='alpha')
    args = parser.parse_args()

    upload(args.package, args.service, args.apk, args.track)


if __name__ == "__main__":
    main()
