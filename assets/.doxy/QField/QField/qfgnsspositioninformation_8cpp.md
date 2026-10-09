

# File qfgnsspositioninformation.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfgnsspositioninformation.cpp**](qfgnsspositioninformation_8cpp.md)

[Go to the source code of this file](qfgnsspositioninformation_8cpp_source.md)



* `#include "qfgnsspositioninformation.h"`
* `#include <QCoreApplication>`
* `#include <QFileInfo>`
* `#include <QIODevice>`
* `#include <QStringList>`
* `#include <QTime>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  QDataStream & | [**operator&lt;&lt;**](#function-operator) (QDataStream & stream, const [**QfGnssPositionDetails**](classQfGnssPositionDetails.md) & positionDetails) <br> |
|  QDataStream & | [**operator&lt;&lt;**](#function-operator_1) (QDataStream & stream, const [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & position) <br> |
|  QDataStream & | [**operator&lt;&lt;**](#function-operator_2) (QDataStream & stream, const QgsSatelliteInfo & satelliteInfo) <br> |
|  QDataStream & | [**operator&gt;&gt;**](#function-operator_3) (QDataStream & stream, [**QfGnssPositionDetails**](classQfGnssPositionDetails.md) & positionDetails) <br> |
|  QDataStream & | [**operator&gt;&gt;**](#function-operator_4) (QDataStream & stream, [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & position) <br> |
|  QDataStream & | [**operator&gt;&gt;**](#function-operator_5) (QDataStream & stream, QgsSatelliteInfo & satelliteInfo) <br> |




























## Public Functions Documentation




### function operator&lt;&lt; 

```C++
QDataStream & operator<< (
    QDataStream & stream,
    const QfGnssPositionDetails & positionDetails
) 
```




<hr>



### function operator&lt;&lt; 

```C++
QDataStream & operator<< (
    QDataStream & stream,
    const QfGnssPositionInformation & position
) 
```




<hr>



### function operator&lt;&lt; 

```C++
QDataStream & operator<< (
    QDataStream & stream,
    const QgsSatelliteInfo & satelliteInfo
) 
```




<hr>



### function operator&gt;&gt; 

```C++
QDataStream & operator>> (
    QDataStream & stream,
    QfGnssPositionDetails & positionDetails
) 
```




<hr>



### function operator&gt;&gt; 

```C++
QDataStream & operator>> (
    QDataStream & stream,
    QfGnssPositionInformation & position
) 
```




<hr>



### function operator&gt;&gt; 

```C++
QDataStream & operator>> (
    QDataStream & stream,
    QgsSatelliteInfo & satelliteInfo
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfgnsspositioninformation.cpp`

