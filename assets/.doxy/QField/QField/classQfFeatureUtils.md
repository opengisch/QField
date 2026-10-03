

# Class QfFeatureUtils



[**ClassList**](annotated.md) **>** [**QfFeatureUtils**](classQfFeatureUtils.md)








Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FieldType**](#enum-fieldtype)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureUtils**](#function-qffeatureutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE bool | [**attributeIsNull**](#function-attributeisnull) (const QVariant & value) <br> |
|  Q\_INVOKABLE QgsFeature | [**createBlankFeature**](#function-createblankfeature) (const QgsFields & fields=QgsFields(), const QgsGeometry & geometry=QgsGeometry()) <br> |
|  Q\_INVOKABLE QgsFeature | [**createFeature**](#function-createfeature) (QgsVectorLayer \* layer, const QgsGeometry & geometry=QgsGeometry(), const [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & positionInformation=[**QfGnssPositionInformation**](classQfGnssPositionInformation.md)(), const [**QfCloudUserInformation**](structQfCloudUserInformation.md) & cloudUserInformation=[**QfCloudUserInformation**](structQfCloudUserInformation.md)()) <br> |
|  Q\_INVOKABLE QgsField | [**createField**](#function-createfield) (const QString & name, FieldType type, const QString & typeName=QString(), int length=0, int precision=0, const QString & comment=QString()) <br> |
|  Q\_INVOKABLE QgsFields | [**createFields**](#function-createfields) (const QList&lt; QgsField &gt; & fields) <br> |
|  Q\_INVOKABLE QString | [**displayName**](#function-displayname) (QgsVectorLayer \* layer, const QgsFeature & feature) <br> |
|  Q\_INVOKABLE QgsRectangle | [**extent**](#function-extent) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings, QgsVectorLayer \* layer, const QgsFeature & feature, bool skipSinglePointLogic=false) <br> |
|  Q\_INVOKABLE QList&lt; QgsFeature &gt; | [**featuresFromJsonString**](#function-featuresfromjsonstring) (const QString & string) <br> |


























## Public Types Documentation




### enum FieldType 

```C++
enum QfFeatureUtils::FieldType {
    Bool = QMetaType::Type::Bool,
    Int = QMetaType::Type::Int,
    String = QMetaType::Type::QString,
    Double = QMetaType::Type::Double,
    Date = QMetaType::Type::QDate,
    DateTime = QMetaType::Type::QDateTime,
    Time = QMetaType::Type::QTime
};
```




<hr>
## Public Functions Documentation




### function QfFeatureUtils 

```C++
explicit QfFeatureUtils::QfFeatureUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function attributeIsNull 

```C++
static Q_INVOKABLE bool QfFeatureUtils::attributeIsNull (
    const QVariant & value
) 
```



Returns TRUE if an attribute value is null. 


        

<hr>



### function createBlankFeature 

```C++
static Q_INVOKABLE QgsFeature QfFeatureUtils::createBlankFeature (
    const QgsFields & fields=QgsFields(),
    const QgsGeometry & geometry=QgsGeometry()
) 
```



Returns a new feature with its _fields_ completely blank. 


        

<hr>



### function createFeature 

```C++
static Q_INVOKABLE QgsFeature QfFeatureUtils::createFeature (
    QgsVectorLayer * layer,
    const QgsGeometry & geometry=QgsGeometry(),
    const QfGnssPositionInformation & positionInformation=QfGnssPositionInformation (),
    const QfCloudUserInformation & cloudUserInformation=QfCloudUserInformation ()
) 
```



Returns a new feature with its fields set to default values. 


        

<hr>



### function createField 

```C++
static Q_INVOKABLE QgsField QfFeatureUtils::createField (
    const QString & name,
    FieldType type,
    const QString & typeName=QString(),
    int length=0,
    int precision=0,
    const QString & comment=QString()
) 
```



Returns a new a field gadget. 


        

<hr>



### function createFields 

```C++
static Q_INVOKABLE QgsFields QfFeatureUtils::createFields (
    const QList< QgsField > & fields
) 
```



Returns a new a fields gadget. 


        

<hr>



### function displayName 

```C++
static Q_INVOKABLE QString QfFeatureUtils::displayName (
    QgsVectorLayer * layer,
    const QgsFeature & feature
) 
```



Returns the display name of a given feature. 

**Parameters:**


* `layer` the vector layer containing the feature 
* `feature` the feature to be named 




        

<hr>



### function extent 

```C++
static Q_INVOKABLE QgsRectangle QfFeatureUtils::extent (
    QgsQuickMapSettings * mapSettings,
    QgsVectorLayer * layer,
    const QgsFeature & feature,
    bool skipSinglePointLogic=false
) 
```



Returns the map extent encompassig a given feature. 

**Parameters:**


* `mapSettings` the map settings used to determine the CRS 
* `layer` the vector layer containing the feature 
* `feature` the feature from which the geometry will be used 
* `skipSinglePointLogic` when TRUE, the special handling for determining a single point extent will be skipped 



**Returns:**

a QgsRectangle extent 





        

<hr>



### function featuresFromJsonString 

```C++
static Q_INVOKABLE QList< QgsFeature > QfFeatureUtils::featuresFromJsonString (
    const QString & string
) 
```



Returns a list of features while attempting to parse a GeoJSON _string_. If the string could not be parsed, an enmpty list will be returned. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qffeatureutils.h`

