==============
Authentication
==============

To successfully load OAuth2-protected layer(s) in QField, the QGIS project must be
setup to use OAuth2.

Prepare the authentication in QGIS
----------------------------------

To setup a WFS with OAuth2 in QGIS follow these steps in the QGIS project configuration.

In the WFS layer configuration:

.. container:: clearer text-center

    .. image:: /images/oauth2_setup_wfs.png
       :width: 600px
       :alt: widgets

Add a new authentication configuration. Parameters need to be adjusted to the oauth2 server
setup:

.. container:: clearer text-center

    .. image:: /images/oauth2_setup_auth.png
       :width: 600px
       :alt: widgets

Export the authentication from QGIS
-----------------------------------

.. note::
   Exporting an auth configuration requires at least QGIS 3.20.
   It is possible to only use QGIS 3.20 to prepare the auth config and configure the
   QGIS project itself with an earlier version of QGIS.
 
.. note::
   It is important that the Configuration ID of the exported configuration
   and the configuration stored in the QGIS project match.

At this point, verify that you are able to properly connect to the service from QGIS.
This authentication configuration must first be exported from QGIS itself.

To do so, users open the QGIS options dialog and focus on the authentication panel.
There, the user selects the OAuth2 configuration used by the project layer(s) by
clicking on the appropriate row in the configurations table widget. Then, the user
uses the lower-right utilities button to select the
‘export selected authentication configurations to file’ action.

.. container:: clearer text-center

    .. image:: /images/oauth2_export_config.png
       :width: 600px
       :alt: widgets

When prompted for a password, leave it blank. The user will be warned that he/she might
be leaking sensitive information. In the case of OAuth2 configurations, it is safe to do so.

Once the user has exported the authentication configuration to an XML file, he/she copies
that file onto the device(s) running QField.
The file has to be copied to the device’s <main storage>/QField/auth/ directory.
The `/QField/auth/` path is created when first launching QField, look for it if you are not
sure which storage (external or internal) is the right one.

Once the authentication configuration XML file is copied, users can copy the project file
containing OAuth2-protected layer(s) onto the device(s) and open the project in QField.
The user will be prompted for his/her user, password, and 2-step-verification authentication
code within a browser panel that will pop up in QField itself.

Once authenticated, content will be rendered without the need to re-enter user
information for the duration of the session.
