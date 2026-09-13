

# Class QfStringUtils



[**ClassList**](annotated.md) **>** [**QfStringUtils**](classQfStringUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfStringUtils**](#function-qfstringutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE double | [**calcFuzzyScore**](#function-calcfuzzyscore) (const QString & string, const QString & searchTerm) <br>_Calculates a fuzzy matching score between a_ _string_ _and a__searchTerm_ _._ |
|  Q\_INVOKABLE QString | [**createUuid**](#function-createuuid) () <br>_Returns a new UUID string._  |
|  const QStringList | [**csvToStringList**](#function-csvtostringlist) (const QString & string) <br>_Returns a list from a CSV formatted_ _string_ _._ |
|  Q\_INVOKABLE bool | [**hasLinks**](#function-haslinks) (const QString & string) <br>_Returns whether a string contains one or more URLs._  |
|  Q\_INVOKABLE QString | [**highlightText**](#function-highlighttext) (const QString & string, const QString & highlightText, const QColor & highlightColor=QColor()) <br>_Returns a string highlighting a text using HTML formatting._  |
|  Q\_INVOKABLE QString | [**insertLinks**](#function-insertlinks) (const QString & string) <br>_Returns a string with any URL (e.g., http(s)/ftp) and mailto: text converted to valid HTML  links._  |
|  Q\_INVOKABLE QString | [**pointInformation**](#function-pointinformation) (const QgsPoint & point, const QgsCoordinateReferenceSystem & crs) <br>_Returns a string containing the_ _point_ _location and details of the__crs_ _._ |
|  Q\_INVOKABLE QString | [**replaceFilenameTags**](#function-replacefilenametags) (const QString & string, const QString & filename) <br> |
|  Q\_INVOKABLE QString | [**snippet**](#function-snippet) (const QString & string, int desiredCharactersLength=160) <br>_Returns a short snippet representing the beginning of a longer text._  |
|  const QString | [**stringListToCsv**](#function-stringlisttocsv) (QStringList list) <br>_Returns a CSV formatted string from a_ _list_ _._ |


























## Public Functions Documentation




### function QfStringUtils 

```C++
explicit QfStringUtils::QfStringUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function calcFuzzyScore 

_Calculates a fuzzy matching score between a_ _string_ _and a__searchTerm_ _._
```C++
static Q_INVOKABLE double QfStringUtils::calcFuzzyScore (
    const QString & string,
    const QString & searchTerm
) 
```




<hr>



### function createUuid 

_Returns a new UUID string._ 
```C++
static Q_INVOKABLE QString QfStringUtils::createUuid () 
```




<hr>



### function csvToStringList 

_Returns a list from a CSV formatted_ _string_ _._
```C++
static const QStringList QfStringUtils::csvToStringList (
    const QString & string
) 
```




<hr>



### function hasLinks 

_Returns whether a string contains one or more URLs._ 
```C++
static Q_INVOKABLE bool QfStringUtils::hasLinks (
    const QString & string
) 
```




<hr>



### function highlightText 

_Returns a string highlighting a text using HTML formatting._ 
```C++
static Q_INVOKABLE QString QfStringUtils::highlightText (
    const QString & string,
    const QString & highlightText,
    const QColor & highlightColor=QColor()
) 
```




<hr>



### function insertLinks 

_Returns a string with any URL (e.g., http(s)/ftp) and mailto: text converted to valid HTML  links._ 
```C++
static Q_INVOKABLE QString QfStringUtils::insertLinks (
    const QString & string
) 
```




<hr>



### function pointInformation 

_Returns a string containing the_ _point_ _location and details of the__crs_ _._
```C++
static Q_INVOKABLE QString QfStringUtils::pointInformation (
    const QgsPoint & point,
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function replaceFilenameTags 

```C++
static Q_INVOKABLE QString QfStringUtils::replaceFilenameTags (
    const QString & string,
    const QString & filename
) 
```



Returns a string in which QField-specific filename tags have been replaced with their values. 


        

<hr>



### function snippet 

_Returns a short snippet representing the beginning of a longer text._ 
```C++
static Q_INVOKABLE QString QfStringUtils::snippet (
    const QString & string,
    int desiredCharactersLength=160
) 
```




<hr>



### function stringListToCsv 

_Returns a CSV formatted string from a_ _list_ _._
```C++
static const QString QfStringUtils::stringListToCsv (
    QStringList list
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfstringutils.h`

