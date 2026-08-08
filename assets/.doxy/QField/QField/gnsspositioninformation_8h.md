

# File gnsspositioninformation.h



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**gnsspositioninformation.h**](gnsspositioninformation_8h.md)

[Go to the source code of this file](gnsspositioninformation_8h_source.md)



* `#include <QDateTime>`
* `#include <QObject>`
* `#include <QString>`
* `#include <qgis.h>`
* `#include <qgssatelliteinformation.h>`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**GnssPositionDetails**](classGnssPositionDetails.md) <br> |
| class | [**GnssPositionInformation**](classGnssPositionInformation.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  QDataStream & | [**operator&lt;&lt;**](#function-operator) (QDataStream & stream, const [**GnssPositionDetails**](classGnssPositionDetails.md) & positionDetails) <br> |
|  QDataStream & | [**operator&lt;&lt;**](#function-operator_1) (QDataStream & stream, const [**GnssPositionInformation**](classGnssPositionInformation.md) & position) <br> |
|  QDataStream & | [**operator&lt;&lt;**](#function-operator_2) (QDataStream & stream, const QgsSatelliteInfo & satelliteInfo) <br> |
|  QDataStream & | [**operator&gt;&gt;**](#function-operator_3) (QDataStream & stream, [**GnssPositionDetails**](classGnssPositionDetails.md) & positionDetails) <br> |
|  QDataStream & | [**operator&gt;&gt;**](#function-operator_4) (QDataStream & stream, [**GnssPositionInformation**](classGnssPositionInformation.md) & position) <br> |
|  QDataStream & | [**operator&gt;&gt;**](#function-operator_5) (QDataStream & stream, QgsSatelliteInfo & satelliteInfo) <br> |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**NMEA\_DEF\_LAT**](gnsspositioninformation_8h.md#define-nmea_def_lat)  `( 5001.2621 )`<br> |
| define  | [**NMEA\_DEF\_LON**](gnsspositioninformation_8h.md#define-nmea_def_lon)  `( 3613.0595 )`<br> |
| define  | [**NMEA\_FIX\_2D**](gnsspositioninformation_8h.md#define-nmea_fix_2d)  `( 2 )`<br> |
| define  | [**NMEA\_FIX\_3D**](gnsspositioninformation_8h.md#define-nmea_fix_3d)  `( 3 )`<br> |
| define  | [**NMEA\_FIX\_BAD**](gnsspositioninformation_8h.md#define-nmea_fix_bad)  `( 1 )`<br> |
| define  | [**NMEA\_MAXSAT**](gnsspositioninformation_8h.md#define-nmea_maxsat)  `( 12 )`<br> |
| define  | [**NMEA\_NSATPACKS**](gnsspositioninformation_8h.md#define-nmea_nsatpacks)  `( NMEA\_MAXSAT / NMEA\_SATINPACK )`<br> |
| define  | [**NMEA\_SATINPACK**](gnsspositioninformation_8h.md#define-nmea_satinpack)  `( 4 )`<br> |
| define  | [**NMEA\_SIG\_BAD**](gnsspositioninformation_8h.md#define-nmea_sig_bad)  `( 0 )`<br> |
| define  | [**NMEA\_SIG\_HIGH**](gnsspositioninformation_8h.md#define-nmea_sig_high)  `( 3 )`<br> |
| define  | [**NMEA\_SIG\_LOW**](gnsspositioninformation_8h.md#define-nmea_sig_low)  `( 1 )`<br> |
| define  | [**NMEA\_SIG\_MID**](gnsspositioninformation_8h.md#define-nmea_sig_mid)  `( 2 )`<br> |

## Public Functions Documentation




### function operator&lt;&lt; 

```C++
QDataStream & operator<< (
    QDataStream & stream,
    const GnssPositionDetails & positionDetails
) 
```




<hr>



### function operator&lt;&lt; 

```C++
QDataStream & operator<< (
    QDataStream & stream,
    const GnssPositionInformation & position
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
    GnssPositionDetails & positionDetails
) 
```




<hr>



### function operator&gt;&gt; 

```C++
QDataStream & operator>> (
    QDataStream & stream,
    GnssPositionInformation & position
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
## Macro Definition Documentation





### define NMEA\_DEF\_LAT 

```C++
#define NMEA_DEF_LAT `( 5001.2621 )`
```




<hr>



### define NMEA\_DEF\_LON 

```C++
#define NMEA_DEF_LON `( 3613.0595 )`
```




<hr>



### define NMEA\_FIX\_2D 

```C++
#define NMEA_FIX_2D `( 2 )`
```




<hr>



### define NMEA\_FIX\_3D 

```C++
#define NMEA_FIX_3D `( 3 )`
```




<hr>



### define NMEA\_FIX\_BAD 

```C++
#define NMEA_FIX_BAD `( 1 )`
```




<hr>



### define NMEA\_MAXSAT 

```C++
#define NMEA_MAXSAT `( 12 )`
```




<hr>



### define NMEA\_NSATPACKS 

```C++
#define NMEA_NSATPACKS `( NMEA_MAXSAT / NMEA_SATINPACK )`
```




<hr>



### define NMEA\_SATINPACK 

```C++
#define NMEA_SATINPACK `( 4 )`
```




<hr>



### define NMEA\_SIG\_BAD 

```C++
#define NMEA_SIG_BAD `( 0 )`
```




<hr>



### define NMEA\_SIG\_HIGH 

```C++
#define NMEA_SIG_HIGH `( 3 )`
```




<hr>



### define NMEA\_SIG\_LOW 

```C++
#define NMEA_SIG_LOW `( 1 )`
```




<hr>



### define NMEA\_SIG\_MID 

```C++
#define NMEA_SIG_MID `( 2 )`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/gnsspositioninformation.h`

