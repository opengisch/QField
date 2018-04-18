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

How to turn external SD card into internal storage on Android 6 and 7.
--------
We recommend turn external SD card into internal storage. 

NOTE: Please be sure you start with a blank SD card. It needs to be blank because when the card is converted to "Internal" storage, it will be formatted (wiped) and encoded (making the cards data only readable by that device).

1. Go to device “Settings”, then select “Storage”.
2. Select your "SD Card", then tap the “three-dot menu“ (top-right), now select “Settings” from in there.
3. Now select “Format as internal”, and then “Erase & Format”.
4. Your SD Card will now be formatted as internal storage.
5. Reboot your phone.

NOTE: If you don't reboot the phone, many things may not work correctly, so make sure you do.
