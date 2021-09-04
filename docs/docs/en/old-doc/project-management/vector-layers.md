---
title: Vector Layer Settings
---

Most of the settings which QGIS offers are directly supported by QField
without any extra effort.

Style
=====

All style settings from QGIS are directly supported by QField. This
includes all renderer types like graduated, categorized, rule based,
2.5D as well as data defined symbology.

Display Expression
==================

In QField, objects are identified with a name. The expression to
generate this name can be defined by opening the attribute table in QGIS
and switching to the form view. Choose the appropriate field or an
expression in the list there. Starting from QGIS 3, this can be done
directly from the
`Vector Layer Properties --> Display`{.interpreted-text
role="menuselection"} page.

The display expression is also used to search in layers.

::: {.container .clearer .text-center}
![The display expression will be used to represent features with an
identification string throughout
QField.](../../assets/images/define_display_expression.png)
:::

Read only, non-identifiable and searchable layers
=================================================

Some layers in a project are just there for pure visual purpose. Such
layers should not show up when a user taps somewhere to identify
objects.

Some other layers serve as source of information and shouldn\'t be
modified by the user. It is possible to protect layers from editing
attributes or adding and deleting features.

It is also possible to configure which layers are searchable.

To configure identify, readonly and searchable behavior, go to the
`Project --> Project Properties --> Identify Layers`{.interpreted-text
role="menuselection"} page and activate the checkboxes to match your
desired behavior.

::: {.container .clearer .text-center}
![Configuration of layers that will not be identifiable, not modifiable
and/or not
searchable.](../../assets/images/project_configuration_readonly.png)
:::

Forms and Fields
================

QField creates forms similar to, but not equal to QGIS.

Suppress Feature Form
---------------------

The setting suppress feature form is directly applied to the QField
form.

