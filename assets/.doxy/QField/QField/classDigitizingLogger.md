

# Class DigitizingLogger



[**ClassList**](annotated.md) **>** [**DigitizingLogger**](classDigitizingLogger.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**CloudUserInformation**](structCloudUserInformation.md) | [**cloudUserInformation**](classDigitizingLogger.md#property-clouduserinformation-12)  <br> |
| property QgsVectorLayer \* | [**digitizingLayer**](classDigitizingLogger.md#property-digitizinglayer-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classDigitizingLogger.md#property-mapsettings-12)  <br> |
| property [**GnssPositionInformation**](classGnssPositionInformation.md) | [**positionInformation**](classDigitizingLogger.md#property-positioninformation-12)  <br> |
| property bool | [**positionLocked**](classDigitizingLogger.md#property-positionlocked-12)  <br> |
| property QgsProject \* | [**project**](classDigitizingLogger.md#property-project-12)  <br> |
| property [**SnappingResult**](classSnappingResult.md) | [**topSnappingResult**](classDigitizingLogger.md#property-topsnappingresult-12)  <br> |
| property QString | [**type**](classDigitizingLogger.md#property-type-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cloudUserInformationChanged**](classDigitizingLogger.md#signal-clouduserinformationchanged)  <br> |
| signal void | [**currentCoordinateChanged**](classDigitizingLogger.md#signal-currentcoordinatechanged)  <br> |
| signal void | [**digitizingLayerChanged**](classDigitizingLogger.md#signal-digitizinglayerchanged)  <br> |
| signal void | [**mapSettingsChanged**](classDigitizingLogger.md#signal-mapsettingschanged)  <br> |
| signal void | [**positionInformationChanged**](classDigitizingLogger.md#signal-positioninformationchanged)  <br> |
| signal void | [**positionLockedChanged**](classDigitizingLogger.md#signal-positionlockedchanged)  <br> |
| signal void | [**projectChanged**](classDigitizingLogger.md#signal-projectchanged)  <br> |
| signal void | [**topSnappingResultChanged**](classDigitizingLogger.md#signal-topsnappingresultchanged)  <br> |
| signal void | [**typeChanged**](classDigitizingLogger.md#signal-typechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DigitizingLogger**](#function-digitizinglogger) () <br> |
|  Q\_INVOKABLE void | [**addCoordinate**](#function-addcoordinate) (const QgsPoint & point) <br> |
|  Q\_INVOKABLE void | [**clearCoordinates**](#function-clearcoordinates) () <br> |
|  [**CloudUserInformation**](structCloudUserInformation.md) | [**cloudUserInformation**](#function-clouduserinformation-22) () const<br> |
|  QgsVectorLayer \* | [**digitizingLayer**](#function-digitizinglayer-22) () const<br>_Returns the current vector layer used to digitize features._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_Returns map settings._  |
|  [**GnssPositionInformation**](classGnssPositionInformation.md) | [**positionInformation**](#function-positioninformation-22) () const<br> |
|  bool | [**positionLocked**](#function-positionlocked-22) () const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_Returns the current project from which the digitizing logs will be sought._  |
|  Q\_INVOKABLE void | [**removeLastCoordinate**](#function-removelastcoordinate) () <br> |
|  void | [**setCloudUserInformation**](#function-setclouduserinformation) (const [**CloudUserInformation**](structCloudUserInformation.md) & cloudUserInformation) <br> |
|  void | [**setDigitizingLayer**](#function-setdigitizinglayer) (QgsVectorLayer \* layer) <br>_Sets the current vector_ _layer_ _used to digitze features._ |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setPositionInformation**](#function-setpositioninformation) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & positionInformation) <br> |
|  void | [**setPositionLocked**](#function-setpositionlocked) (bool positionLocked) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_Sets the_ _project_ _used to find the digitizing logs layer._ |
|  void | [**setTopSnappingResult**](#function-settopsnappingresult) (const [**SnappingResult**](classSnappingResult.md) & topSnappingResult) <br> |
|  void | [**setType**](#function-settype) (const QString & type) <br> |
|  [**SnappingResult**](classSnappingResult.md) | [**topSnappingResult**](#function-topsnappingresult-22) () const<br> |
|  QString | [**type**](#function-type-22) () const<br>_Returns the digitizing logs type._  |
|  Q\_INVOKABLE void | [**writeCoordinates**](#function-writecoordinates) () <br> |




























## Public Properties Documentation




### property cloudUserInformation [1/2]

```C++
CloudUserInformation DigitizingLogger::cloudUserInformation;
```




<hr>



### property digitizingLayer [1/2]

```C++
QgsVectorLayer * DigitizingLogger::digitizingLayer;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * DigitizingLogger::mapSettings;
```




<hr>



### property positionInformation [1/2]

```C++
GnssPositionInformation DigitizingLogger::positionInformation;
```




<hr>



### property positionLocked [1/2]

```C++
bool DigitizingLogger::positionLocked;
```




<hr>



### property project [1/2]

```C++
QgsProject * DigitizingLogger::project;
```




<hr>



### property topSnappingResult [1/2]

```C++
SnappingResult DigitizingLogger::topSnappingResult;
```




<hr>



### property type [1/2]

```C++
QString DigitizingLogger::type;
```




<hr>
## Public Signals Documentation




### signal cloudUserInformationChanged 

```C++
void DigitizingLogger::cloudUserInformationChanged;
```




<hr>



### signal currentCoordinateChanged 

```C++
void DigitizingLogger::currentCoordinateChanged;
```




<hr>



### signal digitizingLayerChanged 

```C++
void DigitizingLogger::digitizingLayerChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void DigitizingLogger::mapSettingsChanged;
```




<hr>



### signal positionInformationChanged 

```C++
void DigitizingLogger::positionInformationChanged;
```




<hr>



### signal positionLockedChanged 

```C++
void DigitizingLogger::positionLockedChanged;
```




<hr>



### signal projectChanged 

```C++
void DigitizingLogger::projectChanged;
```




<hr>



### signal topSnappingResultChanged 

```C++
void DigitizingLogger::topSnappingResultChanged;
```




<hr>



### signal typeChanged 

```C++
void DigitizingLogger::typeChanged;
```




<hr>
## Public Functions Documentation




### function DigitizingLogger 

```C++
explicit DigitizingLogger::DigitizingLogger () 
```




<hr>



### function addCoordinate 

```C++
Q_INVOKABLE void DigitizingLogger::addCoordinate (
    const QgsPoint & point
) 
```



Adds a _point_ into the digitizing logs' buffer. 


        

<hr>



### function clearCoordinates 

```C++
Q_INVOKABLE void DigitizingLogger::clearCoordinates () 
```



Clear the points buffer from the digitizing logs. 


        

<hr>



### function cloudUserInformation [2/2]

```C++
inline CloudUserInformation DigitizingLogger::cloudUserInformation () const
```



Returns the current cloud user information 


        

<hr>



### function digitizingLayer [2/2]

_Returns the current vector layer used to digitize features._ 
```C++
inline QgsVectorLayer * DigitizingLogger::digitizingLayer () const
```




<hr>



### function mapSettings [2/2]

_Returns map settings._ 
```C++
inline QgsQuickMapSettings * DigitizingLogger::mapSettings () const
```




<hr>



### function positionInformation [2/2]

```C++
inline GnssPositionInformation DigitizingLogger::positionInformation () const
```



Returns position information generated by the TransformedPositionSource according to its provider 


        

<hr>



### function positionLocked [2/2]

```C++
inline bool DigitizingLogger::positionLocked () const
```



Returns whether the position is locked to the GNSS 


        

<hr>



### function project [2/2]

_Returns the current project from which the digitizing logs will be sought._ 
```C++
inline QgsProject * DigitizingLogger::project () const
```




<hr>



### function removeLastCoordinate 

```C++
Q_INVOKABLE void DigitizingLogger::removeLastCoordinate () 
```



Removes the last point entered into the digitizing logs' buffer. 


        

<hr>



### function setCloudUserInformation 

```C++
void DigitizingLogger::setCloudUserInformation (
    const CloudUserInformation & cloudUserInformation
) 
```



Sets the current cloud user information 

**Parameters:**


* `cloudUserInformation` the cloud user information 




        

<hr>



### function setDigitizingLayer 

_Sets the current vector_ _layer_ _used to digitze features._
```C++
void DigitizingLogger::setDigitizingLayer (
    QgsVectorLayer * layer
) 
```




<hr>



### function setMapSettings 

```C++
void DigitizingLogger::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```



Sets map settings 

**Parameters:**


* `mapSettings` the [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) object 




        

<hr>



### function setPositionInformation 

```C++
void DigitizingLogger::setPositionInformation (
    const GnssPositionInformation & positionInformation
) 
```



Sets position information generated by the TransformedPositionSource according to its provider 

**Parameters:**


* `positionInformation` the position information 




        

<hr>



### function setPositionLocked 

```C++
void DigitizingLogger::setPositionLocked (
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
void DigitizingLogger::setProject (
    QgsProject * project
) 
```




<hr>



### function setTopSnappingResult 

```C++
void DigitizingLogger::setTopSnappingResult (
    const SnappingResult & topSnappingResult
) 
```



Sets the top snapping result of the coordinate locator 

**Parameters:**


* `topSnappingResult` the top snapping result object 




        

<hr>



### function setType 

```C++
void DigitizingLogger::setType (
    const QString & type
) 
```




<hr>



### function topSnappingResult [2/2]

```C++
inline SnappingResult DigitizingLogger::topSnappingResult () const
```



Returns the top snapping result of the coordinate locator 


        

<hr>



### function type [2/2]

_Returns the digitizing logs type._ 
```C++
inline QString DigitizingLogger::type () const
```




<hr>



### function writeCoordinates 

```C++
Q_INVOKABLE void DigitizingLogger::writeCoordinates () 
```



Writes the points buffer to the digitizing logs layer. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/digitizinglogger.h`

