

# Class PositioningUtils



[**ClassList**](annotated.md) **>** [**PositioningUtils**](classPositioningUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PositioningUtils**](#function-positioningutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE [**GnssPositionInformation**](classGnssPositionInformation.md) | [**averagedPositionInformation**](#function-averagedpositioninformation-12) (const QList&lt; [**GnssPositionInformation**](classGnssPositionInformation.md) &gt; & positionsInformation) <br> |
|  Q\_INVOKABLE [**GnssPositionInformation**](classGnssPositionInformation.md) | [**averagedPositionInformation**](#function-averagedpositioninformation-22) (const QList&lt; QVariant &gt; & positionsInformation) <br> |
|  Q\_INVOKABLE double | [**bearingTrueNorth**](#function-bearingtruenorth) (const QgsPoint & position, const QgsCoordinateReferenceSystem & crs) <br> |
|  Q\_INVOKABLE [**GnssPositionInformation**](classGnssPositionInformation.md) | [**createEmptyGnssPositionInformation**](#function-createemptygnsspositioninformation) () <br> |
|  Q\_INVOKABLE QgsRectangle | [**createExtentForDevice**](#function-createextentfordevice) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & positionInformation, const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem::fromEpsgId(4326), const QgsRectangle & withinRectangle=QgsRectangle()) <br> |
|  Q\_INVOKABLE [**GnssPositionInformation**](classGnssPositionInformation.md) | [**createGnssPositionInformation**](#function-creategnsspositioninformation) (double latitude, double longitude, double altitude, double speed, double direction, double horizontalAccuracy, double verticalAcurracy, double verticalSpeed, double magneticVariation, const QDateTime & timestamp, const QString & sourceName) <br> |


























## Public Functions Documentation




### function PositioningUtils 

```C++
explicit PositioningUtils::PositioningUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function averagedPositionInformation [1/2]

```C++
static Q_INVOKABLE GnssPositionInformation PositioningUtils::averagedPositionInformation (
    const QList< GnssPositionInformation > & positionsInformation
) 
```



Returns an average [**GnssPositionInformation**](classGnssPositionInformation.md) from a list of position information. 


        

<hr>



### function averagedPositionInformation [2/2]

```C++
static Q_INVOKABLE GnssPositionInformation PositioningUtils::averagedPositionInformation (
    const QList< QVariant > & positionsInformation
) 
```



Returns an average [**GnssPositionInformation**](classGnssPositionInformation.md) from a list of position information 


        

<hr>



### function bearingTrueNorth 

```C++
static Q_INVOKABLE double PositioningUtils::bearingTrueNorth (
    const QgsPoint & position,
    const QgsCoordinateReferenceSystem & crs
) 
```



Returns the true north bearing from a given position and CRS 


        

<hr>



### function createEmptyGnssPositionInformation 

```C++
static Q_INVOKABLE GnssPositionInformation PositioningUtils::createEmptyGnssPositionInformation () 
```



Creates an empty [**GnssPositionInformation**](classGnssPositionInformation.md). 


        

<hr>



### function createExtentForDevice 

```C++
static Q_INVOKABLE QgsRectangle PositioningUtils::createExtentForDevice (
    const GnssPositionInformation & positionInformation,
    const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem::fromEpsgId(4326),
    const QgsRectangle & withinRectangle=QgsRectangle()
) 
```



Returns a reasonable default extent derived from a device's timezone and positioning if available. The extent can optionally be clipped to a QgsRectangle. 


        

<hr>



### function createGnssPositionInformation 

```C++
static Q_INVOKABLE GnssPositionInformation PositioningUtils::createGnssPositionInformation (
    double latitude,
    double longitude,
    double altitude,
    double speed,
    double direction,
    double horizontalAccuracy,
    double verticalAcurracy,
    double verticalSpeed,
    double magneticVariation,
    const QDateTime & timestamp,
    const QString & sourceName
) 
```



Creates a [**GnssPositionInformation**](classGnssPositionInformation.md) with values. The status will be set to "A"ctive by default. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/positioningutils.h`

