#################
QField User Guide
#################

**************
Basic workflow
**************

Switch modes
============

Modes are switched via :menuselection:`Menu button --> mode`

Opening a project
=================


Identify features
=================

A tap on a feature will identify it. Pressing the back button will close the identify window.

Exceptions to identified layers
-------------------------------

Often it is not required to be able to query every layer. Some layers are only present as basemap and their attributes are not of interest.

You can manage this layerlist in QGIS desktop in :menuselection:`Project --> Project Properties --> Identify Layers` and uncheck the base layers.

GPS
===

A short press on the GPS button will turn on the GPS and center to the current location once positioning information is available.

A long press on the GPS button will show the positioning menu.

Inside the positioning menu you can turn on the positioning display which will show the current coordinates which are reprojected into the project CRS along with precision information.

.. note::
    If you see WGS 84 lat/lon information instead of information in your project CRS, you probably have no signal yet.
    
Using external GNSS-Reciever
----------------------------

It is possible to provide a mock location a separate android app to qfield.
There are several options for this, one of them is `Android NTRIP Client 
<https://play.google.com/store/apps/details?id=com.lefebure.ntripclient>`_.

To use this you have to `enable mock locations on your Android device 
<https://www.youtube.com/watch?v=v1eRHmMiRJQ>`_.


Digitize
========

To start digitizing new features `Switch modes`_ to digitizing.

A new combobox will appear next to the menu button which lists the layers
available for digitizing.

Points
------

Navigate the crosshair in the center of the screen to the desired location and
click the pencil at the lower right of the screen to confirm the creation of a
new point feature.

Lines and polygons
------------------

Navigate the crosshair in the center of the screen to the desired start of the line
and click the pencil at the lower right of the screen.
Proceed with adding points or removing points until the line or polygon is finished
and then click save.

Attribute form
--------------

After digitizing a geometry, the attribute form will be displayed if it is not suppressed
and the user will be asked to enter the attributes for the new feature.
The form which appears allows entering attribute values for the new feature. The checkboxes
at the right of every attribute allow for remembering each attribute individually.

Delete Features
===============

Deleting features is only possible in digitize mode.

To delete a feature, identify it first and tap the trash icon.
