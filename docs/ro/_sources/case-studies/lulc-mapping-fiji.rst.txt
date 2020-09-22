Ground Truth Data Collection Using QField for LULC Mapping in Fiji
==================================================================

By Kevin Davies, School of Geosciences, The University of Sydney

Purpose
-------

Communities in Fiji rely on landscape resources for agricultural and forestry-related activities.
Accurate mapping and monitoring patterns of land use and land cover (LULC) over time at 
an appropriate scale is important for informing landscape management, policies, and climate-smart 
sustainable development. 

Fiji’s Ministry of Forestry is collaboratively developing an approach with the Universities of Sydney
(USYD), Western Australia (UWA) and the South Pacific (USP) to produce an inter-annual LULC map using
Sentinel-2 satellite data, and freely available geospatial tools. QFIeld is being used for collecting
ground truth data in the landscape for training and validation of the LULC map.


Workflow
--------

1. The LULC ground truth collection form was designed in QGIS.
2. A set of predefined ground truth plot locations were generated based on a stratification of satellite data within the study area.
3. The form, predefined plots, and appropriate offline background layers were packaged in QGIS and then loaded onto each of the tablets used by the field team (Figure 1).
4. A team member navigated to a predefined plot in QField and created a ground truth point at the location and labelled the point with the most appropriate pre-defined LULC class (Figure 2).
5. Data collected from all tablets was combined into one ground truth data collection in QGIS.
6. Image interpretation using the OpenForis platform will be used to increase the number of ground truth plots.
7. The final ground truth collection will be imported into Google Earth Engine to produce the LULC map and calculate the map accuracy.

.. container:: clearer text-center

  .. figure:: /images/use_study_fiji1.png
          :width: 400px
          :alt: Figure 1: Predefined Ground Truth Plots

          Figure 1: Predefined Ground Truth Plots

  ..  figure:: /images/use_study_fiji2.png
          :width: 400px
          :alt: Figure 2: Capturing land cover class

          Figure 2: Capturing land cover class

Preliminary Results and Future Work
-----------------------------------

An example of a preliminary land cover map is shown in Figure 3.
An important objective from our work is to transfer skills and build capacity with local stakeholders
to continue to update the LULC map on an annual basis as well as to expand the map to include other 
communities, catchments and forestry areas across Fiji. This capacity building will include iterative 
stakeholder consultation, online training materials, field and classroom training workshops, and 
collaborative fieldwork.
       
.. container:: clearer text-center

    .. figure:: /images/use_study_fiji3.png
       :width: 400px
       :alt: Figure 3: Preliminary land cover map for the Ba region, Viti Levu, Fiji

       Figure 3: Preliminary land cover map for the Ba region, Viti Levu, Fiji 

Acknowledgement
---------------

We would like to thank the field team from the Fiji Ministry of Forestry especially Viliame Tupua
and Renata Varea (USP). The project was funded by the Australian Centre for International Agricultural
Research (ACIAR; ASEM/2016/101). 

.. container:: clearer text-center

    .. figure:: /images/use_study_fiji4.jpg
       :width: 400px
       :alt: The Fiji Forestry/USP field team is about to collect ground truth data with QField.
 
       The Fiji Forestry/USP field team is about to collect ground truth data with QField.

