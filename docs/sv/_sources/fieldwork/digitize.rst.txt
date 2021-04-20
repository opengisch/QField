Collect features
================

To start digitizing new features, enable the **Edit mode** in the main menu (*pencil button*) and select the layer on which you want to add a new feature (the layer will be highlighted in green).

.. container:: clearer text-left

  .. figure:: ../images/collect_features.webp
     :width: 300px
     :alt: collect_features

Points
------

Navigate the crosshair in the center of the screen to the desired location and
click the **+ button** at the lower right of the screen to confirm the creation of a
new point feature. 

You can use the **keep centered** button to force the crosshair to center on your location if you have enabled the GPS.


Lines and polygons
------------------

Navigate the crosshair in the center of the screen to the desired start of the line or polygon and click the **Plus (+) button** at the lower right of the screen to add the first node.

Proceed with adding points to form you line or polygon by clicking the **Plus (+) button** each time you want to add a new node.

Click the **Minus (-) button** to remove the last added node.

Click the **Close (X) button** to cancel the feature creation completely.

When you have added a least 2 nodes for a line or 3 nodes for a polygon, a **Save button** will appear. Click on it to finish your sketch.

Attribute form
--------------

After digitizing a geometry, the attribute form will be displayed if it is not suppressed
and the user will be asked to enter the attributes for the new feature.
The form which appears allows entering attribute values for the new feature. 

Remember attribute values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
For quick collection of rather homogeneus datasets, it is crucial to not having to enter the same attribute values over an over.
The checkboxes at the right of every attribute allow remembering each attribute individually so that the next time you will add a feature on the same layer, these attributes will be automatically pre-filled.

.. container::

  .. vimeo:: 526919606

Delete features
===============

Deleting features is only possible in digitize mode.

.. container:: clearer text-left

  .. figure:: ../images/delete_features.webp
     :width: 300px
     :alt: delete_features


To delete a feature, identify it first by a short tap on it. Then select it with a long press
in the identification list, select further features to delete if you want and click the
trash button to confirm the deletion.

Edit features
=============

To edit, tap on **Menu button -> pencil.**

You can edit an existing feature by moving, adding or deleting the vertex and also spliting. 

Split tool
----------

It is possible to split an existing geometry by drawing a line where it should be split.

Vertex tool
-----------

It is possible to move and delete vertices of geometries and add new ones.

Demonstration of geometries editing
-----------------------------------

A video demonstration of some editing possibilities:

.. container::

  .. vimeo:: 499564854

Freehand digitizing
-------------------

The freehand digitizing mode allows users to 'draw' lines and polygons using their stylus/pen or mouse.
The mode is available for line/polygon feature additions as well as the geometry editor's rings tool.

The freehand digitizing mode is activated through a new toolbar button which appears when QField is set
to editing mode and a line or polygon vector is selected.

.. container::

  .. vimeo:: 537673220

Topology
--------

If the topology is activated on the project QGIS before exporting for QField, shared nodes are treated
as such when moving / deleting vertices, and additional vertices are added on nodes placed on segments.

.. container::

  .. vimeo:: 499565314


Multi editing
-------------

You can make a multi-selection and edit all the features at the same time. In some cases, it can be very efficient.

.. container::

  .. vimeo:: 499565955
