
Add 1:n pictures
====================

You can add one or more pictures to a feature. Below you find an exemple how to proceed.

Creating two tables
--------------------

**Apiary**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* id (UUID)
* geometry
* ...

**Apiary_pictures**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* id
* apiary_id (UUID)
* path (TEXT)
* ...

Relations
----------

Create a relation with:

* ``apiary`` Referenced layer
* ``id`` Referenced field
* ``apiary_picture`` Referencing layer
* ``apiary_id`` Referencing field

.. container:: clearer text-center

    .. image:: /images/add-1-n-pictures-relations.png
       :width: 600px
       :alt: Relations

Widgets
--------

apiary
~~~~~

Set the default value of id to ``uuid()``. No need to show it in the form.

.. container:: clearer text-center

    .. image:: /images/add-1-n-pictures-widgets.png
       :width: 600px
       :alt: widgets
Set the relation widget to ``Many to one relation``

.. container:: clearer text-center

    .. image:: /images/add-1-n-pictures-widgets2.png
       :width: 600px
       :alt: widgets2

picture
~~~~~~~

Set the widget type of path to ``attachment``

.. container:: clearer text-center

    .. image:: /images/add-1-n-pictures-widget_picture.png
       :width: 600px
       :alt: widget picture
