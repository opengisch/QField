Data collection by QGIS/QField for O&M work of rural water supply systems in Rwanda
=========================================================================================================

By Rural Water and Sanitation Services Department(RWSS), Water and Sanitation Corporation (`WASAC <https://wasac.rw>`__), Rwanda

Goal
----

To conduct data collection of all rural water supply network in Rwanda, 
and keep updating the data continuously in order to improve operation & 
maintenance of waterworks.

Project preparation
-------------------

Before starting our data collection, we conducted the following things.
 - Develop our own PostGIS database
 - Develop QGIS project template with Geopackage. The Geopackage table design is equal to PostGIS to be able to copy and paste to PostGIS.

Apart from preparing Android devices, we purchased GPS devices for higher positioning accuracy. In WASAC, we bought Garmin GPSMAP 64S. 
Sometimes, GPS of smartphone and tablet is not very accurate, so we normally capture the same location by using Garmin GPS, then correct the location of QField's data after data collection work.

Data collection
---------------

Once we prepared Geopackage and QGIS project template, 
we conducted training of QGIS/QField in July 2018 and 
launched our data collection work in 27 districts in the whole country of Rwanda. 
27 engineers sent their Geopackage to the central office in Kigali.
the MIS (Management Information System) specialist validated and entered their data from Geopackage to PostGIS database. 
We completed our initial data collection works until April 2019.

.. container:: clearer text-center

   .. image:: /images/rwanda-rural-water-1.png
      :width: 600px
      :alt: data collection procedure

Data distribution and updating
------------------------------

The most significant thing after data collection is ``updating``. 
We have seen many organization in Africa, which failed to keep data up to date.
Several years later, their data will normally become too old, 
and most of them need to put efforts on data collection again. 

WASAC decided to continuously update all of the data and keeps doing this until now. 
QField has proven to be very well suited for this purpose.

In order to distribute and updating the data, 
we developed a python script `postgis2qfield <https://github.com/WASAC/postgis2qfield>`__. 
This ``postgis2qfield`` tool can extract the data from PostGIS and create Geopackages for each district in Rwanda. 
We upload these 27 geopackage together with QGIS project template to Google Drive storage. 
After that, those engineers in districts download their geopakage to Android device to continue adding and updating the data. 
Once they completed updating, they sent the geopackage to central office again, MIS specialist update PostGIS database and regenerate geopackages for QField.

.. container:: clearer text-center

   .. image:: /images/rwanda-rural-water-2.png
      :width: 600px
      :alt: data distribution and updating procedure

Data sharing via vectortiles
----------------------------
First of all, you can see our collected data from `here <https://rural.water-gis.com>`__.

Since July 2020, we started to distribute our water supply systems' data via vectortiles as open data. 
Although Rwanda's internet situation is being improved, some rural area still have problems of internet. 
In such as poor internet situation, WMS or WFS data distribution will not work well. 
Vectortiles can provide light and fast distribution of map data. We will not talk about our vectortiles here. 
If you are fascinated by how to share the result of data collection, please also see this `instruction <https://github.com/watergis/awesome-vector-tiles>`__. 

Acknowledgement
---------------

We thank all of district water and sanitation support engineers to
conduct their data collection work. Additionally, we want to thank the
developers of QField and QGIS for offering fantastic open source
software. It is great that, due to free software, such projects can be
implemented by an organization of water sector in developing countries.

About WASAC
-----------

``WASAC`` has 2 main departments for urban water(UWSS) and rural
water(RWSS). We are using QField in RWSS. The role of RWSS department is
to support local government to operate and maintain their owned water
supply systems in rural area. Nowadays, these data collected and
maintained by RWSS department are being used by more than 30 private
operators in 27 districts. Total number of water supply systems in the
database is 1,000+.

.. container:: clearer text-center

   .. image:: /images/rwanda-rural-water-3.png
      :width: 600px
      :alt: Organogram of WASAC

Also, one of our colleagues presented WASAC's activity in FOSS4G 2019
Bucharest. Although some of system were little bit changed now, you can
also see
`video <https://media.ccc.de/v/bucharest-30-case-study-of-data-collection-data-sharing-for-rural-water-supply-management-in-rwanda>`__
if you are interested.
