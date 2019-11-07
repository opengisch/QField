#!/usr/bin/env python3
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

import argparse

from googleapiclient.discovery import build
import httplib2
from oauth2client.service_account import ServiceAccountCredentials
from oauth2client import client

TRACK = 'alpha'  # Can be 'alpha', beta', 'production' or 'rollout'
SERVICE_ACCOUNT_EMAIL = (
    'qfielddeployer@api-5606400287044015100-167791.iam.gserviceaccount.com')

# Declare command-line flags.
argparser = argparse.ArgumentParser(add_help=False)
argparser.add_argument('package_name',
                       help='The package name. Example: com.android.sample')
argparser.add_argument('apk_files',
                       nargs='*',
                       help='Paths to the APK files to upload.')


def main():
    # Load the key in PKCS 12 format that you downloaded from the Google APIs
    # Console when you created your Service account.
    with open('.ci/play_developer.p12', 'rb') as f:
        key = f.read()

    # Create an httplib2.Http object to handle our HTTP requests and authorize it
    # with the Credentials. Note that the first parameter, service_account_name,
    # is the Email address created for the Service account. It must be the email
    # address associated with the key that was created.
    credentials = ServiceAccountCredentials.from_p12_keyfile(
        SERVICE_ACCOUNT_EMAIL,
        '.ci/play_developer.p12',
        scopes='https://www.googleapis.com/auth/androidpublisher')
    http = httplib2.Http()
    http = credentials.authorize(http)

    service = build('androidpublisher', 'v3', http=http)

    # Process flags and read their values.
    flags = argparser.parse_args()

    package_name = flags.package_name
    apk_files = flags.apk_files

    try:
        edit_request = service.edits().insert(body={}, packageName=package_name)
        result = edit_request.execute()
        edit_id = result['id']

        version_codes = list()
        for filepath in apk_files:
            apk_response = service.edits().apks().upload(
                editId=edit_id,
                packageName=package_name,
                media_body=filepath).execute()

            print('Version code {version_code} has been uploaded'.format(
                version_code=apk_response['versionCode']))
            version_codes.append(apk_response['versionCode'])

        track_response = service.edits().tracks().update(
            editId=edit_id,
            track=TRACK,
            packageName=package_name,
            body={'track': TRACK,
                'releases': [ {'versionCodes': version_codes, 'status' : 'completed' } ] }).execute()

        print('Track {track} is set for version code(s) {version_code}'.format(
            track=track_response['track'],
            version_code=track_response['versionCodes']
        ))

        commit_request = service.edits().commit(
            editId=edit_id, packageName=package_name).execute()

        print('Edit "{id}" has been committed'.format(id=commit_request['id']))

    except client.AccessTokenRefreshError:
        print('The credentials have been revoked or expired, please re-run the '
              'application to re-authorize')


if __name__ == '__main__':
    main()
