

# Class QfPositioningInformationModel



[**ClassList**](annotated.md) **>** [**QfPositioningInformationModel**](classQfPositioningInformationModel.md)








Inherits the following classes: QStandardItemModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**antennaHeight**](classQfPositioningInformationModel.md#property-antennaheight-12)  <br> |
| property QgsCoordinateReferenceSystem | [**coordinateDisplayCrs**](classQfPositioningInformationModel.md#property-coordinatedisplaycrs-12)  <br> |
| property Qgis::DistanceUnit | [**distanceUnits**](classQfPositioningInformationModel.md#property-distanceunits-12)  <br> |
| property [**QfPositioning**](classQfPositioning.md) \* | [**positioningSource**](classQfPositioningInformationModel.md#property-positioningsource-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**antennaHeightChanged**](classQfPositioningInformationModel.md#signal-antennaheightchanged)  <br> |
| signal void | [**coordinateDisplayCrsChanged**](classQfPositioningInformationModel.md#signal-coordinatedisplaycrschanged)  <br> |
| signal void | [**distanceUnitsChanged**](classQfPositioningInformationModel.md#signal-distanceunitschanged)  <br> |
| signal void | [**positioningSourceChanged**](classQfPositioningInformationModel.md#signal-positioningsourcechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfPositioningInformationModel**](#function-qfpositioninginformationmodel) (QObject \* parent=nullptr) <br> |
|  double | [**antennaHeight**](#function-antennaheight-22) () const<br>_Returns the antenna height._  |
|  QgsCoordinateReferenceSystem | [**coordinateDisplayCrs**](#function-coordinatedisplaycrs-22) () const<br>_Returns the coordinate display CRS._  |
|  Qgis::DistanceUnit | [**distanceUnits**](#function-distanceunits-22) () const<br>_Returns the distance units._  |
|  [**QfPositioning**](classQfPositioning.md) \* | [**positioningSource**](#function-positioningsource-22) () const<br>_Returns the positioning source._  |
|  void | [**refreshData**](#function-refreshdata) () <br>_Refreshes the data in the model._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br>_Returns the names of the roles used by the model._  |
|  void | [**setAntennaHeight**](#function-setantennaheight) (double antennaHeight) <br>_Sets the antenna height._  |
|  void | [**setCoordinateDisplayCrs**](#function-setcoordinatedisplaycrs) (const QgsCoordinateReferenceSystem & coordinateDisplayCrs) <br>_Sets the coordinate display CRS._  |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role) override<br>_Sets the data for the given index._  |
|  void | [**setDistanceUnits**](#function-setdistanceunits) (Qgis::DistanceUnit distanceUnits) <br>_Sets the distance units._  |
|  void | [**setPositioningSource**](#function-setpositioningsource) ([**QfPositioning**](classQfPositioning.md) \* positioningSource) <br>_Updates the positioning source and reconnects the positionInformationChanged signal to refreshData, while disconnecting the previous connection._  |
|  void | [**updateInfo**](#function-updateinfo) (const QString & name, const QVariant & value) <br>_Updates the information in the model with the given name and value._  |




























## Public Types Documentation




### enum Roles 

```C++
enum QfPositioningInformationModel::Roles {
    NameRole = Qt::UserRole,
    ValueRole = Qt::UserRole + 1
};
```




<hr>
## Public Properties Documentation




### property antennaHeight [1/2]

```C++
double QfPositioningInformationModel::antennaHeight;
```




<hr>



### property coordinateDisplayCrs [1/2]

```C++
QgsCoordinateReferenceSystem QfPositioningInformationModel::coordinateDisplayCrs;
```




<hr>



### property distanceUnits [1/2]

```C++
Qgis::DistanceUnit QfPositioningInformationModel::distanceUnits;
```




<hr>



### property positioningSource [1/2]

```C++
QfPositioning * QfPositioningInformationModel::positioningSource;
```




<hr>
## Public Signals Documentation




### signal antennaHeightChanged 

```C++
void QfPositioningInformationModel::antennaHeightChanged;
```




<hr>



### signal coordinateDisplayCrsChanged 

```C++
void QfPositioningInformationModel::coordinateDisplayCrsChanged;
```




<hr>



### signal distanceUnitsChanged 

```C++
void QfPositioningInformationModel::distanceUnitsChanged;
```




<hr>



### signal positioningSourceChanged 

```C++
void QfPositioningInformationModel::positioningSourceChanged;
```




<hr>
## Public Functions Documentation




### function QfPositioningInformationModel 

```C++
QfPositioningInformationModel::QfPositioningInformationModel (
    QObject * parent=nullptr
) 
```




<hr>



### function antennaHeight [2/2]

_Returns the antenna height._ 
```C++
double QfPositioningInformationModel::antennaHeight () const
```




<hr>



### function coordinateDisplayCrs [2/2]

_Returns the coordinate display CRS._ 
```C++
QgsCoordinateReferenceSystem QfPositioningInformationModel::coordinateDisplayCrs () const
```




<hr>



### function distanceUnits [2/2]

_Returns the distance units._ 
```C++
Qgis::DistanceUnit QfPositioningInformationModel::distanceUnits () const
```




<hr>



### function positioningSource [2/2]

_Returns the positioning source._ 
```C++
QfPositioning * QfPositioningInformationModel::positioningSource () const
```




<hr>



### function refreshData 

_Refreshes the data in the model._ 
```C++
void QfPositioningInformationModel::refreshData () 
```




<hr>



### function roleNames 

_Returns the names of the roles used by the model._ 
```C++
QHash< int, QByteArray > QfPositioningInformationModel::roleNames () override const
```




<hr>



### function setAntennaHeight 

_Sets the antenna height._ 
```C++
void QfPositioningInformationModel::setAntennaHeight (
    double antennaHeight
) 
```





**Parameters:**


* `antennaHeight` The new antenna height 




        

<hr>



### function setCoordinateDisplayCrs 

_Sets the coordinate display CRS._ 
```C++
void QfPositioningInformationModel::setCoordinateDisplayCrs (
    const QgsCoordinateReferenceSystem & coordinateDisplayCrs
) 
```





**Parameters:**


* `coordinateDisplayCrs` The new CRS 




        

<hr>



### function setData 

_Sets the data for the given index._ 
```C++
bool QfPositioningInformationModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```





**Parameters:**


* `index` The index to set the data for 
* `value` The new value 
* `role` The role of the data 




        

<hr>



### function setDistanceUnits 

_Sets the distance units._ 
```C++
void QfPositioningInformationModel::setDistanceUnits (
    Qgis::DistanceUnit distanceUnits
) 
```





**Parameters:**


* `distanceUnits` The new distance units 




        

<hr>



### function setPositioningSource 

_Updates the positioning source and reconnects the positionInformationChanged signal to refreshData, while disconnecting the previous connection._ 
```C++
void QfPositioningInformationModel::setPositioningSource (
    QfPositioning * positioningSource
) 
```





**Parameters:**


* `positioningSource` The new positioning source to update with 




        

<hr>



### function updateInfo 

_Updates the information in the model with the given name and value._ 
```C++
void QfPositioningInformationModel::updateInfo (
    const QString & name,
    const QVariant & value
) 
```





**Parameters:**


* `name` The name of the information to update 
* `value` The new value 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfpositioninginformationmodel.h`

