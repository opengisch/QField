

# Class FeatureListModel



[**ClassList**](annotated.md) **>** [**FeatureListModel**](classFeatureListModel.md)



[More...](#detailed-description)

* `#include <featurelistmodel.h>`



Inherits the following classes: QAbstractItemModel


Inherited by the following classes: [FeatureCheckListModelBase](classFeatureCheckListModelBase.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureListRoles**](#enum-featurelistroles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**addNull**](classFeatureListModel.md#property-addnull-12)  <br> |
| property [**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classFeatureListModel.md#property-appexpressioncontextscopesgenerator-12)  <br> |
| property QgsFeature | [**currentFormFeature**](classFeatureListModel.md#property-currentformfeature-12)  <br> |
| property QgsVectorLayer \* | [**currentLayer**](classFeatureListModel.md#property-currentlayer-12)  <br> |
| property bool | [**displayGroupName**](classFeatureListModel.md#property-displaygroupname-12)  <br> |
| property QString | [**displayValueField**](classFeatureListModel.md#property-displayvaluefield-12)  <br> |
| property QString | [**filterExpression**](classFeatureListModel.md#property-filterexpression-12)  <br> |
| property QString | [**groupField**](classFeatureListModel.md#property-groupfield-12)  <br> |
| property QString | [**keyField**](classFeatureListModel.md#property-keyfield-12)  <br> |
| property bool | [**orderByValue**](classFeatureListModel.md#property-orderbyvalue-12)  <br> |
| property QString | [**searchTerm**](classFeatureListModel.md#property-searchterm-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**addNullChanged**](classFeatureListModel.md#signal-addnullchanged)  <br> |
| signal void | [**appExpressionContextScopesGeneratorChanged**](classFeatureListModel.md#signal-appexpressioncontextscopesgeneratorchanged)  <br> |
| signal void | [**currentFormFeatureChanged**](classFeatureListModel.md#signal-currentformfeaturechanged)  <br> |
| signal void | [**currentLayerChanged**](classFeatureListModel.md#signal-currentlayerchanged)  <br> |
| signal void | [**displayGroupNameChanged**](classFeatureListModel.md#signal-displaygroupnamechanged)  <br> |
| signal void | [**displayValueFieldChanged**](classFeatureListModel.md#signal-displayvaluefieldchanged)  <br> |
| signal void | [**filterExpressionChanged**](classFeatureListModel.md#signal-filterexpressionchanged)  <br> |
| signal void | [**groupFieldChanged**](classFeatureListModel.md#signal-groupfieldchanged)  <br> |
| signal void | [**keyFieldChanged**](classFeatureListModel.md#signal-keyfieldchanged)  <br> |
| signal void | [**orderByValueChanged**](classFeatureListModel.md#signal-orderbyvaluechanged)  <br> |
| signal void | [**searchTermChanged**](classFeatureListModel.md#signal-searchtermchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeatureListModel**](#function-featurelistmodel) (QObject \* parent=nullptr) <br> |
|  bool | [**addNull**](#function-addnull-22) () const<br> |
|  [**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](#function-appexpressioncontextscopesgenerator-22) () const<br> |
| virtual int | [**columnCount**](#function-columncount) (const QModelIndex & parent) override const<br> |
|  QgsFeature | [**currentFormFeature**](#function-currentformfeature-22) () const<br> |
|  QgsVectorLayer \* | [**currentLayer**](#function-currentlayer-22) () const<br> |
| virtual QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE QVariant | [**dataFromRowIndex**](#function-datafromrowindex) (int row, int role) <br> |
|  bool | [**displayGroupName**](#function-displaygroupname-22) () const<br> |
|  QString | [**displayValueField**](#function-displayvaluefield-22) () const<br> |
|  QString | [**filterExpression**](#function-filterexpression-22) () const<br> |
|  Q\_INVOKABLE QList&lt; int &gt; | [**findDisplayValueMatches**](#function-finddisplayvaluematches) (const QString & filter) const<br> |
|  Q\_INVOKABLE int | [**findKey**](#function-findkey) (const QVariant & key) const<br> |
|  Q\_INVOKABLE QgsFeature | [**getFeatureById**](#function-getfeaturebyid) (QgsFeatureId id) const<br> |
|  Q\_INVOKABLE QgsFeature | [**getFeatureFromKeyValue**](#function-getfeaturefromkeyvalue) (const QVariant & value) const<br> |
|  QString | [**groupField**](#function-groupfield-22) () const<br> |
| virtual QModelIndex | [**index**](#function-index) (int row, int column, const QModelIndex & parent) override const<br> |
|  QString | [**keyField**](#function-keyfield-22) () const<br> |
|  bool | [**orderByValue**](#function-orderbyvalue-22) () const<br> |
| virtual QModelIndex | [**parent**](#function-parent) (const QModelIndex & child) override const<br> |
| virtual QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
| virtual int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  QString | [**searchTerm**](#function-searchterm-22) () const<br> |
|  void | [**setAddNull**](#function-setaddnull) (bool addNull) <br> |
|  void | [**setAppExpressionContextScopesGenerator**](#function-setappexpressioncontextscopesgenerator) ([**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* generator) <br> |
|  void | [**setCurrentFormFeature**](#function-setcurrentformfeature) (const QgsFeature & feature) <br> |
|  void | [**setCurrentLayer**](#function-setcurrentlayer) (QgsVectorLayer \* currentLayer) <br> |
|  void | [**setDisplayGroupName**](#function-setdisplaygroupname) (bool displayGroupName) <br> |
|  void | [**setDisplayValueField**](#function-setdisplayvaluefield) (const QString & displayValueField) <br> |
|  void | [**setFilterExpression**](#function-setfilterexpression) (const QString & filterExpression) <br> |
|  void | [**setGroupField**](#function-setgroupfield) (const QString & groupField) <br> |
|  void | [**setKeyField**](#function-setkeyfield) (const QString & keyField) <br> |
|  void | [**setOrderByValue**](#function-setorderbyvalue) (bool orderByValue) <br> |
|  void | [**setSearchTerm**](#function-setsearchterm) (const QString & searchTerm) <br> |
|   | [**~FeatureListModel**](#function-featurelistmodel) () <br> |




























## Detailed Description


Provides access to a list of features from a layer. For each feature, the display expression is exposed as DisplayRole and a keyField as KeyFieldRole for a unique identifier. If a displayValueField is set it replaces the display expression of the layer. 


    
## Public Types Documentation




### enum FeatureListRoles 

```C++
enum FeatureListModel::FeatureListRoles {
    KeyFieldRole = Qt::UserRole + 1,
    DisplayStringRole,
    GroupFieldRole,
    FeatureIdRole
};
```




<hr>
## Public Properties Documentation




### property addNull [1/2]

```C++
bool FeatureListModel::addNull;
```



Set to TRUE if null values are allowed in the list 


        

<hr>



### property appExpressionContextScopesGenerator [1/2]

```C++
AppExpressionContextScopesGenerator * FeatureListModel::appExpressionContextScopesGenerator;
```



The application expression context scope generator used when filtering by expression 


        

<hr>



### property currentFormFeature [1/2]

```C++
QgsFeature FeatureListModel::currentFormFeature;
```



The current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### property currentLayer [1/2]

```C++
QgsVectorLayer * FeatureListModel::currentLayer;
```



The vector layer to list 


        

<hr>



### property displayGroupName [1/2]

```C++
bool FeatureListModel::displayGroupName;
```



Set to TRUE if the group name will be displayed in the list 


        

<hr>



### property displayValueField [1/2]

```C++
QString FeatureListModel::displayValueField;
```



The display value field 


        

<hr>



### property filterExpression [1/2]

```C++
QString FeatureListModel::filterExpression;
```



Expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### property groupField [1/2]

```C++
QString FeatureListModel::groupField;
```



The grouping key field 


        

<hr>



### property keyField [1/2]

```C++
QString FeatureListModel::keyField;
```



The primary key field 


        

<hr>



### property orderByValue [1/2]

```C++
bool FeatureListModel::orderByValue;
```



Set to TRUE if features should be ordered by value 


        

<hr>



### property searchTerm [1/2]

```C++
QString FeatureListModel::searchTerm;
```



The current search term used to filter items. 


        

<hr>
## Public Signals Documentation




### signal addNullChanged 

```C++
void FeatureListModel::addNullChanged;
```




<hr>



### signal appExpressionContextScopesGeneratorChanged 

```C++
void FeatureListModel::appExpressionContextScopesGeneratorChanged;
```




<hr>



### signal currentFormFeatureChanged 

```C++
void FeatureListModel::currentFormFeatureChanged;
```




<hr>



### signal currentLayerChanged 

```C++
void FeatureListModel::currentLayerChanged;
```




<hr>



### signal displayGroupNameChanged 

```C++
void FeatureListModel::displayGroupNameChanged;
```




<hr>



### signal displayValueFieldChanged 

```C++
void FeatureListModel::displayValueFieldChanged;
```




<hr>



### signal filterExpressionChanged 

```C++
void FeatureListModel::filterExpressionChanged;
```




<hr>



### signal groupFieldChanged 

```C++
void FeatureListModel::groupFieldChanged;
```




<hr>



### signal keyFieldChanged 

```C++
void FeatureListModel::keyFieldChanged;
```




<hr>



### signal orderByValueChanged 

```C++
void FeatureListModel::orderByValueChanged;
```




<hr>



### signal searchTermChanged 

```C++
void FeatureListModel::searchTermChanged;
```




<hr>
## Public Functions Documentation




### function FeatureListModel 

```C++
explicit FeatureListModel::FeatureListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addNull [2/2]

```C++
bool FeatureListModel::addNull () const
```



Add a NULL value as the first entry. 


        

<hr>



### function appExpressionContextScopesGenerator [2/2]

```C++
AppExpressionContextScopesGenerator * FeatureListModel::appExpressionContextScopesGenerator () const
```



Returns the application expression context scope generator used when filtering by expression 


        

<hr>



### function columnCount 

```C++
virtual int FeatureListModel::columnCount (
    const QModelIndex & parent
) override const
```




<hr>



### function currentFormFeature [2/2]

```C++
QgsFeature FeatureListModel::currentFormFeature () const
```



The current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### function currentLayer [2/2]

```C++
QgsVectorLayer * FeatureListModel::currentLayer () const
```




<hr>



### function data 

```C++
virtual QVariant FeatureListModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function dataFromRowIndex 

```C++
inline Q_INVOKABLE QVariant FeatureListModel::dataFromRowIndex (
    int row,
    int role
) 
```




<hr>



### function displayGroupName [2/2]

```C++
bool FeatureListModel::displayGroupName () const
```




<hr>



### function displayValueField [2/2]

```C++
QString FeatureListModel::displayValueField () const
```




<hr>



### function filterExpression [2/2]

```C++
QString FeatureListModel::filterExpression () const
```



Expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### function findDisplayValueMatches 

```C++
Q_INVOKABLE QList< int > FeatureListModel::findDisplayValueMatches (
    const QString & filter
) const
```



Get rows for a given filter string used to match display values. 


        

<hr>



### function findKey 

```C++
Q_INVOKABLE int FeatureListModel::findKey (
    const QVariant & key
) const
```



Get the row for a given key value. 


        

<hr>



### function getFeatureById 

```C++
Q_INVOKABLE QgsFeature FeatureListModel::getFeatureById (
    QgsFeatureId id
) const
```



Returns the first feature matching the key _id_. 


        

<hr>



### function getFeatureFromKeyValue 

```C++
Q_INVOKABLE QgsFeature FeatureListModel::getFeatureFromKeyValue (
    const QVariant & value
) const
```



Returns the first feature matching the key _value_. 


        

<hr>



### function groupField [2/2]

```C++
QString FeatureListModel::groupField () const
```




<hr>



### function index 

```C++
virtual QModelIndex FeatureListModel::index (
    int row,
    int column,
    const QModelIndex & parent
) override const
```




<hr>



### function keyField [2/2]

```C++
QString FeatureListModel::keyField () const
```




<hr>



### function orderByValue [2/2]

```C++
bool FeatureListModel::orderByValue () const
```



Orders all the values alphabethically by their displayString. 


        

<hr>



### function parent 

```C++
virtual QModelIndex FeatureListModel::parent (
    const QModelIndex & child
) override const
```




<hr>



### function roleNames 

```C++
virtual QHash< int, QByteArray > FeatureListModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
virtual int FeatureListModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function searchTerm [2/2]

```C++
QString FeatureListModel::searchTerm () const
```



Returns the current search term used to filter items. 


        

<hr>



### function setAddNull 

```C++
void FeatureListModel::setAddNull (
    bool addNull
) 
```



Add a NULL value as the first entry. 


        

<hr>



### function setAppExpressionContextScopesGenerator 

```C++
void FeatureListModel::setAppExpressionContextScopesGenerator (
    AppExpressionContextScopesGenerator * generator
) 
```



Sets the application expression context scope generator used when filtering by expression 


        

<hr>



### function setCurrentFormFeature 

```C++
void FeatureListModel::setCurrentFormFeature (
    const QgsFeature & feature
) 
```



Sets the current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### function setCurrentLayer 

```C++
void FeatureListModel::setCurrentLayer (
    QgsVectorLayer * currentLayer
) 
```




<hr>



### function setDisplayGroupName 

```C++
void FeatureListModel::setDisplayGroupName (
    bool displayGroupName
) 
```




<hr>



### function setDisplayValueField 

```C++
void FeatureListModel::setDisplayValueField (
    const QString & displayValueField
) 
```




<hr>



### function setFilterExpression 

```C++
void FeatureListModel::setFilterExpression (
    const QString & filterExpression
) 
```



Sets an expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### function setGroupField 

```C++
void FeatureListModel::setGroupField (
    const QString & groupField
) 
```




<hr>



### function setKeyField 

```C++
void FeatureListModel::setKeyField (
    const QString & keyField
) 
```




<hr>



### function setOrderByValue 

```C++
void FeatureListModel::setOrderByValue (
    bool orderByValue
) 
```



Orders all the values alphabethically by their displayString. 


        

<hr>



### function setSearchTerm 

```C++
void FeatureListModel::setSearchTerm (
    const QString & searchTerm
) 
```



Sets the search term for filtering and updates the filter. 


        

<hr>



### function ~FeatureListModel 

```C++
FeatureListModel::~FeatureListModel () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featurelistmodel.h`

