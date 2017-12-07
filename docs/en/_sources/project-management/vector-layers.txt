*********************
Vector Layer Settings
*********************

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

.. container:: clearer text-center

  .. figure:: /images/define_display_expression.png
     :width: 500px
     :alt: Define display expression

     The display expression will be used to represent features with an identification string throughout QField.

Read only and non-identifiable layers
=====================================

Some layers in a project are just there for pure visual purpose. Such
layers should not show up when a user taps somewhere to identify objects.

Some other layers serve as source of information and shouldn't be modified
by the user. It is possible to protect layers from editing attributes or
adding and deleting features.

To configure identify and readonly behavior, go to the :menuselection:`Project --> Project Properties --> Identify Layers`
page and activate the checkboxes to match your desired behavior.

.. container:: clearer text-center

  .. figure:: /images/project_configuration_readonly.png
     :width: 500px
     :alt: Identify and readonly configuration

     Configuration of layers that will not be identifiable and/or not be modifiable.

Forms and Fields
================

QField creates forms similar to, but not equal to QGIS.

Suppress Feature Form
---------------------

The setting suppress feature form is directly applied to the QField form.

.. _edit_field_widgets:

Field widget
------------

The field widgets from QGIS are supported on a best effort basis.

.. role:: yay
.. role:: nay
.. role:: moreorless

+-------------------+-----------------+-------------------------------------------------+
| Field type        | Support         | Notes                                           |
+===================+=================+=================================================+
| Text Edit         | :yay:`✔`        | - HTML is not supported                         |
|                   |                 | - Numerical input is enforced based on the      |
|                   |                 |   type.                                         |
+-------------------+-----------------+-------------------------------------------------+
| Check Box         | :yay:`✔`        |                                                 |
+-------------------+-----------------+-------------------------------------------------+
| Value Map         | :yay:`✔`        |                                                 |
+-------------------+-----------------+-------------------------------------------------+
| Hidden            | :yay:`✔`        |                                                 |
+-------------------+-----------------+-------------------------------------------------+
| External Resource | :moreorless:`~` | This field is combined with camera integration. |
|                   |                 | Only images can be viewed so far.               |
|                   |                 | :ref:`make_it_grow`                             |
+-------------------+-----------------+-------------------------------------------------+
| Date Time         | :yay:`✔`        |                                                 |
+-------------------+-----------------+-------------------------------------------------+
| Others            | :nay:`✘`        | :ref:`make_it_grow`                             |
+-------------------+-----------------+-------------------------------------------------+


Drag and drop designer forms
----------------------------

Groups and Tabs from Drag and Drop designer forms are supported.

Conditional visibility
----------------------

Groups can be hidden based on an expression. This is useful when certain attributes are
only required under certain conditions. As an example imagine mapping trees. Some of them
might have a disease and you have a list of possible deseases. To not clutter the form with
many fields, make a group and configure a visibility expression for a group "diseases". Add
a field "desease" with a checkbox. Only when the checkbox is checked, the list of diseases
will be available to the user.

.. container:: clearer text-center

  .. figure:: /images/conditional_visibility_configuration.png
     :width: 600px
     :alt: Conditional visibility configuration

     Configuration of a group box that will only be shown if the checkbox "measures_taken" is checked.

Constraints
-----------

Attribute fields can have constraints attached. Constraints are expressions that needs to
evaluate to :code:`True` (or :code:`1`) in order to be able to save the form. Constraints
are configured on a per-field basis and a description can be added that will be shown to the
user if a constraint is not satisfied.

.. container:: clearer text-center

  .. figure:: /images/constraint_configuration.png
     :width: 600px
     :alt: Constraint configuration

     Configuration of a constraint within a range

Common use cases
................

You cannot enter an elevation value higher than the highest mountain in this country.

.. code-block:: sql

  "elevation" < 5000

It is required to fill in an identifier.

.. code-block:: sql

  "identifier" IS NOT NULL

Default values
--------------

Fields can have default values configured. Default values are inserted into the
attribute form when digitizing a new feature. They are visible to the user and can
be modified as long as the field is editable. They can be based on the geometry or
expression variables.

.. container:: clearer text-center

  .. figure:: /images/default_value_configuration.png
     :width: 600px
     :alt: Default value configuration

     Configuration of a formatted date as default value

Additional variables
....................

In QField, there are some additional variables available regarding the current
positioning information.

- `position_coordinate` (Geometry in WGS84!) : Lon, Lat, Altitude as delivered 
  by the sensor. Is also available when the crosshair is not snapped to the sensor.
- `position_timestamp` (DateTime) : Timestamp of the position information
- `position_direction` (float) : Direction in degrees from true north.
- `position_ground_speed` (float) : Groundspeed in meters / second.
- `position_magnetic_variation` (float) : The angle between the horizontal 
  component of the magnetic field and true north, in degrees. Also known as 
  magnetic declination. A positive value indicates a clockwise direction from 
  true north and a negative value indicates a counter-clockwise direction.
- `position_horizontal_accuracy` (float) : The horizontal accuracy of the 
  coordinate (in meters). 
- `position_vertical_accuracy` (float) : The vertical accuracy of the 
  coordinate (in meters).
- `position_vertical_speed` (float) : The value of the vertical speed in meters
  per second. 

Common use cases
................

Insert the current date and time:

.. code-block:: sql

  now()

Insert the length of the digitized line:

.. code-block:: sql

  length($geometry)

Configure global variables on the device and insert them.

.. code-block:: sql

  @operator_name

If you want to assign a region code based on the location where a new feature is
inserted, you can do so by using an aggregate expression:

.. code-block:: sql

  aggregate( layer:='regions', aggregate:='max', expression:="code", filter:=intersects( $geometry, geometry( @parent ) ) )

Editable
--------

The editable flag of fields is respected.

Remember last values
--------------------

QField offers a much more fine-grained control over the last used values and ignores the QGIS setting.

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
