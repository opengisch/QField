

# Class QgsQuickCoordinateTransformer



[**ClassList**](annotated.md) **>** [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md)



[More...](#detailed-description)

* `#include <qgsquickcoordinatetransformer.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property qreal | [**deltaZ**](classQgsQuickCoordinateTransformer.md#property-deltaz-12)  <br> |
| property QgsCoordinateReferenceSystem | [**destinationCrs**](classQgsQuickCoordinateTransformer.md#property-destinationcrs-12)  <br>_Destination CRS._  |
| property QgsPoint | [**projectedPosition**](classQgsQuickCoordinateTransformer.md#property-projectedposition-12)  <br>_Projected (destination) position (in destination CRS)_  |
| property bool | [**skipAltitudeTransformation**](classQgsQuickCoordinateTransformer.md#property-skipaltitudetransformation-12)  <br> |
| property QGeoCoordinate | [**sourceCoordinate**](classQgsQuickCoordinateTransformer.md#property-sourcecoordinate-12)  <br>_Source coordinate for integrating with QtPositioning, alternative to source position._  |
| property QgsCoordinateReferenceSystem | [**sourceCrs**](classQgsQuickCoordinateTransformer.md#property-sourcecrs-12)  <br>_Source CRS, default 4326._  |
| property QgsPoint | [**sourcePosition**](classQgsQuickCoordinateTransformer.md#property-sourceposition-12)  <br>_Source position (in source CRS)_  |
| property QgsCoordinateTransformContext | [**transformContext**](classQgsQuickCoordinateTransformer.md#property-transformcontext-12)  <br>_Transformation context, can be set from_ [_**QgsQuickMapSettings::transformContext()**_](classQgsQuickMapSettings.md#function-transformcontext) __ |
| property QString | [**verticalGrid**](classQgsQuickCoordinateTransformer.md#property-verticalgrid-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**deltaZChanged**](classQgsQuickCoordinateTransformer.md#signal-deltazchanged)  <br> |
| signal void | [**destinationCrsChanged**](classQgsQuickCoordinateTransformer.md#signal-destinationcrschanged)  <br>_Destination CRS._  |
| signal void | [**projectedPositionChanged**](classQgsQuickCoordinateTransformer.md#signal-projectedpositionchanged)  <br>_Projected (destination) position (in destination CRS)_  |
| signal void | [**skipAltitudeTransformationChanged**](classQgsQuickCoordinateTransformer.md#signal-skipaltitudetransformationchanged)  <br> |
| signal void | [**sourceCoordinateChanged**](classQgsQuickCoordinateTransformer.md#signal-sourcecoordinatechanged)  <br> |
| signal void | [**sourceCrsChanged**](classQgsQuickCoordinateTransformer.md#signal-sourcecrschanged)  <br>_Source CRS, default 4326._  |
| signal void | [**sourcePositionChanged**](classQgsQuickCoordinateTransformer.md#signal-sourcepositionchanged)  <br>_Source position (in source CRS)_  |
| signal void | [**transformContextChanged**](classQgsQuickCoordinateTransformer.md#signal-transformcontextchanged)  <br>_Transformation context, can be set from_ [_**QgsQuickMapSettings::transformContext()**_](classQgsQuickMapSettings.md#function-transformcontext) __ |
| signal void | [**verticalGridChanged**](classQgsQuickCoordinateTransformer.md#signal-verticalgridchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QgsQuickCoordinateTransformer**](#function-qgsquickcoordinatetransformer) (QObject \* parent=nullptr) <br>_Creates new coordinate transformer._  |
|  qreal | [**deltaZ**](#function-deltaz-22) () const<br> |
|  QgsCoordinateReferenceSystem | [**destinationCrs**](#function-destinationcrs-22) () const<br>_Destination CRS._  |
|  QgsPoint | [**projectedPosition**](#function-projectedposition-22) () const<br>_Projected (destination) position (in destination CRS)_  |
|  void | [**setDeltaZ**](#function-setdeltaz) (const qreal & deltaZ) <br> |
|  void | [**setDestinationCrs**](#function-setdestinationcrs) (const QgsCoordinateReferenceSystem & destinationCrs) <br>_Destination CRS._  |
|  void | [**setSkipAltitudeTransformation**](#function-setskipaltitudetransformation) (bool skipAltitudeTransformation) <br> |
|  void | [**setSourceCoordinate**](#function-setsourcecoordinate) (const QGeoCoordinate & sourceCoordinate) <br>_Source coordinate for integrating with QtPositioning, alternative to source position._  |
|  void | [**setSourceCrs**](#function-setsourcecrs) (const QgsCoordinateReferenceSystem & sourceCrs) <br>_Source CRS, default 4326._  |
|  void | [**setSourcePosition**](#function-setsourceposition) (const QgsPoint & sourcePosition) <br>_Source position (in source CRS)_  |
|  void | [**setTransformContext**](#function-settransformcontext) (const QgsCoordinateTransformContext & context) <br>_Transformation context, can be set from_ [_**QgsQuickMapSettings::transformContext()**_](classQgsQuickMapSettings.md#function-transformcontext) __ |
|  void | [**setVerticalGrid**](#function-setverticalgrid) (const QString & grid) <br> |
|  bool | [**skipAltitudeTransformation**](#function-skipaltitudetransformation-22) () const<br> |
|  QGeoCoordinate | [**sourceCoordinate**](#function-sourcecoordinate-22) () const<br>_Source coordinate for integrating with QtPositioning, alternative to source position._  |
|  QgsCoordinateReferenceSystem | [**sourceCrs**](#function-sourcecrs-22) () const<br>_Source CRS, default 4326._  |
|  QgsPoint | [**sourcePosition**](#function-sourceposition-22) () const<br>_Source position (in source CRS)_  |
|  QgsCoordinateTransformContext | [**transformContext**](#function-transformcontext-22) () const<br>_Transformation context, can be set from_ [_**QgsQuickMapSettings::transformContext()**_](classQgsQuickMapSettings.md#function-transformcontext) __ |
|  Q\_INVOKABLE QgsPoint | [**transformPosition**](#function-transformposition) (const QgsPoint & position) const<br> |
|  QString | [**verticalGrid**](#function-verticalgrid-22) () const<br> |




























## Detailed Description


Helper class for transform of coordinates (QgsPoint) to a different coordinate reference system.


It requires connection of transformation context from mapSettings, source position and source CRS to calculate projected position in desired destination CRS.




**Note:**

QML Type: CoordinateTransformer 





    
## Public Properties Documentation




### property deltaZ [1/2]

```C++
qreal QgsQuickCoordinateTransformer::deltaZ;
```



The altitude value of captured coordinates is corrected by the amount of deltaZ. This can be used to correct the altitude with the antenna height for example. 


        

<hr>



### property destinationCrs [1/2]

_Destination CRS._ 
```C++
QgsCoordinateReferenceSystem QgsQuickCoordinateTransformer::destinationCrs;
```




<hr>



### property projectedPosition [1/2]

_Projected (destination) position (in destination CRS)_ 
```C++
QgsPoint QgsQuickCoordinateTransformer::projectedPosition;
```




<hr>



### property skipAltitudeTransformation [1/2]

```C++
bool QgsQuickCoordinateTransformer::skipAltitudeTransformation;
```



Skips any altitude correction handling during CRS transformation. deltaZ will still be applied. 


        

<hr>



### property sourceCoordinate [1/2]

_Source coordinate for integrating with QtPositioning, alternative to source position._ 
```C++
QGeoCoordinate QgsQuickCoordinateTransformer::sourceCoordinate;
```




<hr>



### property sourceCrs [1/2]

_Source CRS, default 4326._ 
```C++
QgsCoordinateReferenceSystem QgsQuickCoordinateTransformer::sourceCrs;
```




<hr>



### property sourcePosition [1/2]

_Source position (in source CRS)_ 
```C++
QgsPoint QgsQuickCoordinateTransformer::sourcePosition;
```




<hr>



### property transformContext [1/2]

_Transformation context, can be set from_ [_**QgsQuickMapSettings::transformContext()**_](classQgsQuickMapSettings.md#function-transformcontext) __
```C++
QgsCoordinateTransformContext QgsQuickCoordinateTransformer::transformContext;
```




<hr>



### property verticalGrid [1/2]

```C++
QString QgsQuickCoordinateTransformer::verticalGrid;
```



The vertical grid that will be used to correct the altitude value of captured coordinates 


        

<hr>
## Public Signals Documentation




### signal deltaZChanged 

```C++
void QgsQuickCoordinateTransformer::deltaZChanged;
```




<hr>



### signal destinationCrsChanged 

_Destination CRS._ 
```C++
void QgsQuickCoordinateTransformer::destinationCrsChanged;
```




<hr>



### signal projectedPositionChanged 

_Projected (destination) position (in destination CRS)_ 
```C++
void QgsQuickCoordinateTransformer::projectedPositionChanged;
```




<hr>



### signal skipAltitudeTransformationChanged 

```C++
void QgsQuickCoordinateTransformer::skipAltitudeTransformationChanged;
```



Skips any altitude correction handling during CRS transformation. deltaZ will still be applied.   


        

<hr>



### signal sourceCoordinateChanged 

```C++
void QgsQuickCoordinateTransformer::sourceCoordinateChanged;
```




<hr>



### signal sourceCrsChanged 

_Source CRS, default 4326._ 
```C++
void QgsQuickCoordinateTransformer::sourceCrsChanged;
```




<hr>



### signal sourcePositionChanged 

_Source position (in source CRS)_ 
```C++
void QgsQuickCoordinateTransformer::sourcePositionChanged;
```




<hr>



### signal transformContextChanged 

_Transformation context, can be set from_ [_**QgsQuickMapSettings::transformContext()**_](classQgsQuickMapSettings.md#function-transformcontext) __
```C++
void QgsQuickCoordinateTransformer::transformContextChanged;
```




<hr>



### signal verticalGridChanged 

```C++
void QgsQuickCoordinateTransformer::verticalGridChanged;
```



The vertical grid that will be used to correct the altitude value of captured coordinates   


        

<hr>
## Public Functions Documentation




### function QgsQuickCoordinateTransformer 

_Creates new coordinate transformer._ 
```C++
explicit QgsQuickCoordinateTransformer::QgsQuickCoordinateTransformer (
    QObject * parent=nullptr
) 
```




<hr>



### function deltaZ [2/2]

```C++
qreal QgsQuickCoordinateTransformer::deltaZ () const
```



The altitude value of captured coordinates is corrected by the amount of deltaZ. This can be used to correct the altitude with the antenna height for example.   


        

<hr>



### function destinationCrs [2/2]

_Destination CRS._ 
```C++
QgsCoordinateReferenceSystem QgsQuickCoordinateTransformer::destinationCrs () const
```




<hr>



### function projectedPosition [2/2]

_Projected (destination) position (in destination CRS)_ 
```C++
QgsPoint QgsQuickCoordinateTransformer::projectedPosition () const
```




<hr>



### function setDeltaZ 

```C++
void QgsQuickCoordinateTransformer::setDeltaZ (
    const qreal & deltaZ
) 
```



The altitude value of captured coordinates is corrected by the amount of deltaZ. This can be used to correct the altitude with the antenna height for example.   


        

<hr>



### function setDestinationCrs 

_Destination CRS._ 
```C++
void QgsQuickCoordinateTransformer::setDestinationCrs (
    const QgsCoordinateReferenceSystem & destinationCrs
) 
```




<hr>



### function setSkipAltitudeTransformation 

```C++
void QgsQuickCoordinateTransformer::setSkipAltitudeTransformation (
    bool skipAltitudeTransformation
) 
```



Skips any altitude correction handling during CRS transformation. deltaZ will still be applied.   


        

<hr>



### function setSourceCoordinate 

_Source coordinate for integrating with QtPositioning, alternative to source position._ 
```C++
void QgsQuickCoordinateTransformer::setSourceCoordinate (
    const QGeoCoordinate & sourceCoordinate
) 
```




<hr>



### function setSourceCrs 

_Source CRS, default 4326._ 
```C++
void QgsQuickCoordinateTransformer::setSourceCrs (
    const QgsCoordinateReferenceSystem & sourceCrs
) 
```




<hr>



### function setSourcePosition 

_Source position (in source CRS)_ 
```C++
void QgsQuickCoordinateTransformer::setSourcePosition (
    const QgsPoint & sourcePosition
) 
```




<hr>



### function setTransformContext 

_Transformation context, can be set from_ [_**QgsQuickMapSettings::transformContext()**_](classQgsQuickMapSettings.md#function-transformcontext) __
```C++
void QgsQuickCoordinateTransformer::setTransformContext (
    const QgsCoordinateTransformContext & context
) 
```




<hr>



### function setVerticalGrid 

```C++
void QgsQuickCoordinateTransformer::setVerticalGrid (
    const QString & grid
) 
```



The vertical grid that will be used to correct the altitude value of captured coordinates   


        

<hr>



### function skipAltitudeTransformation [2/2]

```C++
bool QgsQuickCoordinateTransformer::skipAltitudeTransformation () const
```



Skips any altitude correction handling during CRS transformation. deltaZ will still be applied.   


        

<hr>



### function sourceCoordinate [2/2]

_Source coordinate for integrating with QtPositioning, alternative to source position._ 
```C++
QGeoCoordinate QgsQuickCoordinateTransformer::sourceCoordinate () const
```




<hr>



### function sourceCrs [2/2]

_Source CRS, default 4326._ 
```C++
QgsCoordinateReferenceSystem QgsQuickCoordinateTransformer::sourceCrs () const
```




<hr>



### function sourcePosition [2/2]

_Source position (in source CRS)_ 
```C++
QgsPoint QgsQuickCoordinateTransformer::sourcePosition () const
```




<hr>



### function transformContext [2/2]

_Transformation context, can be set from_ [_**QgsQuickMapSettings::transformContext()**_](classQgsQuickMapSettings.md#function-transformcontext) __
```C++
QgsCoordinateTransformContext QgsQuickCoordinateTransformer::transformContext () const
```




<hr>



### function transformPosition 

```C++
Q_INVOKABLE QgsPoint QgsQuickCoordinateTransformer::transformPosition (
    const QgsPoint & position
) const
```




<hr>



### function verticalGrid [2/2]

```C++
QString QgsQuickCoordinateTransformer::verticalGrid () const
```



The vertical grid that will be used to correct the altitude value of captured coordinates   


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsquick/qgsquickcoordinatetransformer.h`

