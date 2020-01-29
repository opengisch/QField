Allow hiding legend nodes
==================================

First, you should to verify if the plugin **Invisible layers and groups** is installed, if you don't have installed, you can do it going to **Plugins > Manage and install plugins > find Invisible layers and groups > click in install plugin**.

.. container:: clearer text-center

  .. figure:: ../images/install_plugin.png
     :width: 500px
     :alt: Install plugin Invisible layers and groups

When you finish the installation of the plugin, you will see in the QGIS interface this buttons:  

.. container:: clearer text-center

  .. figure:: ../images/plugin_invisible_layers.png
     :width: 80px
     :alt: Install plugin Invisible layers and groups

Once the installation process is finished, it is possible to start hiding the nodes of the legend our project.

1. Choose the layers or groups that you want to hide.
2. When you finish the layer/group selection, **click Make selected layers and groups invisible**. You will notice the selected layers and groups have disappear from the layers panel. They are still available in the project for visualization and joins, though.

.. container:: clearer text-center

  .. figure:: ../images/plugin_invisible_layers_hide.png
     :width: 40px
     :alt: Install plugin Invisible layers and groups

3. Save your project, you can use the section QFieldSync plugin to save your project, or save your geopackage and project in the same folder.
4. Copy  the files generated in your device.
5. Open QField in your device, and you will see the configuration of hidden legend nodes. 

.. container:: clearer text-center

  .. figure:: ../images/process_hide_and_show_layers.gif 
     :width: 500px
     :alt: Hide and show layers in Qgis and QFiedl.

Note: If you want to see the hidden layers/groups again, click on show invisible layers and groups.

.. container:: clearer text-center

  .. figure:: ../images/plugin_invisible_layers_show.png
     :width: 40px
     :alt: Plugin Invisible layers and groups show