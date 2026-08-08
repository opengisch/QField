

# Class ScaleBarMeasurement



[**ClassList**](annotated.md) **>** [**ScaleBarMeasurement**](classScaleBarMeasurement.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**label**](classScaleBarMeasurement.md#property-label-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classScaleBarMeasurement.md#property-mapsettings-12)  <br> |
| property QgsProject \* | [**project**](classScaleBarMeasurement.md#property-project-12)  <br> |
| property double | [**referenceScreenLength**](classScaleBarMeasurement.md#property-referencescreenlength-12)  <br> |
| property double | [**screenLength**](classScaleBarMeasurement.md#property-screenlength-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**labelChanged**](classScaleBarMeasurement.md#signal-labelchanged)  <br> |
| signal void | [**mapSettingsChanged**](classScaleBarMeasurement.md#signal-mapsettingschanged)  <br> |
| signal void | [**projectChanged**](classScaleBarMeasurement.md#signal-projectchanged)  <br> |
| signal void | [**referenceScreenLengthChanged**](classScaleBarMeasurement.md#signal-referencescreenlengthchanged)  <br> |
| signal void | [**screenLengthChanged**](classScaleBarMeasurement.md#signal-screenlengthchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ScaleBarMeasurement**](#function-scalebarmeasurement) (QObject \* parent=nullptr) <br> |
|  QString | [**label**](#function-label-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br> |
|  double | [**referenceScreenLength**](#function-referencescreenlength-22) () const<br> |
|  double | [**screenLength**](#function-screenlength-22) () const<br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br> |
|  void | [**setReferenceScreenLength**](#function-setreferencescreenlength) (double referenceScreenLength) <br> |




























## Public Properties Documentation




### property label [1/2]

```C++
QString ScaleBarMeasurement::label;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * ScaleBarMeasurement::mapSettings;
```




<hr>



### property project [1/2]

```C++
QgsProject * ScaleBarMeasurement::project;
```




<hr>



### property referenceScreenLength [1/2]

```C++
double ScaleBarMeasurement::referenceScreenLength;
```




<hr>



### property screenLength [1/2]

```C++
double ScaleBarMeasurement::screenLength;
```




<hr>
## Public Signals Documentation




### signal labelChanged 

```C++
void ScaleBarMeasurement::labelChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void ScaleBarMeasurement::mapSettingsChanged;
```




<hr>



### signal projectChanged 

```C++
void ScaleBarMeasurement::projectChanged;
```




<hr>



### signal referenceScreenLengthChanged 

```C++
void ScaleBarMeasurement::referenceScreenLengthChanged;
```




<hr>



### signal screenLengthChanged 

```C++
void ScaleBarMeasurement::screenLengthChanged;
```




<hr>
## Public Functions Documentation




### function ScaleBarMeasurement 

```C++
explicit ScaleBarMeasurement::ScaleBarMeasurement (
    QObject * parent=nullptr
) 
```




<hr>



### function label [2/2]

```C++
QString ScaleBarMeasurement::label () const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * ScaleBarMeasurement::mapSettings () const
```




<hr>



### function project [2/2]

```C++
QgsProject * ScaleBarMeasurement::project () const
```




<hr>



### function referenceScreenLength [2/2]

```C++
double ScaleBarMeasurement::referenceScreenLength () const
```




<hr>



### function screenLength [2/2]

```C++
double ScaleBarMeasurement::screenLength () const
```




<hr>



### function setMapSettings 

```C++
void ScaleBarMeasurement::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setProject 

```C++
void ScaleBarMeasurement::setProject (
    QgsProject * project
) 
```




<hr>



### function setReferenceScreenLength 

```C++
void ScaleBarMeasurement::setReferenceScreenLength (
    double referenceScreenLength
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/scalebarmeasurement.h`

