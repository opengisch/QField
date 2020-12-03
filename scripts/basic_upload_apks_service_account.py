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

"""Uploads an apk to the chosen track."""

import argparse

from googleapiclient.discovery import build
import httplib2
from oauth2client.service_account import ServiceAccountCredentials
from oauth2client import client
import os
import json

# Declare command-line flags.
argparser = argparse.ArgumentParser(add_help=False)
argparser.add_argument('package_name',
                       help='The package name. Example: com.android.sample')
argparser.add_argument('package_track',
                       help='The track to deploy to.'
                            ' Can be "alpha", "beta", "production" or "rollout"')
argparser.add_argument('release_note',
                       help='text to add as en-US release notes')
argparser.add_argument('apk_files',
                       nargs='*',
                       help='Paths to the APK files to upload.')

google_service_account_config=json.loads(os.environ['GOOGLE_SERVICE_ACCOUNT'])

def main():
    # Create an httplib2.Http object to handle our HTTP requests and authorize it
    # with the Credentials. Note that the first parameter, service_account_name,
    # is the Email address created for the Service account. It must be the email
    # address associated with the key that was created.
    credentials = ServiceAccountCredentials.from_json_keyfile_dict(
        google_service_account_config,
        scopes='https://www.googleapis.com/auth/androidpublisher')
    http = httplib2.Http()
    http = credentials.authorize(http)

    service = build('androidpublisher', 'v3', http=http)

    # Process flags and read their values.
    flags = argparser.parse_args()

    package_name = flags.package_name
    package_track = flags.package_track
    release_note = flags.release_note
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
            track=package_track,
            packageName=package_name,
            body={'track': package_track,
                'releases': [{'versionCodes': version_codes,
                              'releaseNotes': [
                                  {'language': 'en-US', 'text': release_note}],
                              'status': 'completed'}]
                }
            ).execute()

        print('Track {track} is set for releases {releases}'.format(
            track=track_response['track'],
            releases=track_response['releases']
        ))

        commit_request = service.edits().commit(
            editId=edit_id, packageName=package_name).execute()

        print('Edit "{id}" has been committed'.format(id=commit_request['id']))

    except client.AccessTokenRefreshError:
        print('The credentials have been revoked or expired, please re-run the '
              'application to re-authorize')


if __name__ == '__main__':
    main()
