Search
======
Tap on the **Search button** on the top-right corner of the screen to search for features in the current project. 

1. Tap on the **name** in the result to pan the map on the feature (the feature will be highlighted in yellow).
2. Tap on the **attributes button** to open its attributes.


GoTo Coordinate
---------------
It is possible to go to coordinates with the search tool. 

Tap on the **coordinates** in the result to go to the coordinates.
It is always possible to search WGS84 coordinates (*longitude, latitude*) or current map CRS (*x,y*).

.. container::

  .. vimeo:: 499566922

Search in value list
======================

.. _search_value_list:

It is possible to search value in a value list using the magnifying glass next to the field.

.. container::

  .. vimeo:: 604661919

To be able to use the "completer" the widget has to be set up :ref:`Autocomplet form set up<autocomplete_value>`.

Search in layers
================

You can search for features in vector layers.
The search is currently performing the same as the *all layers locator filter* in QGIS
(accessible in QGIS locator bar using *af* prefix).

Configure search in QGIS
------------------------

1. Choose the layers which are searchable in :menuselection:`Project --> Project Properties --> Identify Layers`
2. Define the *display expression* for the concerned layers, this will be used for searching for features. This is configured under :menuselection:`Vector Layer Properties --> Display` page

