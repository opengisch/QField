

# Class StringUtils



[**ClassList**](annotated.md) **>** [**StringUtils**](classStringUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**StringUtils**](#function-stringutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  double | [**calcFuzzyScore**](#function-calcfuzzyscore) (const QString & string, const QString & searchTerm) <br>_Calculates a fuzzy matching score between a_ _string_ _and a__searchTerm_ _._ |
|  Q\_INVOKABLE QString | [**createUuid**](#function-createuuid) () <br>_Returns a new UUID string._  |
|  const QStringList | [**csvToStringList**](#function-csvtostringlist) (const QString & string) <br>_Returns a list from a CSV formatted_ _string_ _._ |
|  bool | [**fuzzyMatch**](#function-fuzzymatch) (const QString & source, const QString & term) <br>_Checks whether the string_ _term_ _is part of__source_ _._ |
|  Q\_INVOKABLE bool | [**hasLinks**](#function-haslinks) (const QString & string) <br>_Returns whether a string contains one or more URLs._  |
|  Q\_INVOKABLE QString | [**highlightText**](#function-highlighttext) (const QString & string, const QString & highlightText, const QColor & highlightColor=QColor()) <br>_Returns a string highlighting a text using HTML formatting._  |
|  Q\_INVOKABLE QString | [**insertLinks**](#function-insertlinks) (const QString & string) <br>_Returns a string with any URL (e.g., http(s)/ftp) and mailto: text converted to valid HTML  links._  |
|  Q\_INVOKABLE QString | [**pointInformation**](#function-pointinformation) (const QgsPoint & point, const QgsCoordinateReferenceSystem & crs) <br>_Returns a string containing the_ _point_ _location and details of the__crs_ _._ |
|  Q\_INVOKABLE QString | [**replaceFilenameTags**](#function-replacefilenametags) (const QString & string, const QString & filename) <br> |
|  const QString | [**stringListToCsv**](#function-stringlisttocsv) (QStringList list) <br>_Returns a CSV formatted string from a_ _list_ _._ |


























## Public Functions Documentation




### function StringUtils 

```C++
explicit StringUtils::StringUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function calcFuzzyScore 

_Calculates a fuzzy matching score between a_ _string_ _and a__searchTerm_ _._
```C++
static double StringUtils::calcFuzzyScore (
    const QString & string,
    const QString & searchTerm
) 
```




<hr>



### function createUuid 

_Returns a new UUID string._ 
```C++
static Q_INVOKABLE QString StringUtils::createUuid () 
```




<hr>



### function csvToStringList 

_Returns a list from a CSV formatted_ _string_ _._
```C++
static const QStringList StringUtils::csvToStringList (
    const QString & string
) 
```




<hr>



### function fuzzyMatch 

_Checks whether the string_ _term_ _is part of__source_ _._
```C++
static bool StringUtils::fuzzyMatch (
    const QString & source,
    const QString & term
) 
```




<hr>



### function hasLinks 

_Returns whether a string contains one or more URLs._ 
```C++
static Q_INVOKABLE bool StringUtils::hasLinks (
    const QString & string
) 
```




<hr>



### function highlightText 

_Returns a string highlighting a text using HTML formatting._ 
```C++
static Q_INVOKABLE QString StringUtils::highlightText (
    const QString & string,
    const QString & highlightText,
    const QColor & highlightColor=QColor()
) 
```




<hr>



### function insertLinks 

_Returns a string with any URL (e.g., http(s)/ftp) and mailto: text converted to valid HTML  links._ 
```C++
static Q_INVOKABLE QString StringUtils::insertLinks (
    const QString & string
) 
```




<hr>



### function pointInformation 

_Returns a string containing the_ _point_ _location and details of the__crs_ _._
```C++
static Q_INVOKABLE QString StringUtils::pointInformation (
    const QgsPoint & point,
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function replaceFilenameTags 

```C++
static Q_INVOKABLE QString StringUtils::replaceFilenameTags (
    const QString & string,
    const QString & filename
) 
```



Returns a string in which QField-specific filename tags have been replaced with their values. 


        

<hr>



### function stringListToCsv 

_Returns a CSV formatted string from a_ _list_ _._
```C++
static const QString StringUtils::stringListToCsv (
    QStringList list
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/stringutils.h`

