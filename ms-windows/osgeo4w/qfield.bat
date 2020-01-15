SET GDAL_DATA="%0\..\gdal" rem "C:\OSGeo4W\apps\gdal-dev\share\gdal"
SET PROJ_LIB="%0\..\proj" rem "C:\OSGeo4W\apps\proj-dev\share\proj"
SET QGIS_PREFIX_PATH="%0\..\qgis"  rem "C:\OSGeo4W\apps\qgis-dev" +/-

CALL %0\..\qfield.exe
