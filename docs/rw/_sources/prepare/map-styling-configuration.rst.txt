*****************************
Map styling and configuration
*****************************
Most of the settings which QGIS offers are directly supported by QField without any extra effort.

Style
=====

All style settings from QGIS are directly supported by QField.
This includes all renderer types like graduated, categorized,
rule based, 2.5D as well as data defined symbology.

Display Expression
==================

In QField, objects are identified with a name. The expression to generate this name can be defined
by opening the attribute table in QGIS and switching to the form view. Choose the appropriate
field or an expression in the list there. Starting from QGIS 3, this can be done directly from
the :menuselection:`Vector Layer Properties --> Display` page.

The display expression is also used to search in layers.

.. container:: clearer text-center

  .. figure:: /images/define_display_expression.png
     :width: 500px
     :alt: Define display expression

     The display expression will be used to represent features with an identification string throughout QField.

Read only, non-identifiable and searchable layers
=================================================

Some layers in a project are just there for pure visual purpose. Such
layers should not show up when a user taps somewhere to identify objects.

Some other layers serve as source of information and shouldn't be modified
by the user. It is possible to protect layers from editing attributes or
adding and deleting features.

It is also possible to configure which layers are searchable.

To configure identify, readonly and searchable behavior, 
go to the :menuselection:`Project --> Project Properties --> Identify Layers`
page and activate the checkboxes to match your desired behavior.

.. container:: clearer text-center

  .. figure:: /images/project_configuration_readonly.png
     :width: 500px
     :alt: Identify and readonly configuration

     Configuration of layers that will not be identifiable, not modifiable and/or not searchable.

Snapping
========

While digitizing new features, new points can be snapped to existing
geometries.

All configuration can be done in :menuselection:`Project --> Snapping Settings`
(:menuselection:`Settings --> Snapping Settings` in QGIS 2.x).

Snapping Types
--------------

It is possible to snap new points

- only to nodes of existing geomtries
- only to segments of existing geomtries
- to nodes and segments of existing geomtries

Snapping to layers
------------------

It is also possible to only snap to one or a few layers.

Snapping tolerance
------------------

The snapping tolerance can be specified in map units or pixels.

In almost any case, the units should be set to pixels. We made good
experiences with a tolerance value of 20.

3D Coordinates
==============

GPS devices are capable of measuring the altitude next to the current 2D 
position on the earth surface. This information can be saved 
