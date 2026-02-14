

# Class CoordinateReferenceSystemUtils



[**ClassList**](annotated.md) **>** [**CoordinateReferenceSystemUtils**](classCoordinateReferenceSystemUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CoordinateReferenceSystemUtils**](#function-coordinatereferencesystemutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE bool | [**defaultCoordinateOrderForCrsIsXY**](#function-defaultcoordinateorderforcrsisxy) (const QgsCoordinateReferenceSystem & crs) <br>_Returns whether the default coordinate order of a given_ _crs_ _is XY._ |
|  Q\_INVOKABLE QgsCoordinateTransformContext | [**emptyTransformContext**](#function-emptytransformcontext) () <br> |
|  Q\_INVOKABLE QgsCoordinateReferenceSystem | [**fromDescription**](#function-fromdescription) (const QString & definition) <br>_Returns an CRS matching the provided_ _definition_ _._ |
|  Q\_INVOKABLE QgsCoordinateReferenceSystem | [**invalidCrs**](#function-invalidcrs) () <br> |
|  Q\_INVOKABLE QgsCoordinateReferenceSystem | [**wgs84Crs**](#function-wgs84crs) () <br>_Returns an EPGS:4326 WGS84 CRS._  |


























## Public Functions Documentation




### function CoordinateReferenceSystemUtils 

```C++
explicit CoordinateReferenceSystemUtils::CoordinateReferenceSystemUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function defaultCoordinateOrderForCrsIsXY 

_Returns whether the default coordinate order of a given_ _crs_ _is XY._
```C++
static Q_INVOKABLE bool CoordinateReferenceSystemUtils::defaultCoordinateOrderForCrsIsXY (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function emptyTransformContext 

```C++
static inline Q_INVOKABLE QgsCoordinateTransformContext CoordinateReferenceSystemUtils::emptyTransformContext () 
```



Returns an empty transform context 

**Note:**

This can be used in QML to avoid errors when a parent object pointer goes undefined 





        

<hr>



### function fromDescription 

_Returns an CRS matching the provided_ _definition_ _._
```C++
static inline Q_INVOKABLE QgsCoordinateReferenceSystem CoordinateReferenceSystemUtils::fromDescription (
    const QString & definition
) 
```




<hr>



### function invalidCrs 

```C++
static inline Q_INVOKABLE QgsCoordinateReferenceSystem CoordinateReferenceSystemUtils::invalidCrs () 
```



Returns an invalid CRS 

**Note:**

This can be used in QML to avoid errors when a parent object pointer goes undefined 





        

<hr>



### function wgs84Crs 

_Returns an EPGS:4326 WGS84 CRS._ 
```C++
static inline Q_INVOKABLE QgsCoordinateReferenceSystem CoordinateReferenceSystemUtils::wgs84Crs () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/coordinatereferencesystemutils.h`

