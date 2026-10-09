

# Class QfDigitizingLogger



[**ClassList**](annotated.md) **>** [**QfDigitizingLogger**](classQfDigitizingLogger.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfCloudUserInformation**](structQfCloudUserInformation.md) | [**cloudUserInformation**](classQfDigitizingLogger.md#property-clouduserinformation-12)  <br> |
| property QgsVectorLayer \* | [**digitizingLayer**](classQfDigitizingLogger.md#property-digitizinglayer-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfDigitizingLogger.md#property-mapsettings-12)  <br> |
| property [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**positionInformation**](classQfDigitizingLogger.md#property-positioninformation-12)  <br> |
| property bool | [**positionLocked**](classQfDigitizingLogger.md#property-positionlocked-12)  <br> |
| property QgsProject \* | [**project**](classQfDigitizingLogger.md#property-project-12)  <br> |
| property [**QfSnappingResult**](classQfSnappingResult.md) | [**topSnappingResult**](classQfDigitizingLogger.md#property-topsnappingresult-12)  <br> |
| property QString | [**type**](classQfDigitizingLogger.md#property-type-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cloudUserInformationChanged**](classQfDigitizingLogger.md#signal-clouduserinformationchanged)  <br> |
| signal void | [**currentCoordinateChanged**](classQfDigitizingLogger.md#signal-currentcoordinatechanged)  <br> |
| signal void | [**digitizingLayerChanged**](classQfDigitizingLogger.md#signal-digitizinglayerchanged)  <br> |
| signal void | [**mapSettingsChanged**](classQfDigitizingLogger.md#signal-mapsettingschanged)  <br> |
| signal void | [**positionInformationChanged**](classQfDigitizingLogger.md#signal-positioninformationchanged)  <br> |
| signal void | [**positionLockedChanged**](classQfDigitizingLogger.md#signal-positionlockedchanged)  <br> |
| signal void | [**projectChanged**](classQfDigitizingLogger.md#signal-projectchanged)  <br> |
| signal void | [**topSnappingResultChanged**](classQfDigitizingLogger.md#signal-topsnappingresultchanged)  <br> |
| signal void | [**typeChanged**](classQfDigitizingLogger.md#signal-typechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDigitizingLogger**](#function-qfdigitizinglogger) () <br> |
|  Q\_INVOKABLE void | [**addCoordinate**](#function-addcoordinate) (const QgsPoint & point) <br> |
|  Q\_INVOKABLE void | [**clearCoordinates**](#function-clearcoordinates) () <br> |
|  [**QfCloudUserInformation**](structQfCloudUserInformation.md) | [**cloudUserInformation**](#function-clouduserinformation-22) () const<br> |
|  QgsVectorLayer \* | [**digitizingLayer**](#function-digitizinglayer-22) () const<br>_Returns the current vector layer used to digitize features._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_Returns map settings._  |
|  [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**positionInformation**](#function-positioninformation-22) () const<br> |
|  bool | [**positionLocked**](#function-positionlocked-22) () const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_Returns the current project from which the digitizing logs will be sought._  |
|  Q\_INVOKABLE void | [**removeLastCoordinate**](#function-removelastcoordinate) () <br> |
|  void | [**setCloudUserInformation**](#function-setclouduserinformation) (const [**QfCloudUserInformation**](structQfCloudUserInformation.md) & cloudUserInformation) <br> |
|  void | [**setDigitizingLayer**](#function-setdigitizinglayer) (QgsVectorLayer \* layer) <br>_Sets the current vector_ _layer_ _used to digitze features._ |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setPositionInformation**](#function-setpositioninformation) (const [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & positionInformation) <br> |
|  void | [**setPositionLocked**](#function-setpositionlocked) (bool positionLocked) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_Sets the_ _project_ _used to find the digitizing logs layer._ |
|  void | [**setTopSnappingResult**](#function-settopsnappingresult) (const [**QfSnappingResult**](classQfSnappingResult.md) & topSnappingResult) <br> |
|  void | [**setType**](#function-settype) (const QString & type) <br> |
|  [**QfSnappingResult**](classQfSnappingResult.md) | [**topSnappingResult**](#function-topsnappingresult-22) () const<br> |
|  QString | [**type**](#function-type-22) () const<br>_Returns the digitizing logs type._  |
|  Q\_INVOKABLE void | [**writeCoordinates**](#function-writecoordinates) () <br> |




























## Public Properties Documentation




### property cloudUserInformation [1/2]

```C++
QfCloudUserInformation QfDigitizingLogger::cloudUserInformation;
```




<hr>



### property digitizingLayer [1/2]

```C++
QgsVectorLayer * QfDigitizingLogger::digitizingLayer;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfDigitizingLogger::mapSettings;
```




<hr>



### property positionInformation [1/2]

```C++
QfGnssPositionInformation QfDigitizingLogger::positionInformation;
```




<hr>



### property positionLocked [1/2]

```C++
bool QfDigitizingLogger::positionLocked;
```




<hr>



### property project [1/2]

```C++
QgsProject * QfDigitizingLogger::project;
```




<hr>



### property topSnappingResult [1/2]

```C++
QfSnappingResult QfDigitizingLogger::topSnappingResult;
```




<hr>



### property type [1/2]

```C++
QString QfDigitizingLogger::type;
```




<hr>
## Public Signals Documentation




### signal cloudUserInformationChanged 

```C++
void QfDigitizingLogger::cloudUserInformationChanged;
```




<hr>



### signal currentCoordinateChanged 

```C++
void QfDigitizingLogger::currentCoordinateChanged;
```




<hr>



### signal digitizingLayerChanged 

```C++
void QfDigitizingLogger::digitizingLayerChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfDigitizingLogger::mapSettingsChanged;
```




<hr>



### signal positionInformationChanged 

```C++
void QfDigitizingLogger::positionInformationChanged;
```




<hr>



### signal positionLockedChanged 

```C++
void QfDigitizingLogger::positionLockedChanged;
```




<hr>



### signal projectChanged 

```C++
void QfDigitizingLogger::projectChanged;
```




<hr>



### signal topSnappingResultChanged 

```C++
void QfDigitizingLogger::topSnappingResultChanged;
```




<hr>



### signal typeChanged 

```C++
void QfDigitizingLogger::typeChanged;
```




<hr>
## Public Functions Documentation




### function QfDigitizingLogger 

```C++
explicit QfDigitizingLogger::QfDigitizingLogger () 
```




<hr>



### function addCoordinate 

```C++
Q_INVOKABLE void QfDigitizingLogger::addCoordinate (
    const QgsPoint & point
) 
```



Adds a _point_ into the digitizing logs' buffer. 


        

<hr>



### function clearCoordinates 

```C++
Q_INVOKABLE void QfDigitizingLogger::clearCoordinates () 
```



Clear the points buffer from the digitizing logs. 


        

<hr>



### function cloudUserInformation [2/2]

```C++
inline QfCloudUserInformation QfDigitizingLogger::cloudUserInformation () const
```



Returns the current cloud user information 


        

<hr>



### function digitizingLayer [2/2]

_Returns the current vector layer used to digitize features._ 
```C++
inline QgsVectorLayer * QfDigitizingLogger::digitizingLayer () const
```




<hr>



### function mapSettings [2/2]

_Returns map settings._ 
```C++
inline QgsQuickMapSettings * QfDigitizingLogger::mapSettings () const
```




<hr>



### function positionInformation [2/2]

```C++
inline QfGnssPositionInformation QfDigitizingLogger::positionInformation () const
```



Returns position information generated by the TransformedPositionSource according to its provider 


        

<hr>



### function positionLocked [2/2]

```C++
inline bool QfDigitizingLogger::positionLocked () const
```



Returns whether the position is locked to the GNSS 


        

<hr>



### function project [2/2]

_Returns the current project from which the digitizing logs will be sought._ 
```C++
inline QgsProject * QfDigitizingLogger::project () const
```




<hr>



### function removeLastCoordinate 

```C++
Q_INVOKABLE void QfDigitizingLogger::removeLastCoordinate () 
```



Removes the last point entered into the digitizing logs' buffer. 


        

<hr>



### function setCloudUserInformation 

```C++
void QfDigitizingLogger::setCloudUserInformation (
    const QfCloudUserInformation & cloudUserInformation
) 
```



Sets the current cloud user information 

**Parameters:**


* `cloudUserInformation` the cloud user information 




        

<hr>



### function setDigitizingLayer 

_Sets the current vector_ _layer_ _used to digitze features._
```C++
void QfDigitizingLogger::setDigitizingLayer (
    QgsVectorLayer * layer
) 
```




<hr>



### function setMapSettings 

```C++
void QfDigitizingLogger::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```



Sets map settings 

**Parameters:**


* `mapSettings` the [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) object 




        

<hr>



### function setPositionInformation 

```C++
void QfDigitizingLogger::setPositionInformation (
    const QfGnssPositionInformation & positionInformation
) 
```



Sets position information generated by the TransformedPositionSource according to its provider 

**Parameters:**


* `positionInformation` the position information 




        

<hr>



### function setPositionLocked 

```C++
void QfDigitizingLogger::setPositionLocked (
    bool positionLocked
) 
```



Sets whether the position is locked to the GNSS 

**Parameters:**


* `positionLocked` set to TRUE if the position is locked 




        

<hr>



### function setProject 

_Sets the_ _project_ _used to find the digitizing logs layer._
```C++
void QfDigitizingLogger::setProject (
    QgsProject * project
) 
```




<hr>



### function setTopSnappingResult 

```C++
void QfDigitizingLogger::setTopSnappingResult (
    const QfSnappingResult & topSnappingResult
) 
```



Sets the top snapping result of the coordinate locator 

**Parameters:**


* `topSnappingResult` the top snapping result object 




        

<hr>



### function setType 

```C++
void QfDigitizingLogger::setType (
    const QString & type
) 
```




<hr>



### function topSnappingResult [2/2]

```C++
inline QfSnappingResult QfDigitizingLogger::topSnappingResult () const
```



Returns the top snapping result of the coordinate locator 


        

<hr>



### function type [2/2]

_Returns the digitizing logs type._ 
```C++
inline QString QfDigitizingLogger::type () const
```




<hr>



### function writeCoordinates 

```C++
Q_INVOKABLE void QfDigitizingLogger::writeCoordinates () 
```



Writes the points buffer to the digitizing logs layer. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfdigitizinglogger.h`