Field widget {#old_edit_field_widgets}
------------

The field widgets from QGIS are supported on a best effort basis.

+---------------+-------------+---------------------------------------+
| Field type    | Support     | Notes                                 |
+===============+=============+=======================================+
| Text Edit     | [✔]{.yay}   | -   HTML is not supported             |
|               |             | -   Numerical input is enforced based |
|               |             |     on the type.                      |
+---------------+-------------+---------------------------------------+
| Check Box     | [✔]{.yay}   |                                       |
+---------------+-------------+---------------------------------------+
| Value Map     | [✔]{.yay}   |                                       |
+---------------+-------------+---------------------------------------+
| Hidden        | [✔]{.yay}   |                                       |
+---------------+-------------+---------------------------------------+
| External      | [\~]{.      | This field is combined with camera    |
| Resource      | moreorless} | integration. Only images can be       |
|               |             | viewed so far.                        |
|               |             | `old_make_it_grow`{.interpreted-text  |
|               |             | role="ref"}                           |
+---------------+-------------+---------------------------------------+
| Date Time     | [✔]{.yay}   |                                       |
+---------------+-------------+---------------------------------------+
| Relation      | [✔]{.yay}   |                                       |
| Reference     |             |                                       |
+---------------+-------------+---------------------------------------+
| Relation      | [✔]{.yay}   |                                       |
| Widget        |             |                                       |
+---------------+-------------+---------------------------------------+
| Others        | [✘]{.nay}   | `old_make_it_grow`{.interpreted-text  |
|               |             | role="ref"}                           |
+---------------+-------------+---------------------------------------+

### Relation Widget

For more information refer to the relations chapter of the official
[QGIS
Documentation](https://docs.qgis.org/3.4/en/docs/user_manual/working_with_vector/attribute_table.html#creating-one-or-many-to-many-relations).

::: {.container .clearer .text-center}
![](../../assets/images/relation_editor_widget_list.png)
:::

The relation widget shows all the referencing child features in a list.
It\'s possible to add, delete and open them.

The visual identification of the list entries are done a Display
Expression on the child layer.

**Key handling**

Since the parents primary keys are used as foreign keys on the
referencing child features, these primary keys must be save to use even
after the synchronization back to the original data. Therefore we
recommend to use stable values such as UUIDs for primary keys.

Child features can be added before the parent is finaly saved - means
while adding a parent. But it\'s blocked if there is no valid primary
key on the parent or the constraints are violated. In case the adding of
the parent feature is canceled after there have been some childs added
already, the childs are deleted by cascade.

**Many-To-Many relations**

On many-to-many relationships, according to the cardinality setting in
the QGIS Vector Layer Properties, on adding, deleting or opening an
entry in the list effects directly the child layer (on direct
cardinality) or the linking table (on many-to-one cardinality).

The second case is usually used when there are additional relation
information (e.g. percentage) in the linking table.

::: {.container .clearer .text-center}
![](../../assets/images/relation_widget_cardinality.png)
:::

In case of the many-to-one cardinality on many-to-many relations the
Display Expression needs to be set on the linking table.

### External Resource (photo settings)

The widget type \"Attachement\" is used to store the path to collected
photos.

::: {.container .clearer .text-center}
![](../../assets/images/attachement-setting.png)
:::

The path should be set to relative. Means the pictures are stored then
in a sub-directory of the project, where the link stored in the
textfield is pointing to.

In QField the picture is displayed. By clicking the camera symbol the
camera is started to update the picture with a new photo.

::: {.container .clearer .text-center}
![](../../assets/images/qfield_picture.png)
:::

On synchronization the sub-directory with the pictures has to be copied
as well.

Drag and drop designer forms
----------------------------

Groups and Tabs from Drag and Drop designer forms are supported.

Conditional visibility
----------------------

Groups can be hidden based on an expression. This is useful when certain
attributes are only required under certain conditions. As an example
imagine mapping trees. Some of them might have a disease and you have a
list of possible deseases. To not clutter the form with many fields,
make a group and configure a visibility expression for a group
\"diseases\". Add a field \"desease\" with a checkbox. Only when the
checkbox is checked, the list of diseases will be available to the user.

::: {.container .clearer .text-center}
![Configuration of a group box that will only be shown if the checkbox
\"measures\_taken\" is
checked.](../../assets/images/conditional_visibility_configuration.png)
:::

Constraints
-----------

Attribute fields can have constraints attached. Constraints are
expressions that needs to evaluate to `True` (or `1`) in order to be
able to save the form. Constraints are configured on a per-field basis
and a description can be added that will be shown to the user if a
constraint is not satisfied.

::: {.container .clearer .text-center}
![Configuration of a constraint within a
range](../../assets/images/constraint_configuration.png)
:::

### Common use cases

You cannot enter an elevation value higher than the highest mountain in
this country.

``` {.sql}
"elevation" < 5000
```

It is required to fill in an identifier.

``` {.sql}
"identifier" IS NOT NULL
```

Default values
--------------

Fields can have default values configured. Default values are inserted
into the attribute form when digitizing a new feature. They are visible
to the user and can be modified as long as the field is editable. They
can be based on the geometry or expression variables.

::: {.container .clearer .text-center}
![Configuration of a formatted date as default
value](../../assets/images/default_value_configuration.png)
:::

### Additional variables

In QField, there are some additional variables available regarding the
current positioning information.

-   [position\_coordinate]{.title-ref} (Geometry in WGS84!) : Lon, Lat,
    Altitude as delivered by the sensor. Is also available when the
    crosshair is not snapped to the sensor.
-   [position\_timestamp]{.title-ref} (DateTime) : Timestamp of the
    position information
-   [position\_direction]{.title-ref} (float) : Direction in degrees
    from true north.
-   [position\_ground\_speed]{.title-ref} (float) : Groundspeed in
    meters / second.
-   [position\_magnetic\_variation]{.title-ref} (float) : The angle
    between the horizontal component of the magnetic field and true
    north, in degrees. Also known as magnetic declination. A positive
    value indicates a clockwise direction from true north and a negative
    value indicates a counter-clockwise direction.
-   [position\_horizontal\_accuracy]{.title-ref} (float) : The
    horizontal accuracy of the coordinate (in meters).
-   [position\_vertical\_accuracy]{.title-ref} (float) : The vertical
    accuracy of the coordinate (in meters).
-   [position\_vertical\_speed]{.title-ref} (float) : The value of the
    vertical speed in meters per second.

### Common use cases

Insert positioning information as variable:

``` {.sql}
@position_horizontal_accuracy
```

Insert the current date and time:

``` {.sql}
now()
```

Insert the length of the digitized line:

``` {.sql}
length($geometry)
```

Configure global variables on the device and insert them.

``` {.sql}
@operator_name
```

If you want to assign a region code based on the location where a new
feature is inserted, you can do so by using an aggregate expression:

``` {.sql}
aggregate( layer:='regions', aggregate:='max', expression:="code", filter:=intersects( $geometry, geometry( @parent ) ) )
```

Editable
--------

The editable flag of fields is respected.

Remember last values
--------------------

QField offers a much more fine-grained control over the last used values
and ignores the QGIS setting.

Snapping
========

While digitizing new features, new points can be snapped to existing
geometries.

All configuration can be done in
`Project --> Snapping Settings`{.interpreted-text role="menuselection"}
(`Settings --> Snapping Settings`{.interpreted-text
role="menuselection"} in QGIS 2.x).

Snapping Types
--------------

It is possible to snap new points

-   only to nodes of existing geomtries
-   only to segments of existing geomtries
-   to nodes and segments of existing geomtries

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
