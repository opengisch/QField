#!/usr/bin/env python3

from github import Github, GithubException
import argparse
import os

GH_TOKEN_ENV_VAR_NAME = 'GH_TOKEN'
GH_REPO_SLUG = os.environ['TRAVIS_REPO_SLUG']


class UploadReleaseAssetError(Exception):
    pass


def upload_asset_to_github_release(
        asset_path: str,
        release_tag: str,
        github_token: str,
        asset_name: str = None
):
    """
    Upload an asset to a release
    :param asset_path: the file path
    :param release_tag: the release tag as identified on Github
    :param github_token: a token to allow editing the release
    :param asset_name: an optional name to give to the asset
    """
    repo = Github(github_token).get_repo(GH_REPO_SLUG)
    try:
        print('Getting release on {}'.format(GH_REPO_SLUG))
        gh_release = repo.get_release(id=release_tag)
        print(gh_release, gh_release.tag_name, gh_release.upload_url)
    except GithubException as e:
        raise UploadReleaseAssetError('Release {} not found'.format(release_tag))
    try:
        assert os.path.exists(asset_path)
        if asset_name:
            print('Uploading asset: {} as {}'.format(asset_path, asset_name))
            gh_release.upload_asset(path=asset_path, label=asset_name, name=asset_name)
        else:
            print('Uploading asset: {}'.format(asset_path))
            gh_release.upload_asset(asset_path)
        print('OK')
    except GithubException as e:
        print(e)
        raise UploadReleaseAssetError('Could not upload asset for release {}.'.format(release_tag))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('release_asset', help='The file to be uploaded')
    parser.add_argument('release_tag', help='The version tag')
    parser.add_argument('--github-token', default=os.environ.get(GH_TOKEN_ENV_VAR_NAME),
                        help='The Github API token. If not specified, it will'
                             ' be taken from ${}.'.format(GH_TOKEN_ENV_VAR_NAME))
    parser.add_argument('--asset-name', default=None,
                        help='An optional name for the release asset')
    args = parser.parse_args()

    upload_asset_to_github_release(args.release_asset, args.release_tag, args.github_token, args.asset_name)

    return 0


if __name__ == "__main__":
    exit(main())
