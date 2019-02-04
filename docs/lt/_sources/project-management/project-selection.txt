Project selection
=================

QField uses QGIS projects in the ".qgs" format. At the moment it is not possible to open ".qgz" files.

If you choose "Open project" from the QField menu, you will see the Android "File selector" with its providers (e.g. device, SD card, Google drive, ...) to which a specific provider for QField projects is added. 

Common providers
----------------
You can use common providers to choose a project on your device. QField requires the project file to be physically on the device, so it is not possible to use providers that work with remote files (e.g. Google Drive).

QField documents provider
-------------------------
The QField documents provider, takes care of scanning the memory of the device to search for ".qgs" projects and propose them for opening. This may take a few seconds. Some directories, which can potentially be very large and probably do not contain QField projects (e.g. DCIM, Pictures, Movies, Music, ...), are excluded from the search.

No information about the files on the device is saved or transmitted in any way by QField.

Issues
------
QField requires to know the real path of a project in order to open it. Android and its file selectors work with URIs and hide the real path. QField tries to trace the path of the file through the URI communicated by Android, but this operation is complex because of the fact that devices of different brands sometimes involve a different management of the memory of the device and the URIs generated in different ways.

If you have any problems, please let us know.
