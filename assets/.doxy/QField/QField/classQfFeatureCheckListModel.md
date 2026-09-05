

# Class QfFeatureCheckListModel



[**ClassList**](annotated.md) **>** [**QfFeatureCheckListModel**](classQfFeatureCheckListModel.md)



[More...](#detailed-description)

* `#include <qffeaturechecklistmodel.h>`



Inherits the following classes: QSortFilterProxyModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**addNull**](classQfFeatureCheckListModel.md#property-addnull-12)  <br>_Set to TRUE if null values are allowed in the list._  |
| property bool | [**allowMulti**](classQfFeatureCheckListModel.md#property-allowmulti-12)  <br>_The attribute field to have information about type (JSON/HSTORE) etc._  |
| property [**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classQfFeatureCheckListModel.md#property-appexpressioncontextscopesgenerator-12)  <br>_The application expression context scope generator used when filtering by expression._  |
| property QgsField | [**attributeField**](classQfFeatureCheckListModel.md#property-attributefield-12)  <br>_The attribute field to have information about type (JSON/HSTORE) etc._  |
| property QVariant | [**attributeValue**](classQfFeatureCheckListModel.md#property-attributevalue-12)  <br>_The attribute value to generate checklist._  |
| property QgsFeature | [**currentFormFeature**](classQfFeatureCheckListModel.md#property-currentformfeature-12)  <br>_The current form feature, used to evaluate expressions such as_  _current\_value('attr1_ _)`._ |
| property QgsVectorLayer \* | [**currentLayer**](classQfFeatureCheckListModel.md#property-currentlayer-12)  <br>_The vector layer to list._  |
| property bool | [**displayGroupName**](classQfFeatureCheckListModel.md#property-displaygroupname-12)  <br>_Set to TRUE if the group name will be displayed in the list._  |
| property QString | [**displayValueField**](classQfFeatureCheckListModel.md#property-displayvaluefield-12)  <br>_The display value field._  |
| property QString | [**filterExpression**](classQfFeatureCheckListModel.md#property-filterexpression-12)  <br>_Expression to filter features with. Empty string if no filter is applied._  |
| property QString | [**groupField**](classQfFeatureCheckListModel.md#property-groupfield-12)  <br>_The grouping key field._  |
| property QString | [**keyField**](classQfFeatureCheckListModel.md#property-keyfield-12)  <br>_The primary key field._  |
| property bool | [**orderByField**](classQfFeatureCheckListModel.md#property-orderbyfield-12)  <br>_Set to TRUE if features should be ordered by a field._  |
| property QString | [**orderByFieldName**](classQfFeatureCheckListModel.md#property-orderbyfieldname-12)  <br>_Field to sort features over._  |
| property bool | [**orderByValue**](classQfFeatureCheckListModel.md#property-orderbyvalue-12)  <br>_Set to TRUE if features should be ordered by value._  |
| property QString | [**searchTerm**](classQfFeatureCheckListModel.md#property-searchterm-12)  <br>_The current search term used to filter items._  |
| property bool | [**sortCheckedFirst**](classQfFeatureCheckListModel.md#property-sortcheckedfirst-12)  <br>_The sorting method._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**addNullChanged**](classQfFeatureCheckListModel.md#signal-addnullchanged)  <br> |
| signal void | [**allowMultiChanged**](classQfFeatureCheckListModel.md#signal-allowmultichanged)  <br> |
| signal void | [**appExpressionContextScopesGeneratorChanged**](classQfFeatureCheckListModel.md#signal-appexpressioncontextscopesgeneratorchanged)  <br> |
| signal void | [**attributeFieldChanged**](classQfFeatureCheckListModel.md#signal-attributefieldchanged)  <br> |
| signal void | [**attributeValueChanged**](classQfFeatureCheckListModel.md#signal-attributevaluechanged)  <br> |
| signal void | [**currentFormFeatureChanged**](classQfFeatureCheckListModel.md#signal-currentformfeaturechanged)  <br> |
| signal void | [**currentLayerChanged**](classQfFeatureCheckListModel.md#signal-currentlayerchanged)  <br> |
| signal void | [**displayGroupNameChanged**](classQfFeatureCheckListModel.md#signal-displaygroupnamechanged)  <br> |
| signal void | [**displayValueFieldChanged**](classQfFeatureCheckListModel.md#signal-displayvaluefieldchanged)  <br> |
| signal void | [**filterExpressionChanged**](classQfFeatureCheckListModel.md#signal-filterexpressionchanged)  <br> |
| signal void | [**groupFieldChanged**](classQfFeatureCheckListModel.md#signal-groupfieldchanged)  <br> |
| signal void | [**keyFieldChanged**](classQfFeatureCheckListModel.md#signal-keyfieldchanged)  <br> |
| signal void | [**listUpdated**](classQfFeatureCheckListModel.md#signal-listupdated)  <br> |
| signal void | [**orderByFieldChanged**](classQfFeatureCheckListModel.md#signal-orderbyfieldchanged)  <br> |
| signal void | [**orderByFieldNameChanged**](classQfFeatureCheckListModel.md#signal-orderbyfieldnamechanged)  <br> |
| signal void | [**orderByValueChanged**](classQfFeatureCheckListModel.md#signal-orderbyvaluechanged)  <br> |
| signal void | [**searchTermChanged**](classQfFeatureCheckListModel.md#signal-searchtermchanged)  <br> |
| signal void | [**sortCheckedFirstChanged**](classQfFeatureCheckListModel.md#signal-sortcheckedfirstchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureCheckListModel**](#function-qffeaturechecklistmodel) (QObject \* parent=nullptr) <br> |
|  bool | [**addNull**](#function-addnull-22) () const<br> |
|  bool | [**allowMulti**](#function-allowmulti-22) () const<br> |
|  [**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](#function-appexpressioncontextscopesgenerator-22) () const<br> |
|  QgsField | [**attributeField**](#function-attributefield-22) () const<br> |
|  QVariant | [**attributeValue**](#function-attributevalue-22) () const<br> |
|  QgsFeature | [**currentFormFeature**](#function-currentformfeature-22) () const<br> |
|  QgsVectorLayer \* | [**currentLayer**](#function-currentlayer-22) () const<br> |
|  Q\_INVOKABLE QVariant | [**dataFromRowIndex**](#function-datafromrowindex) (int row, int role) <br> |
|  bool | [**displayGroupName**](#function-displaygroupname-22) () const<br> |
|  QString | [**displayValueField**](#function-displayvaluefield-22) () const<br> |
|  QString | [**filterExpression**](#function-filterexpression-22) () const<br> |
|  Q\_INVOKABLE QList&lt; int &gt; | [**findDisplayValueMatches**](#function-finddisplayvaluematches) (const QString & filter) const<br> |
|  Q\_INVOKABLE int | [**findKey**](#function-findkey) (const QVariant & key) const<br> |
|  Q\_INVOKABLE QgsFeature | [**getFeatureById**](#function-getfeaturebyid) (QgsFeatureId id) const<br> |
|  Q\_INVOKABLE QgsFeature | [**getFeatureFromKeyValue**](#function-getfeaturefromkeyvalue) (const QVariant & value) const<br> |
|  QString | [**groupField**](#function-groupfield-22) () const<br> |
|  QString | [**keyField**](#function-keyfield-22) () const<br> |
|  bool | [**orderByField**](#function-orderbyfield-22) () const<br> |
|  QString | [**orderByFieldName**](#function-orderbyfieldname-22) () const<br> |
|  bool | [**orderByValue**](#function-orderbyvalue-22) () const<br> |
|  Q\_INVOKABLE int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  QString | [**searchTerm**](#function-searchterm-22) () const<br> |
|  void | [**setAddNull**](#function-setaddnull) (bool addNull) <br> |
|  void | [**setAllowMulti**](#function-setallowmulti) (bool allowMulti) <br> |
|  void | [**setAppExpressionContextScopesGenerator**](#function-setappexpressioncontextscopesgenerator) ([**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* generator) <br> |
|  void | [**setAttributeField**](#function-setattributefield) (const QgsField & field) <br> |
|  void | [**setAttributeValue**](#function-setattributevalue) (const QVariant & attributeValue) <br> |
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
|  void | [**setSortCheckedFirst**](#function-setsortcheckedfirst) (bool enabled) <br> |
|  bool | [**sortCheckedFirst**](#function-sortcheckedfirst-22) () const<br> |
|  Q\_INVOKABLE void | [**toggleCheckAll**](#function-togglecheckall) (bool toggleChecked=true) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & left, const QModelIndex & right) override const<br> |




## Detailed Description


A proxy model for filtering and sorting feature checklist items.


This model enhances a source model (typically a [**QfFeatureCheckListModel**](classQfFeatureCheckListModel.md)) by allowing:
* Text-based filtering using a search term.
* Optional sorting that prioritizes checked items.
* Fuzzy and prefix-based matching for search refinement.




Used in UI contexts where users interact with large lists of features and need to quickly find and sort items by name or selection state. 


    
## Public Properties Documentation




### property addNull [1/2]

_Set to TRUE if null values are allowed in the list._ 
```C++
bool QfFeatureCheckListModel::addNull;
```




<hr>



### property allowMulti [1/2]

_The attribute field to have information about type (JSON/HSTORE) etc._ 
```C++
bool QfFeatureCheckListModel::allowMulti;
```




<hr>



### property appExpressionContextScopesGenerator [1/2]

_The application expression context scope generator used when filtering by expression._ 
```C++
QfAppExpressionContextScopesGenerator * QfFeatureCheckListModel::appExpressionContextScopesGenerator;
```




<hr>



### property attributeField [1/2]

_The attribute field to have information about type (JSON/HSTORE) etc._ 
```C++
QgsField QfFeatureCheckListModel::attributeField;
```




<hr>



### property attributeValue [1/2]

_The attribute value to generate checklist._ 
```C++
QVariant QfFeatureCheckListModel::attributeValue;
```




<hr>



### property currentFormFeature [1/2]

_The current form feature, used to evaluate expressions such as_  _current\_value('attr1_ _)`._
```C++
QgsFeature QfFeatureCheckListModel::currentFormFeature;
```




<hr>



### property currentLayer [1/2]

_The vector layer to list._ 
```C++
QgsVectorLayer * QfFeatureCheckListModel::currentLayer;
```




<hr>



### property displayGroupName [1/2]

_Set to TRUE if the group name will be displayed in the list._ 
```C++
bool QfFeatureCheckListModel::displayGroupName;
```




<hr>



### property displayValueField [1/2]

_The display value field._ 
```C++
QString QfFeatureCheckListModel::displayValueField;
```




<hr>



### property filterExpression [1/2]

_Expression to filter features with. Empty string if no filter is applied._ 
```C++
QString QfFeatureCheckListModel::filterExpression;
```




<hr>



### property groupField [1/2]

_The grouping key field._ 
```C++
QString QfFeatureCheckListModel::groupField;
```




<hr>



### property keyField [1/2]

_The primary key field._ 
```C++
QString QfFeatureCheckListModel::keyField;
```




<hr>



### property orderByField [1/2]

_Set to TRUE if features should be ordered by a field._ 
```C++
bool QfFeatureCheckListModel::orderByField;
```




<hr>



### property orderByFieldName [1/2]

_Field to sort features over._ 
```C++
QString QfFeatureCheckListModel::orderByFieldName;
```




<hr>



### property orderByValue [1/2]

_Set to TRUE if features should be ordered by value._ 
```C++
bool QfFeatureCheckListModel::orderByValue;
```




<hr>



### property searchTerm [1/2]

_The current search term used to filter items._ 
```C++
QString QfFeatureCheckListModel::searchTerm;
```




<hr>



### property sortCheckedFirst [1/2]

_The sorting method._ 
```C++
bool QfFeatureCheckListModel::sortCheckedFirst;
```




<hr>
## Public Signals Documentation




### signal addNullChanged 

```C++
void QfFeatureCheckListModel::addNullChanged;
```




<hr>



### signal allowMultiChanged 

```C++
void QfFeatureCheckListModel::allowMultiChanged;
```




<hr>



### signal appExpressionContextScopesGeneratorChanged 

```C++
void QfFeatureCheckListModel::appExpressionContextScopesGeneratorChanged;
```




<hr>



### signal attributeFieldChanged 

```C++
void QfFeatureCheckListModel::attributeFieldChanged;
```




<hr>



### signal attributeValueChanged 

```C++
void QfFeatureCheckListModel::attributeValueChanged;
```




<hr>



### signal currentFormFeatureChanged 

```C++
void QfFeatureCheckListModel::currentFormFeatureChanged;
```




<hr>



### signal currentLayerChanged 

```C++
void QfFeatureCheckListModel::currentLayerChanged;
```




<hr>



### signal displayGroupNameChanged 

```C++
void QfFeatureCheckListModel::displayGroupNameChanged;
```




<hr>



### signal displayValueFieldChanged 

```C++
void QfFeatureCheckListModel::displayValueFieldChanged;
```




<hr>



### signal filterExpressionChanged 

```C++
void QfFeatureCheckListModel::filterExpressionChanged;
```




<hr>



### signal groupFieldChanged 

```C++
void QfFeatureCheckListModel::groupFieldChanged;
```




<hr>



### signal keyFieldChanged 

```C++
void QfFeatureCheckListModel::keyFieldChanged;
```




<hr>



### signal listUpdated 

```C++
void QfFeatureCheckListModel::listUpdated;
```




<hr>



### signal orderByFieldChanged 

```C++
void QfFeatureCheckListModel::orderByFieldChanged;
```




<hr>



### signal orderByFieldNameChanged 

```C++
void QfFeatureCheckListModel::orderByFieldNameChanged;
```




<hr>



### signal orderByValueChanged 

```C++
void QfFeatureCheckListModel::orderByValueChanged;
```




<hr>



### signal searchTermChanged 

```C++
void QfFeatureCheckListModel::searchTermChanged;
```




<hr>



### signal sortCheckedFirstChanged 

```C++
void QfFeatureCheckListModel::sortCheckedFirstChanged;
```




<hr>
## Public Functions Documentation




### function QfFeatureCheckListModel 

```C++
explicit QfFeatureCheckListModel::QfFeatureCheckListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addNull [2/2]

```C++
bool QfFeatureCheckListModel::addNull () const
```



Add a NULL value as the first entry. 


        

<hr>



### function allowMulti [2/2]

```C++
bool QfFeatureCheckListModel::allowMulti () const
```



Whether multiple selected items are allowed 


        

<hr>



### function appExpressionContextScopesGenerator [2/2]

```C++
QfAppExpressionContextScopesGenerator * QfFeatureCheckListModel::appExpressionContextScopesGenerator () const
```



Returns the application expression context scope generator used when filtering by expression 


        

<hr>



### function attributeField [2/2]

```C++
QgsField QfFeatureCheckListModel::attributeField () const
```



the current attribute field 


        

<hr>



### function attributeValue [2/2]

```C++
QVariant QfFeatureCheckListModel::attributeValue () const
```



the attribute value. A QVariantList or an hstore formatted string, depending on the field type. 


        

<hr>



### function currentFormFeature [2/2]

```C++
QgsFeature QfFeatureCheckListModel::currentFormFeature () const
```



The current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### function currentLayer [2/2]

```C++
QgsVectorLayer * QfFeatureCheckListModel::currentLayer () const
```




<hr>



### function dataFromRowIndex 

```C++
inline Q_INVOKABLE QVariant QfFeatureCheckListModel::dataFromRowIndex (
    int row,
    int role
) 
```




<hr>



### function displayGroupName [2/2]

```C++
bool QfFeatureCheckListModel::displayGroupName () const
```




<hr>



### function displayValueField [2/2]

```C++
QString QfFeatureCheckListModel::displayValueField () const
```




<hr>



### function filterExpression [2/2]

```C++
QString QfFeatureCheckListModel::filterExpression () const
```



Expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### function findDisplayValueMatches 

```C++
Q_INVOKABLE QList< int > QfFeatureCheckListModel::findDisplayValueMatches (
    const QString & filter
) const
```



Get rows for a given filter string used to match display values. 


        

<hr>



### function findKey 

```C++
Q_INVOKABLE int QfFeatureCheckListModel::findKey (
    const QVariant & key
) const
```



Get the row for a given key value. 


        

<hr>



### function getFeatureById 

```C++
Q_INVOKABLE QgsFeature QfFeatureCheckListModel::getFeatureById (
    QgsFeatureId id
) const
```



Returns the first feature matching the feature _id_. 


        

<hr>



### function getFeatureFromKeyValue 

```C++
Q_INVOKABLE QgsFeature QfFeatureCheckListModel::getFeatureFromKeyValue (
    const QVariant & value
) const
```



Returns the first feature matching the key _value_. 


        

<hr>



### function groupField [2/2]

```C++
QString QfFeatureCheckListModel::groupField () const
```




<hr>



### function keyField [2/2]

```C++
QString QfFeatureCheckListModel::keyField () const
```




<hr>



### function orderByField [2/2]

```C++
bool QfFeatureCheckListModel::orderByField () const
```



Orders all the values by a field. 


        

<hr>



### function orderByFieldName [2/2]

```C++
QString QfFeatureCheckListModel::orderByFieldName () const
```



Orders all the values by a field. 


        

<hr>



### function orderByValue [2/2]

```C++
bool QfFeatureCheckListModel::orderByValue () const
```



Orders all the values alphabethically by their displayString. 


        

<hr>



### function rowCount 

```C++
Q_INVOKABLE int QfFeatureCheckListModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function searchTerm [2/2]

```C++
QString QfFeatureCheckListModel::searchTerm () const
```



Returns the current search term used to filter items. 


        

<hr>



### function setAddNull 

```C++
void QfFeatureCheckListModel::setAddNull (
    bool addNull
) 
```



Add a NULL value as the first entry. 


        

<hr>



### function setAllowMulti 

```C++
void QfFeatureCheckListModel::setAllowMulti (
    bool allowMulti
) 
```



Sets whether multiple selected items are allowed 


        

<hr>



### function setAppExpressionContextScopesGenerator 

```C++
void QfFeatureCheckListModel::setAppExpressionContextScopesGenerator (
    QfAppExpressionContextScopesGenerator * generator
) 
```



Sets the application expression context scope generator used when filtering by expression 


        

<hr>



### function setAttributeField 

```C++
void QfFeatureCheckListModel::setAttributeField (
    const QgsField & field
) 
```



the current attribute field 


        

<hr>



### function setAttributeValue 

```C++
void QfFeatureCheckListModel::setAttributeValue (
    const QVariant & attributeValue
) 
```



the attribute value. A QVariantList or an hstore formatted string, depending on the field type. 


        

<hr>



### function setCurrentFormFeature 

```C++
void QfFeatureCheckListModel::setCurrentFormFeature (
    const QgsFeature & feature
) 
```



Sets the current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### function setCurrentLayer 

```C++
void QfFeatureCheckListModel::setCurrentLayer (
    QgsVectorLayer * currentLayer
) 
```




<hr>



### function setDisplayGroupName 

```C++
void QfFeatureCheckListModel::setDisplayGroupName (
    bool displayGroupName
) 
```




<hr>



### function setDisplayValueField 

```C++
void QfFeatureCheckListModel::setDisplayValueField (
    const QString & displayValueField
) 
```




<hr>



### function setFilterExpression 

```C++
void QfFeatureCheckListModel::setFilterExpression (
    const QString & filterExpression
) 
```



Sets an expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### function setGroupField 

```C++
void QfFeatureCheckListModel::setGroupField (
    const QString & groupField
) 
```




<hr>



### function setKeyField 

```C++
void QfFeatureCheckListModel::setKeyField (
    const QString & keyField
) 
```




<hr>



### function setOrderByField 

```C++
void QfFeatureCheckListModel::setOrderByField (
    bool orderByField
) 
```



Orders all the values by a field. 


        

<hr>



### function setOrderByFieldName 

```C++
void QfFeatureCheckListModel::setOrderByFieldName (
    const QString & orderByFieldName
) 
```



Orders all the values by a field. 


        

<hr>



### function setOrderByValue 

```C++
void QfFeatureCheckListModel::setOrderByValue (
    bool orderByValue
) 
```



Orders all the values alphabethically by their displayString. 


        

<hr>



### function setSearchTerm 

```C++
void QfFeatureCheckListModel::setSearchTerm (
    const QString & searchTerm
) 
```



Sets the search term for filtering and updates the filter. 


        

<hr>



### function setSortCheckedFirst 

```C++
void QfFeatureCheckListModel::setSortCheckedFirst (
    bool enabled
) 
```



Enables or disables sorting checked items first. 


        

<hr>



### function sortCheckedFirst [2/2]

```C++
bool QfFeatureCheckListModel::sortCheckedFirst () const
```



Returns whether checked items are sorted to the top. 


        

<hr>



### function toggleCheckAll 

```C++
Q_INVOKABLE void QfFeatureCheckListModel::toggleCheckAll (
    bool toggleChecked=true
) 
```



Sets all element to either checked or unchecked state, depending on _toggleChecked_ 


        

<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
bool QfFeatureCheckListModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function lessThan 

```C++
bool QfFeatureCheckListModel::lessThan (
    const QModelIndex & left,
    const QModelIndex & right
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qffeaturechecklistmodel.h`

