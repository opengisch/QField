---
title: Attribute Form Configuration
---

QField creates forms similar to, but not equal to QGIS.

Suppress Attribute Form
=======================

The setting \"suppress attribute form\" is directly applied to the
QField form.

Field widget {#edit_field_widgets}
============

The field widgets from QGIS are supported on a best effort basis.

+---------------+-------------+---------------------------------------+
| Widget type   | Support     | Notes                                 |
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
| Attachment    | [✔]{.yay}   | This field is combined with camera    |
|               |             | integration. It is also able to open  |
|               |             | other files like pdf and doc (if the  |
|               |             | user has an appropriate viewer)       |
|               |             | `attachment`{.interpreted-text        |
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
| Value         | [✔]{.yay}   |                                       |
| Relation      |             |                                       |
+---------------+-------------+---------------------------------------+
| Others        | [✘]{.nay}   | `make_it_grow`{.interpreted-text      |
|               |             | role="ref"}                           |
+---------------+-------------+---------------------------------------+

Relation Widget
===============

For more information refer to the relations chapter of the official
[QGIS
Documentation](https://docs.qgis.org/3.4/en/docs/user_manual/working_with_vector/attribute_table.html#creating-one-or-many-to-many-relations).

::: {.container .clearer .text-center}
![](/images/relation_editor_widget_list.png){width="500px"}
:::

The relation widget shows all the referencing child features in a list.
It is possible to add, delete and open them.

The visual identification of the list entries are done via the \"Display
Expression\" of the child layer.

**Key handling**

Since the parents primary keys are used as foreign keys on the
referencing child features, these primary keys must be save to use even
after the synchronization back to the original data. Therefore we
recommend to use stable values such as UUIDs for primary keys.

It is possible to already add children from a parent that is about to be
added. This possibility will be blocked if there is no valid primary key
on the parent or the constraints are violated. In case the adding of the
parent feature is canceled after some children have been added already,
the children are deleted as well.

**Many-To-Many relations**

On many-to-many relationships, according to the cardinality setting in
the QGIS Vector Layer Properties, on adding, deleting or opening an
entry in the list effects directly the child layer (on direct
cardinality) or the linking table (on many-to-one cardinality).

The second case is usually used when there are additional relation
information (e.g. percentage) in the linking table.

::: {.container .clearer .text-center}
![](/images/relation_widget_cardinality.png){width="500px"}
:::

In case of the many-to-one cardinality on many-to-many relations the
Display Expression needs to be set on the linking table.

Attachment (photo settings) {#attachment}
---------------------------

The widget type \"Attachment\" is used with fields that store the path
to files.

It can be used to:

-   show and take photos
-   show links to external files like pdfs or documents

::: {.container .clearer .text-center}
![](/images/attachement-setting.png){width="600px"}
:::

The path should be set to relative. The pictures, pdf and docs are
stored then in a sub-directory of the project, where the link stored in
the textfield is pointing to.

In QField the picture is displayed. By clicking the camera symbol the
camera is started to update the picture with a new photo.

::: {.container .clearer .text-center}
![](/images/qfield_picture.png){width="600px"}
:::

On synchronization the sub-directory with the pictures, pdf and docs has
to be copied as well.

The option \"hyperlink\" on the attachment widget will disable the
camera functionality and enable a hyperlink to open a document (pdf,
doc, \...).

::: {.container .clearer .text-center}
![](/images/hyperlink_option.png){width="600px"}
:::

Value Relation
==============

The widget \"Value Relation\" offers values from a related table in a
combobox. You can select layer, key column and value column. Several
options are available to change the standard behaviors: allow null
value, order by value and allow multiple selections. The widget will
display a drop-down or multiple-select list of the referenced features.

::: {.container .clearer .text-center}
![](/images/value_relation_widget.gif){width="600px"}
:::

Drag and drop designer forms
============================

Groups and Tabs from Drag and Drop designer forms are supported.

Conditional visibility
======================

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
checked.](/images/conditional_visibility_configuration.png){width="600px"}
:::

Let\'s see what we got on the field:

::: {.container}
::: {.vimeo}
499565113
:::
:::

Constraints
===========

Attribute fields can have constraints attached. Constraints are
expressions that needs to evaluate to `True` (or `1`) in order to be
able to save the form. Constraints are configured on a per-field basis
and a description can be added that will be shown to the user if a
constraint is not satisfied.

::: {.container .clearer .text-center}
![Configuration of a constraint within a
range](/images/constraint_configuration.png){width="600px"}
:::

Common use cases
----------------

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
==============

Fields can have default values configured. Default values are inserted
into the attribute form when digitizing a new feature. They are visible
to the user and can be modified as long as the field is editable. They
can be based on the geometry or expression variables.

::: {.container .clearer .text-center}
![Configuration of a formatted date as default
value](/images/default_value_configuration.png){width="600px"}
:::

Additional variables
--------------------

You can get access to positioning information through additional
expression variables. These will only be available when the positioning
is enabled, either through the internal device receiver or through an
external device connected via NMEA connection.

-   [\@position\_coordinate]{.title-ref}
    -   A point with the coordinate in WGS84. Lon, Lat, Altitude as
        delivered by the sensor. It is only available when the crosshair
        is snapped to the sensor.
    -   [x(\@position\_coordinate)]{.title-ref}
    -   IE
-   [\@position\_timestamp]{.title-ref}
    -   The timestamp of the position in UTC as reported by the sensor.
        It is only available when the crosshair is snapped to the
        sensor.
    -   IE
-   [\@position\_direction]{.title-ref}
    -   The direction of movement in degrees from true north as reported
        by the sensor. It is only available when the crosshair is
        snapped to the sensor.
    -   IE
-   [\@position\_ground\_speed]{.title-ref}
    -   Groundspeed (in m/s) as reported by the sensor. It is only
        available when the crosshair is snapped to the sensor.
    -   IE
-   [\@position\_magnetic\_variation]{.title-ref}
    -   The angle between the horizontal component of the magnetic field
        and true north, in degrees as reported by the sensor. Also known
        as magnetic declination. A positive value indicates a clockwise
        direction from true north and a negative value indicates a
        counter-clockwise direction. It is only available when the
        crosshair is snapped to the sensor.
    -   IE
-   [\@position\_horizontal\_accuracy]{.title-ref}
    -   The horizontal accuracy of the coordinate (in meters) as
        reported by the sensor. It is only available when the crosshair
        is snapped to the sensor.
    -   IE
-   [\@position\_vertical\_accuracy]{.title-ref}
    -   The vertical accuracy of the coordinate (in meters) as reported
        by the sensor. It is only available when the crosshair is
        snapped to the sensor.
    -   IE
-   [\@position\_3d\_accuracy]{.title-ref}
    -   The 3 dimensional accuracy of the coordinate (in meters), 3D-RMS
        as reported by the sensor. It is only available when the
        crosshair is snapped to the sensor.
    -   IE
-   [\@position\_vertical\_speed]{.title-ref}
    -   The vertical speed (in m/s) as reported by the sensor. It is
        only available when the crosshair is snapped to the sensor.
    -   IE
-   [\@position\_source\_name]{.title-ref}
    -   The name of the device that gave location information as
        reported by the sensor. If the position is manually set, the
        source name is \"manual\".
    -   IE
-   [\@position\_pdop]{.title-ref}
    -   Position dilution of precision as reported by the sensor. It is
        only available when the crosshair is snapped to the sensor.
    -   E
-   [\@position\_hdop]{.title-ref}
    -   Horizontal dilution of precision as reported by the sensor. It
        is only available when the crosshair is snapped to the sensor.
    -   E
-   [\@position\_vdop]{.title-ref}
    -   Vertical dilution of precision as reported by the sensor. It is
        only available when the crosshair is snapped to the sensor.
    -   E
-   [\@position\_number\_of\_used\_satellites]{.title-ref}
    -   Number of satellites as reported by the sensor. It is only
        available when the crosshair is snapped to the sensor.
    -   E
-   [\@position\_used\_satellites]{.title-ref}
    -   A list of satellites in use (pri) as reported by the sensor. It
        is only available when the crosshair is snapped to the sensor.
    -   [array\_count(\@position\_used\_satellites)]{.title-ref}
    -   E
-   [\@position\_quality\_description]{.title-ref}
    -   A human readable and translated string for the quality as
        reported by the sensor. E.g. \"Fixed RTK\". It is only available
        when the crosshair is snapped to the sensor.
    -   E
-   [\@position\_fix\_status\_description]{.title-ref}
    -   The GPS Fix Status \"NoData\", \"NoFix\", \"Fix2D\" or \"Fix3D\"
        as reported by the sensor. It is only available when the
        crosshair is snapped to the sensor.
    -   E
-   [\@position\_fix\_mode]{.title-ref}
    -   Fix mode (where \'M\' = Manual, forced to operate in 2D or 3D or
        \'A\' = Automatic, 3D/2D) as reported by the sensor. It is only
        available when the crosshair is snapped to the sensor.
    -   E

All \@position\_\* variables have a corresponding \@gnss\_\* variable.
The gnss variables always report the gnss sensor values, even when the
crosshair is not snapped.

Examples:

:   -   when the crosshair is snapped to the sensor
        -   [\@gnss\_horizontal\_accuracy]{.title-ref} \--\> The
            horizontal accuracy of the coordinate (in meters) as
            reported by the sensor.
        -   [\@position\_horizontal\_accuracy]{.title-ref} \--\> The
            horizontal accuracy of the coordinate (in meters) as
            reported by the sensor.
        -   [\@position\_source\_name]{.title-ref} \--\> sensor name.
    -   when the crosshair is manually moved
        -   [\@gnss\_horizontal\_accuracy]{.title-ref} \--\> The
            horizontal accuracy of the coordinate (in meters) as
            reported by the sensor.
        -   [\@position\_horizontal\_accuracy]{.title-ref} \--\> The
            value is [NULL]{.title-ref}.
        -   [\@position\_source\_name]{.title-ref} \--\> The value is
            [manual]{.title-ref}.

I: Internal position source E: External (NMEA) position source

Common use cases
----------------

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

To transform the coordinates received from \@position\_coordinate to the
coordinate system of your project:

``` {.sql}
x(transform(@position_coordinate, 'EPSG:4326', @project_crs ))
y(transform(@position_coordinate, 'EPSG:4326', @project_crs ))
```

Editable
========

The editable flag of fields is respected.

Remember last values
====================

QField offers a much more fine-grained control over the last used values
and ignores the QGIS setting.
