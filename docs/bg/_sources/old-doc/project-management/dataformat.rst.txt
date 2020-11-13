Supported data formats
======================

QField supports a wide variety of formats via QGIS data providers and GDAL.
This page offers a non-exhaustive list of supported data formats.

Table
.....

  .. role:: yay
  .. role:: nay
  .. role:: moreorless

+-----------------+-----------------+---------------------------------------+
| Data Format     | Support         | Notes                                 |
+=================+=================+=======================================+
| Spatialite      | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| Geopackage      | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| WMS             | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| WFS             | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| WFS-T           | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| Postgis         | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| MBTiles         | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| Shapefile       | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| Tiff            | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| JPEG2000        | :yay:`✔`        |                                       |
+-----------------+-----------------+---------------------------------------+
| ECW             | :nay:`✘`        | License restricts usage.              |
+-----------------+-----------------+---------------------------------------+
| MrSID           | :nay:`✘`        | License restricts usage.              |
+-----------------+-----------------+---------------------------------------+

If you don't find your favorite data format on this table, please check if it
works and `adapt the list above <https://github.com/opengisch/QField-docs/edit/master/en/project-management/dataformat.rst>`_ to share your findings. If it does not work,
please `open an issue <https://github.com/opengisch/OSGeo4A/issues>`_. We will be
happy to help you with the implementation.

Raster data
===========

Raster data can become quite big quickly, when working with uncompressed tiff files it's often several Gb of data. Especially on mobile devices, this is inefficient.

Use GeoPackage
..............

We recommend to us the geopackage format to deal with raster data. The following commands will convert a file called :code:`raster.tif` to a file :code:`raster.gpkg` with pyramids. Make sure you adjust EPSG:21781 (CH1903 LV03) to your desired CRS.

.. code:: bash

  gdal_translate --config OGR_SQLITE_SYNCHRONOUS OFF -co  APPEND_SUBDATASET=YES -co TILE_FORMAT=PNG_JPEG -a_srs EPSG:21781 -of GPKG raster.tif raster.gpkg
  gdaladdo --config OGR_SQLITE_SYNCHRONOUS OFF -r AVERAGE raster.gpkg 2 4 8 16 32 64 128 256
