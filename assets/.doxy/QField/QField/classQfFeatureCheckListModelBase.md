

# Class QfFeatureCheckListModelBase



[**ClassList**](annotated.md) **>** [**QfFeatureCheckListModelBase**](classQfFeatureCheckListModelBase.md)








Inherits the following classes: [QfFeatureListModel](classQfFeatureListModel.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureListRoles**](#enum-featurelistroles)  <br> |


## Public Types inherited from QfFeatureListModel

See [QfFeatureListModel](classQfFeatureListModel.md)

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureListRoles**](classQfFeatureListModel.md#enum-featurelistroles)  <br> |
























## Public Properties inherited from QfFeatureListModel

See [QfFeatureListModel](classQfFeatureListModel.md)

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
| signal void | [**allowMultiChanged**](classQfFeatureCheckListModelBase.md#signal-allowmultichanged)  <br> |
| signal void | [**attributeFieldChanged**](classQfFeatureCheckListModelBase.md#signal-attributefieldchanged)  <br> |
| signal void | [**attributeValueChanged**](classQfFeatureCheckListModelBase.md#signal-attributevaluechanged)  <br> |
| signal void | [**listUpdated**](classQfFeatureCheckListModelBase.md#signal-listupdated)  <br> |


## Public Signals inherited from QfFeatureListModel

See [QfFeatureListModel](classQfFeatureListModel.md)

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
|   | [**QfFeatureCheckListModelBase**](#function-qffeaturechecklistmodelbase) (QObject \* parent=nullptr) <br> |
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


## Public Functions inherited from QfFeatureListModel

See [QfFeatureListModel](classQfFeatureListModel.md)

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureListModel**](classQfFeatureListModel.md#function-qffeaturelistmodel) (QObject \* parent=nullptr) <br> |
|  bool | [**addNull**](classQfFeatureListModel.md#function-addnull-22) () const<br> |
|  [**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classQfFeatureListModel.md#function-appexpressioncontextscopesgenerator-22) () const<br> |
| virtual int | [**columnCount**](classQfFeatureListModel.md#function-columncount) (const QModelIndex & parent) override const<br> |
|  QgsFeature | [**currentFormFeature**](classQfFeatureListModel.md#function-currentformfeature-22) () const<br> |
|  QgsVectorLayer \* | [**currentLayer**](classQfFeatureListModel.md#function-currentlayer-22) () const<br> |
| virtual QVariant | [**data**](classQfFeatureListModel.md#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE QVariant | [**dataFromRowIndex**](classQfFeatureListModel.md#function-datafromrowindex) (int row, int role) <br> |
|  bool | [**displayGroupName**](classQfFeatureListModel.md#function-displaygroupname-22) () const<br> |
|  QString | [**displayValueField**](classQfFeatureListModel.md#function-displayvaluefield-22) () const<br> |
|  QString | [**filterExpression**](classQfFeatureListModel.md#function-filterexpression-22) () const<br> |
|  Q\_INVOKABLE QList&lt; int &gt; | [**findDisplayValueMatches**](classQfFeatureListModel.md#function-finddisplayvaluematches) (const QString & filter) const<br> |
|  Q\_INVOKABLE int | [**findKey**](classQfFeatureListModel.md#function-findkey) (const QVariant & key) const<br> |
|  Q\_INVOKABLE QgsFeature | [**getFeatureById**](classQfFeatureListModel.md#function-getfeaturebyid) (QgsFeatureId id) const<br> |
|  Q\_INVOKABLE QgsFeature | [**getFeatureFromKeyValue**](classQfFeatureListModel.md#function-getfeaturefromkeyvalue) (const QVariant & value) const<br> |
|  QString | [**groupField**](classQfFeatureListModel.md#function-groupfield-22) () const<br> |
| virtual QModelIndex | [**index**](classQfFeatureListModel.md#function-index) (int row, int column, const QModelIndex & parent) override const<br> |
|  QString | [**keyField**](classQfFeatureListModel.md#function-keyfield-22) () const<br> |
|  bool | [**orderByField**](classQfFeatureListModel.md#function-orderbyfield-22) () const<br> |
|  QString | [**orderByFieldName**](classQfFeatureListModel.md#function-orderbyfieldname-22) () const<br> |
|  bool | [**orderByValue**](classQfFeatureListModel.md#function-orderbyvalue-22) () const<br> |
| virtual QModelIndex | [**parent**](classQfFeatureListModel.md#function-parent) (const QModelIndex & child) override const<br> |
| virtual QHash&lt; int, QByteArray &gt; | [**roleNames**](classQfFeatureListModel.md#function-rolenames) () override const<br> |
| virtual int | [**rowCount**](classQfFeatureListModel.md#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  QString | [**searchTerm**](classQfFeatureListModel.md#function-searchterm-22) () const<br> |
|  void | [**setAddNull**](classQfFeatureListModel.md#function-setaddnull) (bool addNull) <br> |
|  void | [**setAppExpressionContextScopesGenerator**](classQfFeatureListModel.md#function-setappexpressioncontextscopesgenerator) ([**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* generator) <br> |
|  void | [**setCurrentFormFeature**](classQfFeatureListModel.md#function-setcurrentformfeature) (const QgsFeature & feature) <br> |
|  void | [**setCurrentLayer**](classQfFeatureListModel.md#function-setcurrentlayer) (QgsVectorLayer \* currentLayer) <br> |
|  void | [**setDisplayGroupName**](classQfFeatureListModel.md#function-setdisplaygroupname) (bool displayGroupName) <br> |
|  void | [**setDisplayValueField**](classQfFeatureListModel.md#function-setdisplayvaluefield) (const QString & displayValueField) <br> |
|  void | [**setFilterExpression**](classQfFeatureListModel.md#function-setfilterexpression) (const QString & filterExpression) <br> |
|  void | [**setGroupField**](classQfFeatureListModel.md#function-setgroupfield) (const QString & groupField) <br> |
|  void | [**setKeyField**](classQfFeatureListModel.md#function-setkeyfield) (const QString & keyField) <br> |
|  void | [**setOrderByField**](classQfFeatureListModel.md#function-setorderbyfield) (bool orderByField) <br> |
|  void | [**setOrderByFieldName**](classQfFeatureListModel.md#function-setorderbyfieldname) (const QString & orderByFieldName) <br> |
|  void | [**setOrderByValue**](classQfFeatureListModel.md#function-setorderbyvalue) (bool orderByValue) <br> |
|  void | [**setSearchTerm**](classQfFeatureListModel.md#function-setsearchterm) (const QString & searchTerm) <br> |
|   | [**~QfFeatureListModel**](classQfFeatureListModel.md#function-qffeaturelistmodel) () <br> |






















































## Public Types Documentation




### enum FeatureListRoles 

```C++
enum QfFeatureCheckListModelBase::FeatureListRoles {
    CheckedRole = Qt::UserRole + 100
};
```




<hr>
## Public Signals Documentation




### signal allowMultiChanged 

```C++
void QfFeatureCheckListModelBase::allowMultiChanged;
```




<hr>



### signal attributeFieldChanged 

```C++
void QfFeatureCheckListModelBase::attributeFieldChanged;
```




<hr>



### signal attributeValueChanged 

```C++
void QfFeatureCheckListModelBase::attributeValueChanged;
```




<hr>



### signal listUpdated 

```C++
void QfFeatureCheckListModelBase::listUpdated;
```




<hr>
## Public Functions Documentation




### function QfFeatureCheckListModelBase 

```C++
explicit QfFeatureCheckListModelBase::QfFeatureCheckListModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function allowMulti 

```C++
bool QfFeatureCheckListModelBase::allowMulti () const
```



Whether multiple selected items are allowed 


        

<hr>



### function attributeField 

```C++
QgsField QfFeatureCheckListModelBase::attributeField () const
```



the current attribute field 


        

<hr>



### function attributeValue 

```C++
QVariant QfFeatureCheckListModelBase::attributeValue () const
```



the attribute value. A QVariantList or an hstore formatted string, depending on the field type. 


        

<hr>



### function data 

```C++
virtual QVariant QfFeatureCheckListModelBase::data (
    const QModelIndex & index,
    int role
) override const
```



Implements [*QfFeatureListModel::data*](classQfFeatureListModel.md#function-data)


<hr>



### function roleNames 

```C++
virtual QHash< int, QByteArray > QfFeatureCheckListModelBase::roleNames () override const
```



Implements [*QfFeatureListModel::roleNames*](classQfFeatureListModel.md#function-rolenames)


<hr>



### function setAllowMulti 

```C++
void QfFeatureCheckListModelBase::setAllowMulti (
    bool allowMulti
) 
```



Sets whether multiple selected items are allowed 


        

<hr>



### function setAttributeField 

```C++
void QfFeatureCheckListModelBase::setAttributeField (
    const QgsField & field
) 
```



the current attribute field 


        

<hr>



### function setAttributeValue 

```C++
void QfFeatureCheckListModelBase::setAttributeValue (
    const QVariant & attributeValue
) 
```



the attribute value. A QVariantList or an hstore formatted string, depending on the field type. 


        

<hr>



### function setData 

```C++
virtual bool QfFeatureCheckListModelBase::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```




<hr>



### function toggleCheckAll 

```C++
Q_INVOKABLE void QfFeatureCheckListModelBase::toggleCheckAll (
    bool toggleChecked=true
) 
```



Sets all element to either checked or unchecked state, depending on _toggleChecked_ 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qffeaturechecklistmodel.h`

