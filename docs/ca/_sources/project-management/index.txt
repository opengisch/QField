QField Project Management
=========================

Portable Project
----------------

You will need a portable version of your QGIS project file (.qgs). Portable
means that all paths are relative and datasets are reachable from the device.

Check that :menuselection:`Project --> Project Properties --> General --> Save paths` 
is set to "Relative" and that all required data files are in the same folder like the .qgs file or a subfolder of it.

For increased productivity, we suggest having a look at the `QFieldSync plugin <https://plugins.qgis.org/plugins/qfieldsync>`_.

Coordinate Reference Systems
----------------------------

QField uses the Coordinate Reference System (CRS) information found in the project file.

* Project CRS
* On the fly reprojection (OTF) is used for visualization
* Layer CRS

.. note::
    QField does not respect OTF when digitizing new features.
    Make sure that all the layers on which you digitize are in the same CRS like the project.
    If you fail to do so it will look as if new features are not saved because they end up in far far away.

Vector Layer Settings
---------------------

Most of the settings which QGIS offers are directly supported by QField without any extra effort.

Style
.....

All style settings from QGIS are directly supported by QField. This includes all renderer types like graduated, categorized, rule based, 2.5D as well as data defined symbology.

Forms and Fields
................

QField creates forms similar to, but not equal to QGIS.

Suppress Feature Form
  The setting suppress feature form is directly applied to the QField form.

Remember last values
  QField offers a much more fine-grained control over the last used values and ignores the QGIS setting.

Field settings
  The field settings from QGIS are supported on a beste effort basis and is constantly being improved.

  .. role:: yay
  .. role:: nay
  .. role:: moreorless

  +-------------------+-----------------+-------------------------------------------------+
  | Field type        | Support         | Notes                                           |
  +===================+=================+=================================================+
  | Text Edit         | :yay:`✔`        | - Multiline is always allowed                   |
  |                   |                 | - HTML is not supported                         |
  +-------------------+-----------------+-------------------------------------------------+
  | Check Box         | :yay:`✔`        |                                                 |
  +-------------------+-----------------+-------------------------------------------------+
  | Value Map         | :yay:`✔`        |                                                 |
  +-------------------+-----------------+-------------------------------------------------+
  | Hidden            | :yay:`✔`        | :ref:`make_it_grow`                             |
  +-------------------+-----------------+-------------------------------------------------+
  | External Resource | :moreorless:`~` | This field is combined with camera integration. |
  |                   |                 | Only images can be viewed so far.               |
  |                   |                 | :ref:`make_it_grow`                             |
  +-------------------+-----------------+-------------------------------------------------+
  | Date Time         | :nay:`✘`        | :ref:`make_it_grow`                             |
  +-------------------+-----------------+-------------------------------------------------+
  | Others            | :nay:`✘`        | :ref:`make_it_grow`                             |
  +-------------------+-----------------+-------------------------------------------------+


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

Custom SVG symbols and settings
===============================

In the about dialog you can see where the shared folders are on your device.

If you want to use custom symbols you need to put them there using a file manager.

External SD card
================

In order to have write access to projects which are on an external SD card, you will have to put it into the following folder:

.. code-block:: none

    /Android/data/ch.opengis.qfield/files

