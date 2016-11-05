#######################
Development and support
#######################

*******
License
*******

QField is released under the GNU Public License (GPL) Version 2 or above.
Developing QField under this license means that you can inspect and modify
the source code and guarantees that you will always have access to a QGIS 
based field data collection app that is free of cost and can be freely 
modified.

You can find the sourcecode `on github <https://github.com/opengisch/QField>`_

*******
Support
*******

You can ask your questions on `gis.stackexchange <http://gis.stackexchange.com/questions/tagged/qfield?sort=newest>`_

You can use the `user mailing list <http://lists.qfield.org/listinfo.cgi/users-qfield.org>`_

We also provide commercial support, `contact us <http://www.opengis.ch/contact/>`_

.. _make_it_grow:

Make it grow, make it yours
===========================

This is a list of features which we think are very useful for a nice user
experience and an effective workflow.

Edit widgets
............

Edit widgets are a core part of efficient field work. Entering attributes
on a touch device is one of the jobs that takes the most time and
keeping this task as easy as possible is key for pleasuring work.

Some important :ref:`edit field widgets are already
available<edit_field_widgets>` but much more is possible `for example in QGIS
<http://gis.stackexchange.com/a/30051/9839>`_.

Legend
......

To properly interpret the contents of a map, a legend is a very useful tool.
It gives a user the required context to understand the semantic meaning of
colors, shapes, strokes and other visual coding.

It can also be used to access additional meta data of individual layers or
chose which layer to work on.

Connection based on-/offline synchronization
............................................

Data collection is one of the domains where QField can help a lot. The final
destination of data is normally not on the device itself but somewhere on a
database. Currently it is possible to directly work on the database but since
network access is often unreliable when in the field, it boils down to manual
synchronization on a desktop computer.

It would be great to have a synchronization process running on the devices
where data will be synchronized automatically with a server when the network
is available and cache data locally whenever no network connection is available.

This may be combined with `GeoGIG <http://geogig.org/>`_ or other server
components like a database.

Improved coordinate precision
.............................

Computers work mostly with two different datatypes for storing decimals (floating
point numbers) internally. `Single <https://en.wikipedia.org/wiki/Single-precision_floating-point_format>`_
and `double <https://en.wikipedia.org/wiki/Double-precision_floating-point_format>`_
precision. With single precision only the first 6 digits of a number
are significant and can be trusted, with double precision the first 15
digits can be trusted. This takes all digits (before and after the decimal
point) into account.

When working with coordinates this is very important when a high accurracy is
required. Only having 6 significant digits often means having a maximum
accurracy around one meter but depends on the CRS and location on the earth
surface.

Some parts of QField currently treat coordinates with single point precision. It
would be a very good improvement to be able to use the full precision of
accurate positioning devices by fixing this shortcoming.

Saving GPS status information into attributes
.............................................

Positioning devices offer more information than just latitude and longitude.
They are also able to estimate the precision which they are currently able to
provide. This information can be a radius calculated from variables like the
number of satellites but also additional flags for parameters like the RTK
status.

Editing geometries
..................

QField offers to digitize new geometries, edit existing attributes or delete
features. What it lacks is the feature to edit existing geometries.

This can be split into different pieces like moving points, moving other
features, editing vertices, moving edges...

Search functionality
....................

Locating features with the help of a search field is something that can really
help to work more efficient. In general this can be split into two categories:

Attribute based search
  Search results are provided based on the attributes of features on layers
  within the project.
  Either a local database for searching is available. The data inside is based
  on the customized data you work with or the layers are queried directly for
  features with attributes in question.

Geocoding
  Search results are provided based on real world addresses. Services like
  `Nominatim <http://nominatim.openstreetmap.org/>`_ can be accessed via a web
  interface and convert real-world addresses to locations on a map. Just try the
  link.

External measurement devices
............................

Are you working with external devices with sensors? Air pressure, temperature,
slope, carbon monoxide concentration, you name it... Wouldn't it be great to not
having to read the information from your external device and manually typing it
again? We would love to make life easier for you by adding support for directly
connecting your external measurement device to QField.

Record Audio
............

Ever wondered how you would write a lot of text when out in the field with a
touch screen?

Just don't!

It would be amazing to have the possibility to have audio integration just like
image integration. When back in the office, you could just type in the
information on a regular keyboard.


[Your favorite feature]
.......................

You probably have plenty of ideas for other nice features. Please contact us and
we can discuss the best approach to integrate this idea into QField or a derived
app.

*******
Funding
*******

The following oranizations have contributed to make QField what it currently is.
We would like to warmly say: Thanks a lot!

.. container:: clearer text-center

    .. image:: /images/logo_sige.jpg
       :width: 200px
       :alt: SIGE
       :target: http://www.sige.ch
    
    .. image:: /images/logo_vogelwarte.gif
       :width: 200px
       :alt: Schweizerische Vogelwarte
       :target: http://www.vogelwarte.ch
    
    .. image:: /images/logo_geoconseils.png
       :width: 200px
       :alt: Géoconseils SA
       :target: http://www.geoconseils.ch
    
.. container:: clearer text-center

    .. image:: /images/logo_hydrosafety.png
       :width: 200px
       :alt: hydro safety srls
       :target: http://www.hydro-safety.com/

    .. image:: /images/logo_vevey.png
       :width: 100px
       :alt: Ville de Vevey
       :target: http://www.vevey.ch
    
    .. image:: /images/logo_morges.png
       :width: 100px
       :alt: Ville de Morges
       :target: http://www.morges.ch

*******
Credits
*******

QField is developped by OPENGIS.ch. OPENGIS.ch does consulting, development and
training for open source software including QField and QGIS.

.. image:: /images/openGis-websiteLogo.png
   :alt: OPENGIS.ch
   :target: http://www.opengis.ch

*************
API stability
*************

QField is currently not considered API stable.
