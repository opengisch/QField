

# Class FeatureCheckListModel



[**ClassList**](annotated.md) **>** [**FeatureCheckListModel**](classFeatureCheckListModel.md)



[More...](#detailed-description)

* `#include <featurechecklistmodel.h>`



Inherits the following classes: QSortFilterProxyModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**addNull**](classFeatureCheckListModel.md#property-addnull-12)  <br>_Set to TRUE if null values are allowed in the list._  |
| property bool | [**allowMulti**](classFeatureCheckListModel.md#property-allowmulti-12)  <br>_The attribute field to have information about type (JSON/HSTORE) etc._  |
| property [**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classFeatureCheckListModel.md#property-appexpressioncontextscopesgenerator-12)  <br>_The application expression context scope generator used when filtering by expression._  |
| property QgsField | [**attributeField**](classFeatureCheckListModel.md#property-attributefield-12)  <br>_The attribute field to have information about type (JSON/HSTORE) etc._  |
| property QVariant | [**attributeValue**](classFeatureCheckListModel.md#property-attributevalue-12)  <br>_The attribute value to generate checklist._  |
| property QgsFeature | [**currentFormFeature**](classFeatureCheckListModel.md#property-currentformfeature-12)  <br>_The current form feature, used to evaluate expressions such as_  _current\_value('attr1_ _)`._ |
| property QgsVectorLayer \* | [**currentLayer**](classFeatureCheckListModel.md#property-currentlayer-12)  <br>_The vector layer to list._  |
| property bool | [**displayGroupName**](classFeatureCheckListModel.md#property-displaygroupname-12)  <br>_Set to TRUE if the group name will be displayed in the list._  |
| property QString | [**displayValueField**](classFeatureCheckListModel.md#property-displayvaluefield-12)  <br>_The display value field._  |
| property QString | [**filterExpression**](classFeatureCheckListModel.md#property-filterexpression-12)  <br>_Expression to filter features with. Empty string if no filter is applied._  |
| property QString | [**groupField**](classFeatureCheckListModel.md#property-groupfield-12)  <br>_The grouping key field._  |
| property QString | [**keyField**](classFeatureCheckListModel.md#property-keyfield-12)  <br>_The primary key field._  |
| property bool | [**orderByValue**](classFeatureCheckListModel.md#property-orderbyvalue-12)  <br>_Set to TRUE if features should be ordered by value._  |
| property QString | [**searchTerm**](classFeatureCheckListModel.md#property-searchterm-12)  <br>_The current search term used to filter items._  |
| property bool | [**sortCheckedFirst**](classFeatureCheckListModel.md#property-sortcheckedfirst-12)  <br>_The sorting method._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**addNullChanged**](classFeatureCheckListModel.md#signal-addnullchanged)  <br> |
| signal void | [**allowMultiChanged**](classFeatureCheckListModel.md#signal-allowmultichanged)  <br> |
| signal void | [**appExpressionContextScopesGeneratorChanged**](classFeatureCheckListModel.md#signal-appexpressioncontextscopesgeneratorchanged)  <br> |
| signal void | [**attributeFieldChanged**](classFeatureCheckListModel.md#signal-attributefieldchanged)  <br> |
| signal void | [**attributeValueChanged**](classFeatureCheckListModel.md#signal-attributevaluechanged)  <br> |
| signal void | [**currentFormFeatureChanged**](classFeatureCheckListModel.md#signal-currentformfeaturechanged)  <br> |
| signal void | [**currentLayerChanged**](classFeatureCheckListModel.md#signal-currentlayerchanged)  <br> |
| signal void | [**displayGroupNameChanged**](classFeatureCheckListModel.md#signal-displaygroupnamechanged)  <br> |
| signal void | [**displayValueFieldChanged**](classFeatureCheckListModel.md#signal-displayvaluefieldchanged)  <br> |
| signal void | [**filterExpressionChanged**](classFeatureCheckListModel.md#signal-filterexpressionchanged)  <br> |
| signal void | [**groupFieldChanged**](classFeatureCheckListModel.md#signal-groupfieldchanged)  <br> |
| signal void | [**keyFieldChanged**](classFeatureCheckListModel.md#signal-keyfieldchanged)  <br> |
| signal void | [**listUpdated**](classFeatureCheckListModel.md#signal-listupdated)  <br> |
| signal void | [**orderByValueChanged**](classFeatureCheckListModel.md#signal-orderbyvaluechanged)  <br> |
| signal void | [**searchTermChanged**](classFeatureCheckListModel.md#signal-searchtermchanged)  <br> |
| signal void | [**sortCheckedFirstChanged**](classFeatureCheckListModel.md#signal-sortcheckedfirstchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeatureCheckListModel**](#function-featurechecklistmodel) (QObject \* parent=nullptr) <br> |
|  bool | [**addNull**](#function-addnull-22) () const<br> |
|  bool | [**allowMulti**](#function-allowmulti-22) () const<br> |
|  [**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](#function-appexpressioncontextscopesgenerator-22) () const<br> |
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
|  bool | [**orderByValue**](#function-orderbyvalue-22) () const<br> |
|  QString | [**searchTerm**](#function-searchterm-22) () const<br> |
|  void | [**setAddNull**](#function-setaddnull) (bool addNull) <br> |
|  void | [**setAllowMulti**](#function-setallowmulti) (bool allowMulti) <br> |
|  void | [**setAppExpressionContextScopesGenerator**](#function-setappexpressioncontextscopesgenerator) ([**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* generator) <br> |
|  void | [**setAttributeField**](#function-setattributefield) (const QgsField & field) <br> |
|  void | [**setAttributeValue**](#function-setattributevalue) (const QVariant & attributeValue) <br> |
|  void | [**setCurrentFormFeature**](#function-setcurrentformfeature) (const QgsFeature & feature) <br> |
|  void | [**setCurrentLayer**](#function-setcurrentlayer) (QgsVectorLayer \* currentLayer) <br> |
|  void | [**setDisplayGroupName**](#function-setdisplaygroupname) (bool displayGroupName) <br> |
|  void | [**setDisplayValueField**](#function-setdisplayvaluefield) (const QString & displayValueField) <br> |
|  void | [**setFilterExpression**](#function-setfilterexpression) (const QString & filterExpression) <br> |
|  void | [**setGroupField**](#function-setgroupfield) (const QString & groupField) <br> |
|  void | [**setKeyField**](#function-setkeyfield) (const QString & keyField) <br> |
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


This model enhances a source model (typically a [**FeatureCheckListModel**](classFeatureCheckListModel.md)) by allowing:
* Text-based filtering using a search term.
* Optional sorting that prioritizes checked items.
* Fuzzy and prefix-based matching for search refinement.




Used in UI contexts where users interact with large lists of features and need to quickly find and sort items by name or selection state. 


    
## Public Properties Documentation




### property addNull [1/2]

_Set to TRUE if null values are allowed in the list._ 
```C++
bool FeatureCheckListModel::addNull;
```




<hr>



### property allowMulti [1/2]

_The attribute field to have information about type (JSON/HSTORE) etc._ 
```C++
bool FeatureCheckListModel::allowMulti;
```




<hr>



### property appExpressionContextScopesGenerator [1/2]

_The application expression context scope generator used when filtering by expression._ 
```C++
AppExpressionContextScopesGenerator * FeatureCheckListModel::appExpressionContextScopesGenerator;
```




<hr>



### property attributeField [1/2]

_The attribute field to have information about type (JSON/HSTORE) etc._ 
```C++
QgsField FeatureCheckListModel::attributeField;
```




<hr>



### property attributeValue [1/2]

_The attribute value to generate checklist._ 
```C++
QVariant FeatureCheckListModel::attributeValue;
```




<hr>



### property currentFormFeature [1/2]

_The current form feature, used to evaluate expressions such as_  _current\_value('attr1_ _)`._
```C++
QgsFeature FeatureCheckListModel::currentFormFeature;
```




<hr>



### property currentLayer [1/2]

_The vector layer to list._ 
```C++
QgsVectorLayer * FeatureCheckListModel::currentLayer;
```




<hr>



### property displayGroupName [1/2]

_Set to TRUE if the group name will be displayed in the list._ 
```C++
bool FeatureCheckListModel::displayGroupName;
```




<hr>



### property displayValueField [1/2]

_The display value field._ 
```C++
QString FeatureCheckListModel::displayValueField;
```




<hr>



### property filterExpression [1/2]

_Expression to filter features with. Empty string if no filter is applied._ 
```C++
QString FeatureCheckListModel::filterExpression;
```




<hr>



### property groupField [1/2]

_The grouping key field._ 
```C++
QString FeatureCheckListModel::groupField;
```




<hr>



### property keyField [1/2]

_The primary key field._ 
```C++
QString FeatureCheckListModel::keyField;
```




<hr>



### property orderByValue [1/2]

_Set to TRUE if features should be ordered by value._ 
```C++
bool FeatureCheckListModel::orderByValue;
```




<hr>



### property searchTerm [1/2]

_The current search term used to filter items._ 
```C++
QString FeatureCheckListModel::searchTerm;
```




<hr>



### property sortCheckedFirst [1/2]

_The sorting method._ 
```C++
bool FeatureCheckListModel::sortCheckedFirst;
```




<hr>
## Public Signals Documentation




### signal addNullChanged 

```C++
void FeatureCheckListModel::addNullChanged;
```




<hr>



### signal allowMultiChanged 

```C++
void FeatureCheckListModel::allowMultiChanged;
```




<hr>



### signal appExpressionContextScopesGeneratorChanged 

```C++
void FeatureCheckListModel::appExpressionContextScopesGeneratorChanged;
```




<hr>



### signal attributeFieldChanged 

```C++
void FeatureCheckListModel::attributeFieldChanged;
```




<hr>



### signal attributeValueChanged 

```C++
void FeatureCheckListModel::attributeValueChanged;
```




<hr>



### signal currentFormFeatureChanged 

```C++
void FeatureCheckListModel::currentFormFeatureChanged;
```




<hr>



### signal currentLayerChanged 

```C++
void FeatureCheckListModel::currentLayerChanged;
```




<hr>



### signal displayGroupNameChanged 

```C++
void FeatureCheckListModel::displayGroupNameChanged;
```




<hr>



### signal displayValueFieldChanged 

```C++
void FeatureCheckListModel::displayValueFieldChanged;
```




<hr>



### signal filterExpressionChanged 

```C++
void FeatureCheckListModel::filterExpressionChanged;
```




<hr>



### signal groupFieldChanged 

```C++
void FeatureCheckListModel::groupFieldChanged;
```




<hr>



### signal keyFieldChanged 

```C++
void FeatureCheckListModel::keyFieldChanged;
```




<hr>



### signal listUpdated 

```C++
void FeatureCheckListModel::listUpdated;
```




<hr>



### signal orderByValueChanged 

```C++
void FeatureCheckListModel::orderByValueChanged;
```




<hr>



### signal searchTermChanged 

```C++
void FeatureCheckListModel::searchTermChanged;
```




<hr>



### signal sortCheckedFirstChanged 

```C++
void FeatureCheckListModel::sortCheckedFirstChanged;
```




<hr>
## Public Functions Documentation




### function FeatureCheckListModel 

```C++
explicit FeatureCheckListModel::FeatureCheckListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addNull [2/2]

```C++
bool FeatureCheckListModel::addNull () const
```



Add a NULL value as the first entry. 


        

<hr>



### function allowMulti [2/2]

```C++
bool FeatureCheckListModel::allowMulti () const
```



Whether multiple selected items are allowed 


        

<hr>



### function appExpressionContextScopesGenerator [2/2]

```C++
AppExpressionContextScopesGenerator * FeatureCheckListModel::appExpressionContextScopesGenerator () const
```



Returns the application expression context scope generator used when filtering by expression 


        

<hr>



### function attributeField [2/2]

```C++
QgsField FeatureCheckListModel::attributeField () const
```



the current attribute field 


        

<hr>



### function attributeValue [2/2]

```C++
QVariant FeatureCheckListModel::attributeValue () const
```



the attribute value. A QVariantList or an hstore formatted string, depending on the field type. 


        

<hr>



### function currentFormFeature [2/2]

```C++
QgsFeature FeatureCheckListModel::currentFormFeature () const
```



The current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### function currentLayer [2/2]

```C++
QgsVectorLayer * FeatureCheckListModel::currentLayer () const
```




<hr>



### function dataFromRowIndex 

```C++
inline Q_INVOKABLE QVariant FeatureCheckListModel::dataFromRowIndex (
    int row,
    int role
) 
```




<hr>



### function displayGroupName [2/2]

```C++
bool FeatureCheckListModel::displayGroupName () const
```




<hr>



### function displayValueField [2/2]

```C++
QString FeatureCheckListModel::displayValueField () const
```




<hr>



### function filterExpression [2/2]

```C++
QString FeatureCheckListModel::filterExpression () const
```



Expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### function findDisplayValueMatches 

```C++
Q_INVOKABLE QList< int > FeatureCheckListModel::findDisplayValueMatches (
    const QString & filter
) const
```



Get rows for a given filter string used to match display values. 


        

<hr>



### function findKey 

```C++
Q_INVOKABLE int FeatureCheckListModel::findKey (
    const QVariant & key
) const
```



Get the row for a given key value. 


        

<hr>



### function getFeatureById 

```C++
Q_INVOKABLE QgsFeature FeatureCheckListModel::getFeatureById (
    QgsFeatureId id
) const
```



Returns the first feature matching the feature _id_. 


        

<hr>



### function getFeatureFromKeyValue 

```C++
Q_INVOKABLE QgsFeature FeatureCheckListModel::getFeatureFromKeyValue (
    const QVariant & value
) const
```



Returns the first feature matching the key _value_. 


        

<hr>



### function groupField [2/2]

```C++
QString FeatureCheckListModel::groupField () const
```




<hr>



### function keyField [2/2]

```C++
QString FeatureCheckListModel::keyField () const
```




<hr>



### function orderByValue [2/2]

```C++
bool FeatureCheckListModel::orderByValue () const
```



Orders all the values alphabethically by their displayString. 


        

<hr>



### function searchTerm [2/2]

```C++
QString FeatureCheckListModel::searchTerm () const
```



Returns the current search term used to filter items. 


        

<hr>



### function setAddNull 

```C++
void FeatureCheckListModel::setAddNull (
    bool addNull
) 
```



Add a NULL value as the first entry. 


        

<hr>



### function setAllowMulti 

```C++
void FeatureCheckListModel::setAllowMulti (
    bool allowMulti
) 
```



Sets whether multiple selected items are allowed 


        

<hr>



### function setAppExpressionContextScopesGenerator 

```C++
void FeatureCheckListModel::setAppExpressionContextScopesGenerator (
    AppExpressionContextScopesGenerator * generator
) 
```



Sets the application expression context scope generator used when filtering by expression 


        

<hr>



### function setAttributeField 

```C++
void FeatureCheckListModel::setAttributeField (
    const QgsField & field
) 
```



the current attribute field 


        

<hr>



### function setAttributeValue 

```C++
void FeatureCheckListModel::setAttributeValue (
    const QVariant & attributeValue
) 
```



the attribute value. A QVariantList or an hstore formatted string, depending on the field type. 


        

<hr>



### function setCurrentFormFeature 

```C++
void FeatureCheckListModel::setCurrentFormFeature (
    const QgsFeature & feature
) 
```



Sets the current form feature, used to evaluate expressions such as current\_value('attr1)` 


        

<hr>



### function setCurrentLayer 

```C++
void FeatureCheckListModel::setCurrentLayer (
    QgsVectorLayer * currentLayer
) 
```




<hr>



### function setDisplayGroupName 

```C++
void FeatureCheckListModel::setDisplayGroupName (
    bool displayGroupName
) 
```




<hr>



### function setDisplayValueField 

```C++
void FeatureCheckListModel::setDisplayValueField (
    const QString & displayValueField
) 
```




<hr>



### function setFilterExpression 

```C++
void FeatureCheckListModel::setFilterExpression (
    const QString & filterExpression
) 
```



Sets an expression to filter features with. Empty string if no filter is applied. 


        

<hr>



### function setGroupField 

```C++
void FeatureCheckListModel::setGroupField (
    const QString & groupField
) 
```




<hr>



### function setKeyField 

```C++
void FeatureCheckListModel::setKeyField (
    const QString & keyField
) 
```




<hr>



### function setOrderByValue 

```C++
void FeatureCheckListModel::setOrderByValue (
    bool orderByValue
) 
```



Orders all the values alphabethically by their displayString. 


        

<hr>



### function setSearchTerm 

```C++
void FeatureCheckListModel::setSearchTerm (
    const QString & searchTerm
) 
```



Sets the search term for filtering and updates the filter. 


        

<hr>



### function setSortCheckedFirst 

```C++
void FeatureCheckListModel::setSortCheckedFirst (
    bool enabled
) 
```



Enables or disables sorting checked items first. 


        

<hr>



### function sortCheckedFirst [2/2]

```C++
bool FeatureCheckListModel::sortCheckedFirst () const
```



Returns whether checked items are sorted to the top. 


        

<hr>



### function toggleCheckAll 

```C++
Q_INVOKABLE void FeatureCheckListModel::toggleCheckAll (
    bool toggleChecked=true
) 
```



Sets all element to either checked or unchecked state, depending on _toggleChecked_ 


        

<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
bool FeatureCheckListModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```



Determines whether a row should be accepted based on the current filter settings. Checks if the data in the row matches the current search term. 


        

<hr>



### function lessThan 

```C++
bool FeatureCheckListModel::lessThan (
    const QModelIndex & left,
    const QModelIndex & right
) override const
```



Compares two items for sorting in the feature check list.


Sorting priority:
* Null placeholder (if [**addNull()**](classFeatureCheckListModel.md#property-addnull-12) is enabled).
* Grouping by [**groupField()**](classFeatureCheckListModel.md#property-groupfield-12) (empty groups first, then alphabetically).
* Checked-first (if mSortCheckedFirst is enabled and no search term).
* Search relevance (startsWith + fuzzy score if search term is active).
* Value-based alphabetical ordering (if [**orderByValue()**](classFeatureCheckListModel.md#property-orderbyvalue-12) is enabled).
* Key-based fallback (empty keys first, then alphabetically by key).






**Parameters:**


* `left` The model index of the left item. 
* `right` The model index of the right item. 



**Returns:**

True if the left item should appear before the right item. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featurechecklistmodel.h`

