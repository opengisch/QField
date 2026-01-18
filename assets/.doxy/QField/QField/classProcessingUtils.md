

# Class ProcessingUtils



[**ClassList**](annotated.md) **>** [**ProcessingUtils**](classProcessingUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ProcessingUtils**](#function-processingutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE double | [**fromAreaUnitToAreaUnitFactor**](#function-fromareaunittoareaunitfactor) (Qgis::AreaUnit fromUnit, Qgis::AreaUnit toUnit) <br> |


























## Public Functions Documentation




### function ProcessingUtils 

```C++
explicit ProcessingUtils::ProcessingUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function fromAreaUnitToAreaUnitFactor 

```C++
static Q_INVOKABLE double ProcessingUtils::fromAreaUnitToAreaUnitFactor (
    Qgis::AreaUnit fromUnit,
    Qgis::AreaUnit toUnit
) 
```



Returns the conversion factor between the specified areal units. 

**Parameters:**


* `fromUnit` area unit to convert from 
* `toUnit` area unit to convert to 



**Returns:**

multiplication factor to convert between units 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/processingutils.h`

