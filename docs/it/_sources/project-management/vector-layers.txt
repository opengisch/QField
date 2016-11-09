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

  .. image:: /images/define_display_expression.png
     :width: 500px
     :alt: Define display expression

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

  .. image:: /images/conditional_visibility_configuration.png
     :width: 600px
     :alt: Conditional visibility configuration

Constraints
-----------

Attribute fields can have constraints attached. Constraints are expressions that needs to
evaluate to :code:`True` (or :code:`1`) in order to be able to save the form. Constraints
are configured on a per-field basis and a description can be added that will be shown to the
user if a constraint is not satisfied.

.. container:: clearer text-center

  .. image:: /images/constraint_configuration.png
     :width: 600px
     :alt: Constraint configuration

Common use cases
................

You cannot enter an elevation value higher than the highest mountain in this country.

.. code-block:: sql

  "elevation" < 5000

It is required to fill in an identifier.

.. code-block:: sql

 "identifier" IS NOT NULL`

Default values
--------------

Fields can have default values configured. Default values are inserted into the
attribute form when digitizing a new feature. They are visible to the user and can
be modified as long as the field is editable. They can be based on the geometry or
expression variables.

.. container:: clearer text-center

  .. image:: /images/default_value_configuration.png
     :width: 600px
     :alt: Default value configuration

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

