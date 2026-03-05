

# Class PositioningInformationModel



[**ClassList**](annotated.md) **>** [**PositioningInformationModel**](classPositioningInformationModel.md)








Inherits the following classes: QStandardItemModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**antennaHeight**](classPositioningInformationModel.md#property-antennaheight-12)  <br> |
| property QgsCoordinateReferenceSystem | [**coordinateDisplayCrs**](classPositioningInformationModel.md#property-coordinatedisplaycrs-12)  <br> |
| property Qgis::DistanceUnit | [**distanceUnits**](classPositioningInformationModel.md#property-distanceunits-12)  <br> |
| property [**Positioning**](classPositioning.md) \* | [**positioningSource**](classPositioningInformationModel.md#property-positioningsource-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**antennaHeightChanged**](classPositioningInformationModel.md#signal-antennaheightchanged)  <br> |
| signal void | [**coordinateDisplayCrsChanged**](classPositioningInformationModel.md#signal-coordinatedisplaycrschanged)  <br> |
| signal void | [**distanceUnitsChanged**](classPositioningInformationModel.md#signal-distanceunitschanged)  <br> |
| signal void | [**positioningSourceChanged**](classPositioningInformationModel.md#signal-positioningsourcechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PositioningInformationModel**](#function-positioninginformationmodel) (QObject \* parent=nullptr) <br> |
|  double | [**antennaHeight**](#function-antennaheight-22) () const<br>_Returns the antenna height._  |
|  QgsCoordinateReferenceSystem | [**coordinateDisplayCrs**](#function-coordinatedisplaycrs-22) () const<br>_Returns the coordinate display CRS._  |
|  Qgis::DistanceUnit | [**distanceUnits**](#function-distanceunits-22) () const<br>_Returns the distance units._  |
|  [**Positioning**](classPositioning.md) \* | [**positioningSource**](#function-positioningsource-22) () const<br>_Returns the positioning source._  |
|  void | [**refreshData**](#function-refreshdata) () <br>_Refreshes the data in the model._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br>_Returns the names of the roles used by the model._  |
|  void | [**setAntennaHeight**](#function-setantennaheight) (double antennaHeight) <br>_Sets the antenna height._  |
|  void | [**setCoordinateDisplayCrs**](#function-setcoordinatedisplaycrs) (const QgsCoordinateReferenceSystem & coordinateDisplayCrs) <br>_Sets the coordinate display CRS._  |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role) override<br>_Sets the data for the given index._  |
|  void | [**setDistanceUnits**](#function-setdistanceunits) (Qgis::DistanceUnit distanceUnits) <br>_Sets the distance units._  |
|  void | [**setPositioningSource**](#function-setpositioningsource) ([**Positioning**](classPositioning.md) \* positioningSource) <br>_Updates the positioning source and reconnects the positionInformationChanged signal to refreshData, while disconnecting the previous connection._  |
|  void | [**updateInfo**](#function-updateinfo) (const QString & name, const QVariant & value) <br>_Updates the information in the model with the given name and value._  |




























## Public Types Documentation




### enum Roles 

```C++
enum PositioningInformationModel::Roles {
    NameRole = Qt::UserRole,
    ValueRole = Qt::UserRole + 1
};
```




<hr>
## Public Properties Documentation




### property antennaHeight [1/2]

```C++
double PositioningInformationModel::antennaHeight;
```




<hr>



### property coordinateDisplayCrs [1/2]

```C++
QgsCoordinateReferenceSystem PositioningInformationModel::coordinateDisplayCrs;
```




<hr>



### property distanceUnits [1/2]

```C++
Qgis::DistanceUnit PositioningInformationModel::distanceUnits;
```




<hr>



### property positioningSource [1/2]

```C++
Positioning * PositioningInformationModel::positioningSource;
```




<hr>
## Public Signals Documentation




### signal antennaHeightChanged 

```C++
void PositioningInformationModel::antennaHeightChanged;
```




<hr>



### signal coordinateDisplayCrsChanged 

```C++
void PositioningInformationModel::coordinateDisplayCrsChanged;
```




<hr>



### signal distanceUnitsChanged 

```C++
void PositioningInformationModel::distanceUnitsChanged;
```




<hr>



### signal positioningSourceChanged 

```C++
void PositioningInformationModel::positioningSourceChanged;
```




<hr>
## Public Functions Documentation




### function PositioningInformationModel 

```C++
PositioningInformationModel::PositioningInformationModel (
    QObject * parent=nullptr
) 
```




<hr>



### function antennaHeight [2/2]

_Returns the antenna height._ 
```C++
double PositioningInformationModel::antennaHeight () const
```




<hr>



### function coordinateDisplayCrs [2/2]

_Returns the coordinate display CRS._ 
```C++
QgsCoordinateReferenceSystem PositioningInformationModel::coordinateDisplayCrs () const
```




<hr>



### function distanceUnits [2/2]

_Returns the distance units._ 
```C++
Qgis::DistanceUnit PositioningInformationModel::distanceUnits () const
```




<hr>



### function positioningSource [2/2]

_Returns the positioning source._ 
```C++
Positioning * PositioningInformationModel::positioningSource () const
```




<hr>



### function refreshData 

_Refreshes the data in the model._ 
```C++
void PositioningInformationModel::refreshData () 
```




<hr>



### function roleNames 

_Returns the names of the roles used by the model._ 
```C++
QHash< int, QByteArray > PositioningInformationModel::roleNames () override const
```




<hr>



### function setAntennaHeight 

_Sets the antenna height._ 
```C++
void PositioningInformationModel::setAntennaHeight (
    double antennaHeight
) 
```





**Parameters:**


* `antennaHeight` The new antenna height 




        

<hr>



### function setCoordinateDisplayCrs 

_Sets the coordinate display CRS._ 
```C++
void PositioningInformationModel::setCoordinateDisplayCrs (
    const QgsCoordinateReferenceSystem & coordinateDisplayCrs
) 
```





**Parameters:**


* `coordinateDisplayCrs` The new CRS 




        

<hr>



### function setData 

_Sets the data for the given index._ 
```C++
bool PositioningInformationModel::setData (
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
void PositioningInformationModel::setDistanceUnits (
    Qgis::DistanceUnit distanceUnits
) 
```





**Parameters:**


* `distanceUnits` The new distance units 




        

<hr>



### function setPositioningSource 

_Updates the positioning source and reconnects the positionInformationChanged signal to refreshData, while disconnecting the previous connection._ 
```C++
void PositioningInformationModel::setPositioningSource (
    Positioning * positioningSource
) 
```





**Parameters:**


* `positioningSource` The new positioning source to update with 




        

<hr>



### function updateInfo 

_Updates the information in the model with the given name and value._ 
```C++
void PositioningInformationModel::updateInfo (
    const QString & name,
    const QVariant & value
) 
```





**Parameters:**


* `name` The name of the information to update 
* `value` The new value 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/positioninginformationmodel.h`

