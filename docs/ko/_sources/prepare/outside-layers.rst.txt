Add a layer outside of exported project folder
==============================================

In a QField project it is possible to use a layer outside the project folder, like a basemap.

It is useful if a basemap is used in all your project, no need to copy it everytime in the project folder.

In QGIS, open "Options" --> "Data Sources" --> "Localized Data Paths" 
There add the path to the folder where are the external data.

.. image:: ../images/external_path.png

To make it works in the device, add the layers in the folder `QField/basemaps`