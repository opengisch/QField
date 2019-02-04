Project selection
=================

QField uses QGIS projects in the ".qgs" format. Currently it is not possible to open ".qgz" files.

On choosing "Open project" from the QField menu, the Android "File selector" will be opened and the usual providers (e.g. device, SD card, Google drive, ...) will be visible on the left. At the bottom of the list, a new QFild-specific provider is added. 

Common providers
----------------
You can use common providers to choose a project on your device. QField requires the project file to be physically on the device, so it is not possible to use providers that work with remote files (e.g. Google Drive). On the other hand you can use apps like syncthing or nextcloud (or dropbox and probably many others) that allow you tho physically sync the file to your device.

QField documents provider
-------------------------
The QField documents provider, scans the device searching for ".qgs" projects and shows them for opening. This may take a few seconds. Some directories, which could be very large and probably not contain QField projects are excluded from the search.
Currently excluded are: DCIM, Pictures, Movies, Books, MyImages, Playlists, Podcasts, Sounds, Music, WhatsApp

No information about the files on the device is saved or transmitted in any way by QField.

Issues
------
QField requires to know the real path of a project in order to open it. Android and its file selectors work with URIs and hide the real path. QField tries to trace the path of the file through the URI communicated by Android, but this operation is complex because devices of different brands use different memory management strategies and the URIs are generated in different ways.

If you have any problems, please let us know.
