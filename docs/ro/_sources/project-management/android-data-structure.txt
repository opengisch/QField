Android: Special File Access Notes
==================================

Custom SVG symbols and settings
-------------------------------

In the about dialog you can see where the shared folders are on your device.

If you want to use custom symbols you need to put them there using a file manager.

External SD card
----------------

In order to have write access to projects which are on an external SD card, you will have to put it into the following folder:

.. code-block:: none

    /Android/data/ch.opengis.qfield/files

Warning!
--------
By storing data in this folder, you risk losing data. You can lose data by uninstalling QField from your Android device. Android system will delete the /Android/data/ch.opengis.qfield directory. This is important if you keep the data in the External sd card.
