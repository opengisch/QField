

# Class QfPositioningUtils



[**ClassList**](annotated.md) **>** [**QfPositioningUtils**](classQfPositioningUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfPositioningUtils**](#function-qfpositioningutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**averagedPositionInformation**](#function-averagedpositioninformation-12) (const QList&lt; [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) &gt; & positionsInformation) <br> |
|  Q\_INVOKABLE [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**averagedPositionInformation**](#function-averagedpositioninformation-22) (const QList&lt; QVariant &gt; & positionsInformation) <br> |
|  Q\_INVOKABLE double | [**bearingTrueNorth**](#function-bearingtruenorth) (const QgsPoint & position, const QgsCoordinateReferenceSystem & crs) <br> |
|  Q\_INVOKABLE [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**createEmptyGnssPositionInformation**](#function-createemptygnsspositioninformation) () <br> |
|  Q\_INVOKABLE QgsRectangle | [**createExtentForDevice**](#function-createextentfordevice) (const [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & positionInformation, const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem::fromEpsgId(4326), const QgsRectangle & withinRectangle=QgsRectangle()) <br> |
|  Q\_INVOKABLE [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**createGnssPositionInformation**](#function-creategnsspositioninformation) (double latitude, double longitude, double altitude, double speed, double direction, double horizontalAccuracy, double verticalAcurracy, double verticalSpeed, double magneticVariation, const QDateTime & timestamp, const QString & sourceName) <br> |
|  Q\_INVOKABLE [**QfNtripSettings**](classQfNtripSettings.md) | [**createNtripSettings**](#function-createntripsettings) (const QVariantMap & settings) <br> |


























## Public Functions Documentation




### function QfPositioningUtils 

```C++
explicit QfPositioningUtils::QfPositioningUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function averagedPositionInformation [1/2]

```C++
static Q_INVOKABLE QfGnssPositionInformation QfPositioningUtils::averagedPositionInformation (
    const QList< QfGnssPositionInformation > & positionsInformation
) 
```



Returns an average [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) from a list of position information. 


        

<hr>



### function averagedPositionInformation [2/2]

```C++
static Q_INVOKABLE QfGnssPositionInformation QfPositioningUtils::averagedPositionInformation (
    const QList< QVariant > & positionsInformation
) 
```



Returns an average [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) from a list of position information 


        

<hr>



### function bearingTrueNorth 

```C++
static Q_INVOKABLE double QfPositioningUtils::bearingTrueNorth (
    const QgsPoint & position,
    const QgsCoordinateReferenceSystem & crs
) 
```



Returns the true north bearing from a given position and CRS 


        

<hr>



### function createEmptyGnssPositionInformation 

```C++
static Q_INVOKABLE QfGnssPositionInformation QfPositioningUtils::createEmptyGnssPositionInformation () 
```



Creates an empty [**QfGnssPositionInformation**](classQfGnssPositionInformation.md). 


        

<hr>



### function createExtentForDevice 

```C++
static Q_INVOKABLE QgsRectangle QfPositioningUtils::createExtentForDevice (
    const QfGnssPositionInformation & positionInformation,
    const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem::fromEpsgId(4326),
    const QgsRectangle & withinRectangle=QgsRectangle()
) 
```



Returns a reasonable default extent derived from a device's timezone and positioning if available. The extent can optionally be clipped to a QgsRectangle. 


        

<hr>



### function createGnssPositionInformation 

```C++
static Q_INVOKABLE QfGnssPositionInformation QfPositioningUtils::createGnssPositionInformation (
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



Creates a [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) with values. The status will be set to "A"ctive by default. 


        

<hr>



### function createNtripSettings 

```C++
static Q_INVOKABLE QfNtripSettings QfPositioningUtils::createNtripSettings (
    const QVariantMap & settings
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfpositioningutils.h`

