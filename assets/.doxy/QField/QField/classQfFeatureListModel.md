

# Class QfFeatureListModel



[**ClassList**](annotated.md) **>** [**QfFeatureListModel**](classQfFeatureListModel.md)



[More...](#detailed-description)

* `#include <qffeaturelistmodel.h>`



Inherits the following classes: QAbstractItemModel


Inherited by the following classes: [QfFeatureCheckListModelBase](classQfFeatureCheckListModelBase.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureListRoles**](#enum-featurelistroles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**addNull**](classQfFeatureListModel.md#property-addnull-12)  <br> |
| property [**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classQfFeatureListModel.md#property-appexpressioncontextscopesgenerator-12)  <br> |
| property QgsFeature | [**currentFormFeature**](classQfFeatureListModel.md#property-currentformfeature-12)  <br> |
| property QgsVectorLayer \* | [**currentLayer**](classQfFeatureListModel.md#property-currentlayer-12)  <br> |
| property bool | [**displayGroupName**](classQfFeatureListModel.md#property-displaygroupname-12)  <br> |
| property QString | [**displayValueField**](classQfFeatureListModel.md#property-displayvaluefield-12)  <br> |
| property QString | [**filterExpression**](classQfFeatureListModel.md#property-filterexpression-12)  <br> |
| property QString | [**groupField**](classQfFeatureListModel.md#property-groupfield-12)  <br> |
| property QString | [**keyField**](classQfFeatureListModel.md#property-keyfield-12)  <br> |
| property bool | [**orderByField**](classQfFeatureListModel.md#property-orderbyfield-12)  <br> |
| property QString | [**orderByFieldName**](classQfFeatureListModel.md#property-orderbyfieldname-12)  <br> |
| property bool | [**orderByValue**](classQfFeatureListModel.md#property-orderbyvalue-12)  <br> |
| property QString | [**searchTerm**](classQfFeatureListModel.md#property-searchterm-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**addNullChanged**](classQfFeatureListModel.md#signal-addnullchanged)  <br> |
| signal void | [**appExpressionContextScopesGeneratorChanged**](classQfFeatureListModel.md#signal-appexpressioncontextscopesgeneratorchanged)  <br> |
| signal void | [**currentFormFeatureChanged**](classQfFeatureListModel.md#signal-currentformfeaturechanged)  <br> |
| signal void | [**currentLayerChanged**](classQfFeatureListModel.md#signal-currentlayerchanged)  <br> |
| signal void | [**displayGroupNameChanged**](classQfFeatureListModel.md#signal-displaygroupnamechanged)  <br> |
| signal void | [**displayValueFieldChanged**](classQfFeatureListModel.md#signal-displayvaluefieldchanged)  <br> |
| signal void | [**filterExpressionChanged**](classQfFeatureListModel.md#signal-filterexpressionchanged)  <br> |
| signal void | [**groupFieldChanged**](classQfFeatureListModel.md#signal-groupfieldchanged)  <br> |
| signal void | [**keyFieldChanged**](classQfFeatureListModel.md#signal-keyfieldchanged)  <br> |
| signal void | [**orderByFieldChanged**](classQfFeatureListModel.md#signal-orderbyfieldchanged)  <br> |
| signal void | [**orderByFieldNameChanged**](classQfFeatureListModel.md#signal-orderbyfieldnamechanged)  <br> |
| signal void | [**orderByValueChanged**](classQfFeatureListModel.md#signal-orderbyvaluechanged)  <br> |
| signal void | [**searchTermChanged**](classQfFeatureListModel.md#signal-searchtermchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureListModel**](#function-qffeaturelistmodel) (QObject \* parent=nullptr) <br> |
|  bool | [**addNull**](#function-addnull-22) () const<br> |
|  [**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](#function-appexpressioncontextscopesgenerator-22) () const<br> |
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
|  bool | [**orderByField**](#function-orderbyfield-22) () const<br> |
|  QString | [**orderByFieldName**](#function-orderbyfieldname-22) () const<br> |
|  bool | [**orderByValue**](#function-orderbyvalue-22) () const<br> |
| virtual QModelIndex | [**parent**](#function-parent) (const QModelIndex & child) override const<br> |
| virtual QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
| virtual int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  QString | [**searchTerm**](#function-searchterm-22) () const<br> |
|  void | [**setAddNull**](#function-setaddnull) (bool addNull) <br> |
|  void | [**setAppExpressionContextScopesGenerator**](#function-setappexpressioncontextscopesgenerator) ([**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* generator) <br> |
|  void | [**setCurrentFormFeature**](#function-setcurrentformfeature) (const QgsFeature & feature) <br> |
|  void | [**setCurrentLayer**](#function-setcurrentlayer) (QgsVectorLayer \* currentLayer) <br> |
|  void | [**setDisplayGroupName**](#function-setdisplaygroupname) (bool displayGroupName) <br> |
|  void | [**setDisplayValueField**](#function-setdisplayvaluefield) (const QString & displayValueField) <br> |
|  void | [**setFilterExpression**](#function-setfilterexpression) (const QString & filterExpression) <br> |
|  void | [**setGroupField**](#function-setgroupfield) (const QString & groupField) <br> |
|  void | [**setKeyField**](#function-setkeyfield) (const QString & keyField) <br> |
|  void | [**setOrderByField**](#function-setorderbyfield) (bool orderByField) <br> |
|  void | [**setOrderByFieldName**](#function-setorderbyfieldname) (const QString & orderByFieldName) <br> |
|  void | [**setOrderByValue**](#function-setorderbyvalue) (bool orderByValue) <br> |
|  void | [**setSearchTerm**](#function-setsearchterm) (const QString & searchTerm) <br> |
|   | [**~QfFeatureListModel**](#function-qffeaturelistmodel) () <br> |




























## Detailed Description


Provides access to a list of features from a layer. For each feature, the display expression is exposed as DisplayRole and a keyField as KeyFieldRole for a unique identifier. If a displayValueField is set it replaces the display expression of the layer. 


    
## Public Types Documentation




### enum FeatureListRoles 

```C++
enum QfFeatureListModel::FeatureListRoles {
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
bool QfFeatureListModel::addNull;
```



Set to TRUE if null values are allowed in the list 


        

<hr>



### property appExpressionContextScopesGenerator [1/2]

```C++
QfAppExpressionContextScopesGenerator * QfFeatureListModel::appExpressionContextScopesGenerator;
```



The application expression context scope generator used when filtering by expression 


        

<hr>



### property currentFormFeature [1/2]

```C++
QgsFeature QfFeatureListModel::currentFormFeature;
```



The current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### property currentLayer [1/2]

```C++
QgsVectorLayer * QfFeatureListModel::currentLayer;
```



The vector layer to list 


        

<hr>



### property displayGroupName [1/2]

```C++
bool QfFeatureListModel::displayGroupName;
```



Set to TRUE if the group name will be displayed in the list 


        

<hr>



### property displayValueField [1/2]

```C++
QString QfFeatureListModel::displayValueField;
```



The display value field 


        

<hr>



### property filterExpression [1/2]

```C++
QString QfFeatureListModel::filterExpression;
```



Expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### property groupField [1/2]

```C++
QString QfFeatureListModel::groupField;
```



The grouping key field 


        

<hr>



### property keyField [1/2]

```C++
QString QfFeatureListModel::keyField;
```



The primary key field 


        

<hr>



### property orderByField [1/2]

```C++
bool QfFeatureListModel::orderByField;
```



Set to TRUE if features should be ordered by a field 


        

<hr>



### property orderByFieldName [1/2]

```C++
QString QfFeatureListModel::orderByFieldName;
```



Name of field to order features by 


        

<hr>



### property orderByValue [1/2]

```C++
bool QfFeatureListModel::orderByValue;
```



Set to TRUE if features should be ordered by value 


        

<hr>



### property searchTerm [1/2]

```C++
QString QfFeatureListModel::searchTerm;
```



The current search term used to filter items. 


        

<hr>
## Public Signals Documentation




### signal addNullChanged 

```C++
void QfFeatureListModel::addNullChanged;
```




<hr>



### signal appExpressionContextScopesGeneratorChanged 

```C++
void QfFeatureListModel::appExpressionContextScopesGeneratorChanged;
```




<hr>



### signal currentFormFeatureChanged 

```C++
void QfFeatureListModel::currentFormFeatureChanged;
```




<hr>



### signal currentLayerChanged 

```C++
void QfFeatureListModel::currentLayerChanged;
```




<hr>



### signal displayGroupNameChanged 

```C++
void QfFeatureListModel::displayGroupNameChanged;
```




<hr>



### signal displayValueFieldChanged 

```C++
void QfFeatureListModel::displayValueFieldChanged;
```




<hr>



### signal filterExpressionChanged 

```C++
void QfFeatureListModel::filterExpressionChanged;
```




<hr>



### signal groupFieldChanged 

```C++
void QfFeatureListModel::groupFieldChanged;
```




<hr>



### signal keyFieldChanged 

```C++
void QfFeatureListModel::keyFieldChanged;
```




<hr>



### signal orderByFieldChanged 

```C++
void QfFeatureListModel::orderByFieldChanged;
```




<hr>



### signal orderByFieldNameChanged 

```C++
void QfFeatureListModel::orderByFieldNameChanged;
```




<hr>



### signal orderByValueChanged 

```C++
void QfFeatureListModel::orderByValueChanged;
```




<hr>



### signal searchTermChanged 

```C++
void QfFeatureListModel::searchTermChanged;
```




<hr>
## Public Functions Documentation




### function QfFeatureListModel 

```C++
explicit QfFeatureListModel::QfFeatureListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addNull [2/2]

```C++
bool QfFeatureListModel::addNull () const
```



Add a NULL value as the first entry. 


        

<hr>



### function appExpressionContextScopesGenerator [2/2]

```C++
QfAppExpressionContextScopesGenerator * QfFeatureListModel::appExpressionContextScopesGenerator () const
```



Returns the application expression context scope generator used when filtering by expression 


        

<hr>



### function columnCount 

```C++
virtual int QfFeatureListModel::columnCount (
    const QModelIndex & parent
) override const
```




<hr>



### function currentFormFeature [2/2]

```C++
QgsFeature QfFeatureListModel::currentFormFeature () const
```



The current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### function currentLayer [2/2]

```C++
QgsVectorLayer * QfFeatureListModel::currentLayer () const
```




<hr>



### function data 

```C++
virtual QVariant QfFeatureListModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function dataFromRowIndex 

```C++
inline Q_INVOKABLE QVariant QfFeatureListModel::dataFromRowIndex (
    int row,
    int role
) 
```




<hr>



### function displayGroupName [2/2]

```C++
bool QfFeatureListModel::displayGroupName () const
```




<hr>



### function displayValueField [2/2]

```C++
QString QfFeatureListModel::displayValueField () const
```




<hr>



### function filterExpression [2/2]

```C++
QString QfFeatureListModel::filterExpression () const
```



Expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### function findDisplayValueMatches 

```C++
Q_INVOKABLE QList< int > QfFeatureListModel::findDisplayValueMatches (
    const QString & filter
) const
```



Get rows for a given filter string used to match display values. 


        

<hr>



### function findKey 

```C++
Q_INVOKABLE int QfFeatureListModel::findKey (
    const QVariant & key
) const
```



Get the row for a given key value. 


        

<hr>



### function getFeatureById 

```C++
Q_INVOKABLE QgsFeature QfFeatureListModel::getFeatureById (
    QgsFeatureId id
) const
```



Returns the first feature matching the key _id_. 


        

<hr>



### function getFeatureFromKeyValue 

```C++
Q_INVOKABLE QgsFeature QfFeatureListModel::getFeatureFromKeyValue (
    const QVariant & value
) const
```



Returns the first feature matching the key _value_. 


        

<hr>



### function groupField [2/2]

```C++
QString QfFeatureListModel::groupField () const
```




<hr>



### function index 

```C++
virtual QModelIndex QfFeatureListModel::index (
    int row,
    int column,
    const QModelIndex & parent
) override const
```




<hr>



### function keyField [2/2]

```C++
QString QfFeatureListModel::keyField () const
```




<hr>



### function orderByField [2/2]

```C++
bool QfFeatureListModel::orderByField () const
```



Orders all the values alphabethically by a field 


        

<hr>



### function orderByFieldName [2/2]

```C++
QString QfFeatureListModel::orderByFieldName () const
```



Orders all the values by another field. 


        

<hr>



### function orderByValue [2/2]

```C++
bool QfFeatureListModel::orderByValue () const
```



Orders all the values alphabethically by their displayString. 


        

<hr>



### function parent 

```C++
virtual QModelIndex QfFeatureListModel::parent (
    const QModelIndex & child
) override const
```




<hr>



### function roleNames 

```C++
virtual QHash< int, QByteArray > QfFeatureListModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
virtual int QfFeatureListModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function searchTerm [2/2]

```C++
QString QfFeatureListModel::searchTerm () const
```



Returns the current search term used to filter items. 


        

<hr>



### function setAddNull 

```C++
void QfFeatureListModel::setAddNull (
    bool addNull
) 
```



Add a NULL value as the first entry. 


        

<hr>



### function setAppExpressionContextScopesGenerator 

```C++
void QfFeatureListModel::setAppExpressionContextScopesGenerator (
    QfAppExpressionContextScopesGenerator * generator
) 
```



Sets the application expression context scope generator used when filtering by expression 


        

<hr>



### function setCurrentFormFeature 

```C++
void QfFeatureListModel::setCurrentFormFeature (
    const QgsFeature & feature
) 
```



Sets the current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### function setCurrentLayer 

```C++
void QfFeatureListModel::setCurrentLayer (
    QgsVectorLayer * currentLayer
) 
```




<hr>



### function setDisplayGroupName 

```C++
void QfFeatureListModel::setDisplayGroupName (
    bool displayGroupName
) 
```




<hr>



### function setDisplayValueField 

```C++
void QfFeatureListModel::setDisplayValueField (
    const QString & displayValueField
) 
```




<hr>



### function setFilterExpression 

```C++
void QfFeatureListModel::setFilterExpression (
    const QString & filterExpression
) 
```



Sets an expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### function setGroupField 

```C++
void QfFeatureListModel::setGroupField (
    const QString & groupField
) 
```




<hr>



### function setKeyField 

```C++
void QfFeatureListModel::setKeyField (
    const QString & keyField
) 
```




<hr>



### function setOrderByField 

```C++
void QfFeatureListModel::setOrderByField (
    bool orderByField
) 
```



Orders all the values alphabethically by a field 


        

<hr>



### function setOrderByFieldName 

```C++
void QfFeatureListModel::setOrderByFieldName (
    const QString & orderByFieldName
) 
```



Orders all the values by another field. 


        

<hr>



### function setOrderByValue 

```C++
void QfFeatureListModel::setOrderByValue (
    bool orderByValue
) 
```



Orders all the values alphabethically by their displayString. 


        

<hr>



### function setSearchTerm 

```C++
void QfFeatureListModel::setSearchTerm (
    const QString & searchTerm
) 
```



Sets the search term for filtering and updates the filter. 


        

<hr>



### function ~QfFeatureListModel 

```C++
QfFeatureListModel::~QfFeatureListModel () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qffeaturelistmodel.h`

