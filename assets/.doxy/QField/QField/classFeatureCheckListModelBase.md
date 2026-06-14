

# Class FeatureCheckListModelBase



[**ClassList**](annotated.md) **>** [**FeatureCheckListModelBase**](classFeatureCheckListModelBase.md)








Inherits the following classes: [FeatureListModel](classFeatureListModel.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureListRoles**](#enum-featurelistroles)  <br> |


## Public Types inherited from FeatureListModel

See [FeatureListModel](classFeatureListModel.md)

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureListRoles**](classFeatureListModel.md#enum-featurelistroles)  <br> |
























## Public Properties inherited from FeatureListModel

See [FeatureListModel](classFeatureListModel.md)

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
| signal void | [**allowMultiChanged**](classFeatureCheckListModelBase.md#signal-allowmultichanged)  <br> |
| signal void | [**attributeFieldChanged**](classFeatureCheckListModelBase.md#signal-attributefieldchanged)  <br> |
| signal void | [**attributeValueChanged**](classFeatureCheckListModelBase.md#signal-attributevaluechanged)  <br> |
| signal void | [**listUpdated**](classFeatureCheckListModelBase.md#signal-listupdated)  <br> |


## Public Signals inherited from FeatureListModel

See [FeatureListModel](classFeatureListModel.md)

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
|   | [**FeatureCheckListModelBase**](#function-featurechecklistmodelbase) (QObject \* parent=nullptr) <br> |
|  bool | [**allowMulti**](#function-allowmulti) () const<br> |
|  QgsField | [**attributeField**](#function-attributefield) () const<br> |
|  QVariant | [**attributeValue**](#function-attributevalue) () const<br> |
| virtual QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
| virtual QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  void | [**setAllowMulti**](#function-setallowmulti) (bool allowMulti) <br> |
|  void | [**setAttributeField**](#function-setattributefield) (const QgsField & field) <br> |
|  void | [**setAttributeValue**](#function-setattributevalue) (const QVariant & attributeValue) <br> |
| virtual bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role) override<br> |
|  Q\_INVOKABLE void | [**toggleCheckAll**](#function-togglecheckall) (bool toggleChecked=true) <br> |


## Public Functions inherited from FeatureListModel

See [FeatureListModel](classFeatureListModel.md)

| Type | Name |
| ---: | :--- |
|   | [**FeatureListModel**](classFeatureListModel.md#function-featurelistmodel) (QObject \* parent=nullptr) <br> |
|  bool | [**addNull**](classFeatureListModel.md#function-addnull-22) () const<br> |
|  [**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classFeatureListModel.md#function-appexpressioncontextscopesgenerator-22) () const<br> |
| virtual int | [**columnCount**](classFeatureListModel.md#function-columncount) (const QModelIndex & parent) override const<br> |
|  QgsFeature | [**currentFormFeature**](classFeatureListModel.md#function-currentformfeature-22) () const<br> |
|  QgsVectorLayer \* | [**currentLayer**](classFeatureListModel.md#function-currentlayer-22) () const<br> |
| virtual QVariant | [**data**](classFeatureListModel.md#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE QVariant | [**dataFromRowIndex**](classFeatureListModel.md#function-datafromrowindex) (int row, int role) <br> |
|  bool | [**displayGroupName**](classFeatureListModel.md#function-displaygroupname-22) () const<br> |
|  QString | [**displayValueField**](classFeatureListModel.md#function-displayvaluefield-22) () const<br> |
|  QString | [**filterExpression**](classFeatureListModel.md#function-filterexpression-22) () const<br> |
|  Q\_INVOKABLE QList&lt; int &gt; | [**findDisplayValueMatches**](classFeatureListModel.md#function-finddisplayvaluematches) (const QString & filter) const<br> |
|  Q\_INVOKABLE int | [**findKey**](classFeatureListModel.md#function-findkey) (const QVariant & key) const<br> |
|  Q\_INVOKABLE QgsFeature | [**getFeatureById**](classFeatureListModel.md#function-getfeaturebyid) (QgsFeatureId id) const<br> |
|  Q\_INVOKABLE QgsFeature | [**getFeatureFromKeyValue**](classFeatureListModel.md#function-getfeaturefromkeyvalue) (const QVariant & value) const<br> |
|  QString | [**groupField**](classFeatureListModel.md#function-groupfield-22) () const<br> |
| virtual QModelIndex | [**index**](classFeatureListModel.md#function-index) (int row, int column, const QModelIndex & parent) override const<br> |
|  QString | [**keyField**](classFeatureListModel.md#function-keyfield-22) () const<br> |
|  bool | [**orderByValue**](classFeatureListModel.md#function-orderbyvalue-22) () const<br> |
| virtual QModelIndex | [**parent**](classFeatureListModel.md#function-parent) (const QModelIndex & child) override const<br> |
| virtual QHash&lt; int, QByteArray &gt; | [**roleNames**](classFeatureListModel.md#function-rolenames) () override const<br> |
| virtual int | [**rowCount**](classFeatureListModel.md#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  QString | [**searchTerm**](classFeatureListModel.md#function-searchterm-22) () const<br> |
|  void | [**setAddNull**](classFeatureListModel.md#function-setaddnull) (bool addNull) <br> |
|  void | [**setAppExpressionContextScopesGenerator**](classFeatureListModel.md#function-setappexpressioncontextscopesgenerator) ([**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* generator) <br> |
|  void | [**setCurrentFormFeature**](classFeatureListModel.md#function-setcurrentformfeature) (const QgsFeature & feature) <br> |
|  void | [**setCurrentLayer**](classFeatureListModel.md#function-setcurrentlayer) (QgsVectorLayer \* currentLayer) <br> |
|  void | [**setDisplayGroupName**](classFeatureListModel.md#function-setdisplaygroupname) (bool displayGroupName) <br> |
|  void | [**setDisplayValueField**](classFeatureListModel.md#function-setdisplayvaluefield) (const QString & displayValueField) <br> |
|  void | [**setFilterExpression**](classFeatureListModel.md#function-setfilterexpression) (const QString & filterExpression) <br> |
|  void | [**setGroupField**](classFeatureListModel.md#function-setgroupfield) (const QString & groupField) <br> |
|  void | [**setKeyField**](classFeatureListModel.md#function-setkeyfield) (const QString & keyField) <br> |
|  void | [**setOrderByValue**](classFeatureListModel.md#function-setorderbyvalue) (bool orderByValue) <br> |
|  void | [**setSearchTerm**](classFeatureListModel.md#function-setsearchterm) (const QString & searchTerm) <br> |
|   | [**~FeatureListModel**](classFeatureListModel.md#function-featurelistmodel) () <br> |






















































## Public Types Documentation




### enum FeatureListRoles 

```C++
enum FeatureCheckListModelBase::FeatureListRoles {
    CheckedRole = Qt::UserRole + 100
};
```




<hr>
## Public Signals Documentation




### signal allowMultiChanged 

```C++
void FeatureCheckListModelBase::allowMultiChanged;
```




<hr>



### signal attributeFieldChanged 

```C++
void FeatureCheckListModelBase::attributeFieldChanged;
```




<hr>



### signal attributeValueChanged 

```C++
void FeatureCheckListModelBase::attributeValueChanged;
```




<hr>



### signal listUpdated 

```C++
void FeatureCheckListModelBase::listUpdated;
```




<hr>
## Public Functions Documentation




### function FeatureCheckListModelBase 

```C++
explicit FeatureCheckListModelBase::FeatureCheckListModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function allowMulti 

```C++
bool FeatureCheckListModelBase::allowMulti () const
```



Whether multiple selected items are allowed 


        

<hr>



### function attributeField 

```C++
QgsField FeatureCheckListModelBase::attributeField () const
```



the current attribute field 


        

<hr>



### function attributeValue 

```C++
QVariant FeatureCheckListModelBase::attributeValue () const
```



the attribute value. A QVariantList or an hstore formatted string, depending on the field type. 


        

<hr>



### function data 

```C++
virtual QVariant FeatureCheckListModelBase::data (
    const QModelIndex & index,
    int role
) override const
```



Implements [*FeatureListModel::data*](classFeatureListModel.md#function-data)


<hr>



### function roleNames 

```C++
virtual QHash< int, QByteArray > FeatureCheckListModelBase::roleNames () override const
```



Implements [*FeatureListModel::roleNames*](classFeatureListModel.md#function-rolenames)


<hr>



### function setAllowMulti 

```C++
void FeatureCheckListModelBase::setAllowMulti (
    bool allowMulti
) 
```



Sets whether multiple selected items are allowed 


        

<hr>



### function setAttributeField 

```C++
void FeatureCheckListModelBase::setAttributeField (
    const QgsField & field
) 
```



the current attribute field 


        

<hr>



### function setAttributeValue 

```C++
void FeatureCheckListModelBase::setAttributeValue (
    const QVariant & attributeValue
) 
```



the attribute value. A QVariantList or an hstore formatted string, depending on the field type. 


        

<hr>



### function setData 

```C++
virtual bool FeatureCheckListModelBase::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```




<hr>



### function toggleCheckAll 

```C++
Q_INVOKABLE void FeatureCheckListModelBase::toggleCheckAll (
    bool toggleChecked=true
) 
```



Sets all element to either checked or unchecked state, depending on _toggleChecked_ 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featurechecklistmodel.h`

