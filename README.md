[![Read the Docs](https://img.shields.io/badge/Read-the%20Docs-green.svg)](https://docs.qfield.org/)
[![Community Platform](https://img.shields.io/discourse/topics?server=https://community.qfield.org)](https://community.qfield.org)
[![Sponsor](https://img.shields.io/static/v1?label=Support&message=%E2%9D%A4)](https://github.com/sponsors/opengisch)
[![Contribute](https://img.shields.io/static/v1?label=Contribute&message=💪)](#contribute)
[![Release](https://img.shields.io/github/release/opengisch/QField.svg?label=Release)](https://github.com/opengisch/QField/releases)
[![OpenSSF Best Practices](https://www.bestpractices.dev/projects/8392/badge)](https://www.bestpractices.dev/projects/8392)
[![Digital Public Good](https://img.shields.io/badge/Digital%20Public%20Good-verified-brightgreen)](https://www.digitalpublicgoods.net/r/qfield)
[![QFieldCloud](https://img.shields.io/badge/QFieldCloud-GitHub-blue)](https://github.com/opengisch/QFieldCloud)

# QField for QGIS

A simplified touch-optimized interface for QGIS in the field.

[![Visit QField's homepage](https://github.com/user-attachments/assets/88771ae0-3701-4cf4-8d8c-cd295c0831b1)](https://qfield.org)

## 🧭 About QField

QField works fully offline or connected, and supports seamless synchronization with the optional [**QFieldCloud** platform](https://qfield.cloud) for collaborative field-to-office workflows.
You can find the open-source QFieldCloud backend on GitHub here: [github.com/opengisch/QFieldCloud](https://github.com/opengisch/QFieldCloud)

QField is officially recognized as a [Digital Public Good](https://digitalpublicgoods.net/r/qfield) for its contributions to open, inclusive, and sustainable digital development.

Explore the full documentation at [docs.qfield.org](https://docs.qfield.org/)

## 📲 Get QField
<p align="center">
  <a href="https://play.google.com/store/apps/details?id=ch.opengis.qfield"><img src="https://qfield.org/images/play_store.png" alt="Get it on Google Play" height="60"/></a>
  <a href="https://apps.microsoft.com/detail/xp99h3bcx4bw7f"><img src="https://qfield.org/images/download_windows.png" alt="Get it on Microsoft Store" height="60"/></a>
  <a href="https://apps.apple.com/app/qfield-for-qgis/id1531726814"><img src="https://qfield.org/images/app_store.png" alt="Get it on the App Store" height="60"/></a>
</p>
<p align="center">
  <a href="https://qfield.org/get_latest?platform=linux"><img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/linux/linux-original.svg" alt="Linux" width="20"/>Download for Linux</a>
  <a href="https://qfield.org/get_latest?platform=macos"><img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/apple/apple-original.svg" alt="macOS" width="20"/>Download for macOS</a>
</p>

### All Platforms
📦 Prefer direct downloads or older versions?  Check out the full list of releases on [GitHub Releases](https://github.com/opengisch/QField/releases)

### Get master (unstable) version

We automatically publish the latest master build to a [dedicated channel on the playstore](https://play.google.com/store/apps/details?id=ch.opengis.qfield_dev). You'll need to [join the beta program](https://play.google.com/apps/testing/ch.opengis.qfield_dev) to start getting the latest version.

Please remember that this is the latest development build and is not meant for production.

## Contribute

QField is an open source project, licensed under the terms of the GPLv2 or later. This means that it is free to use and modify and will stay like that.

We are very happy if this app helps you to get your job done or in whatever creative way you may use it.

If you found it useful, we will be even happier if you could give something back. A couple of things you can do are

 * Rate the app [★★★★★](https://play.google.com/store/apps/details?id=ch.opengis.qfield&hl=en#details-reviews)
 * [Share about it on Mastodon](https://mastodon.social/share?text=Looking%20for%20a%20good%20tool%20for%20field%20work%20in%20GIS?%20Check%20out%20%23QField!)
 * [Share on Bluesky](https://bsky.app/profile/qfield.bsky.social/share?text=Looking%20for%20a%20good%20tool%20for%20field%20work%20in%20GIS?%20Check%20out%20%23QField!)
 * Write about your experience (please [let us know](mailto:sales@qfield.cloud)!)
 * [Help with the documentation](https://github.com/opengisch/QField-docs#documentation-process)
 * [Translate the documentation](https://github.com/opengisch/QField-docs#translation-process) or [the app](https://www.transifex.com/opengisch/qfield-for-qgis/)
 * [Sponsor a feature](https://docs.qfield.org/get-started/sponsor/)
 * And just drop by to say thank you or have a beer with us next time you meet OPENGIS.ch at a conference


## Development

For development information, refer to the dedicated [developer documentation](doc/dev.md).

## Verify Authenticity of the App packages (Android only)

SHA-256 hash of signing certificate:

```5a7dd946a4b700c081a5bd375dbc8f0d11aa89d53832567ce5b8a92088e0e898```

Use the following command to verify the hash of the signing certificate:

```apksigner verify --print-certs [filename.apk] | grep "5a7dd946a4b700c081a5bd375dbc8f0d11aa89d53832567ce5b8a92088e0e898"```
