

# Class QfFeatureModel



[**ClassList**](annotated.md) **>** [**QfFeatureModel**](classQfFeatureModel.md)








Inherits the following classes: QAbstractListModel












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**RememberValues**](structQfFeatureModel_1_1RememberValues.md) <br>_keeping the information what attributes are remembered and the last edited feature_  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureRoles**](#enum-featureroles)  <br> |
| enum  | [**ModelModes**](#enum-modelmodes)  <br> |








## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**removeLayer**](classQfFeatureModel.md#slot-removelayer) (QObject \* layer) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classQfFeatureModel.md#property-appexpressioncontextscopesgenerator-12)  <br> |
| property bool | [**attributeEditingLocked**](classQfFeatureModel.md#property-attributeeditinglocked-12)  <br> |
| property bool | [**batchMode**](classQfFeatureModel.md#property-batchmode-12)  <br> |
| property QgsVectorLayer \* | [**currentLayer**](classQfFeatureModel.md#property-currentlayer)  <br> |
| property QgsFeature | [**feature**](classQfFeatureModel.md#property-feature-12)  <br> |
| property bool | [**featureAdditionLocked**](classQfFeatureModel.md#property-featureadditionlocked-12)  <br> |
| property bool | [**featureDeletionLocked**](classQfFeatureModel.md#property-featuredeletionlocked-12)  <br> |
| property QList&lt; QgsFeature &gt; | [**features**](classQfFeatureModel.md#property-features-12)  <br> |
| property [**QfGeometry**](classQfGeometry.md) \* | [**geometry**](classQfFeatureModel.md#property-geometry-12)  <br> |
| property bool | [**geometryEditingLocked**](classQfFeatureModel.md#property-geometryeditinglocked-12)  <br> |
| property QgsFeature | [**linkedParentFeature**](classQfFeatureModel.md#property-linkedparentfeature-12)  <br> |
| property QgsRelation | [**linkedRelation**](classQfFeatureModel.md#property-linkedrelation-12)  <br> |
| property QString | [**linkedRelationOrderingField**](classQfFeatureModel.md#property-linkedrelationorderingfield-12)  <br> |
| property QfFeatureModel::ModelModes | [**modelMode**](classQfFeatureModel.md#property-modelmode-12)  <br> |
| property QgsProject \* | [**project**](classQfFeatureModel.md#property-project-12)  <br> |
| property [**QfSnappingResult**](classQfSnappingResult.md) | [**topSnappingResult**](classQfFeatureModel.md#property-topsnappingresult-12)  <br> |
| property [**QfVertexModel**](classQfVertexModel.md) \* | [**vertexModel**](classQfFeatureModel.md#property-vertexmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**appExpressionContextScopesGeneratorChanged**](classQfFeatureModel.md#signal-appexpressioncontextscopesgeneratorchanged)  <br> |
| signal void | [**attributeEditingLockedChanged**](classQfFeatureModel.md#signal-attributeeditinglockedchanged)  <br> |
| signal void | [**batchModeChanged**](classQfFeatureModel.md#signal-batchmodechanged)  <br> |
| signal void | [**currentLayerChanged**](classQfFeatureModel.md#signal-currentlayerchanged)  <br> |
| signal void | [**featureAdditionLockedChanged**](classQfFeatureModel.md#signal-featureadditionlockedchanged)  <br> |
| signal void | [**featureChanged**](classQfFeatureModel.md#signal-featurechanged)  <br>_Emitted when the model's single feature has been changed._  |
| signal void | [**featureDeletionLockedChanged**](classQfFeatureModel.md#signal-featuredeletionlockedchanged)  <br> |
| signal void | [**featureUpdated**](classQfFeatureModel.md#signal-featureupdated)  <br>_Emitted when the model's feature has been saved (i.e. updated) but not changed as a result._  |
| signal void | [**featuresChanged**](classQfFeatureModel.md#signal-featureschanged)  <br>_Emitted when the model's multi features list has been changed._  |
| signal void | [**geometryChanged**](classQfFeatureModel.md#signal-geometrychanged)  <br> |
| signal void | [**geometryEditingLockedChanged**](classQfFeatureModel.md#signal-geometryeditinglockedchanged)  <br> |
| signal void | [**linkedParentFeatureChanged**](classQfFeatureModel.md#signal-linkedparentfeaturechanged)  <br> |
| signal void | [**linkedRelationChanged**](classQfFeatureModel.md#signal-linkedrelationchanged)  <br> |
| signal void | [**linkedRelationOrderingFieldChanged**](classQfFeatureModel.md#signal-linkedrelationorderingfieldchanged)  <br> |
| signal void | [**modelModeChanged**](classQfFeatureModel.md#signal-modelmodechanged)  <br> |
| signal void | [**projectChanged**](classQfFeatureModel.md#signal-projectchanged)  <br> |
| signal void | [**topSnappingResultChanged**](classQfFeatureModel.md#signal-topsnappingresultchanged)  <br> |
| signal void | [**vertexModelChanged**](classQfFeatureModel.md#signal-vertexmodelchanged)  <br> |
| signal void | [**warning**](classQfFeatureModel.md#signal-warning) (const QString & text) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureModel**](#function-qffeaturemodel) (QObject \* parent=nullptr) <br> |
|  [**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](#function-appexpressioncontextscopesgenerator-22) () const<br> |
|  Q\_INVOKABLE void | [**applyGeometry**](#function-applygeometry) (bool fromVertexModel=false, bool skipTopologicalEditing=false) <br> |
|  Q\_INVOKABLE void | [**applyGeometryToVertexModel**](#function-applygeometrytovertexmodel) () <br>_Apply the feature geometry to a vertex model if present._  |
|  bool | [**attributeEditingLocked**](#function-attributeeditinglocked-22) () const<br> |
|  bool | [**batchMode**](#function-batchmode-22) () const<br> |
|  Q\_INVOKABLE bool | [**changeGeometry**](#function-changegeometry) (const QgsGeometry & geometry) <br> |
|  Q\_INVOKABLE bool | [**create**](#function-create) (bool flushBuffer=true) <br> |
|  QgsExpressionContext | [**createExpressionContext**](#function-createexpressioncontext) () const<br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE bool | [**deleteFeature**](#function-deletefeature) () <br> |
|  QgsFeature | [**feature**](#function-feature-22) () const<br> |
|  bool | [**featureAdditionLocked**](#function-featureadditionlocked-22) () const<br> |
|  bool | [**featureDeletionLocked**](#function-featuredeletionlocked-22) () const<br> |
|  QList&lt; QgsFeature &gt; | [**features**](#function-features-22) () const<br> |
|  [**QfGeometry**](classQfGeometry.md) \* | [**geometry**](#function-geometry-22) () <br> |
|  bool | [**geometryEditingLocked**](#function-geometryeditinglocked-22) () const<br> |
|  QgsVectorLayer \* | [**layer**](#function-layer) () const<br> |
|  QgsFeature | [**linkedParentFeature**](#function-linkedparentfeature-22) () const<br> |
|  QgsRelation | [**linkedRelation**](#function-linkedrelation-22) () const<br> |
|  QString | [**linkedRelationOrderingField**](#function-linkedrelationorderingfield-22) () const<br> |
|  ModelModes | [**modelMode**](#function-modelmode-22) () const<br> |
|  bool | [**positionLocked**](#function-positionlocked) () const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_Returns the current project from which the digitizing logs will be sought._  |
|  Q\_INVOKABLE void | [**refresh**](#function-refresh) () <br> |
|  QVector&lt; bool &gt; | [**rememberedAttributes**](#function-rememberedattributes) () const<br> |
|  Q\_INVOKABLE void | [**reset**](#function-reset) () <br> |
|  Q\_INVOKABLE void | [**resetAttributes**](#function-resetattributes) (bool partialReset=false) <br> |
|  Q\_INVOKABLE void | [**resetFeature**](#function-resetfeature) () <br> |
|  Q\_INVOKABLE void | [**resetFeatureId**](#function-resetfeatureid) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  Q\_INVOKABLE bool | [**save**](#function-save) (bool flushBuffer=true) <br> |
|  void | [**setAppExpressionContextScopesGenerator**](#function-setappexpressioncontextscopesgenerator) ([**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* generator) <br> |
|  void | [**setBatchMode**](#function-setbatchmode) (bool batchMode) <br> |
|  void | [**setCurrentLayer**](#function-setcurrentlayer) (QgsVectorLayer \* layer) <br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role=Qt::EditRole) override<br> |
|  void | [**setFeature**](#function-setfeature) (const QgsFeature & feature) <br> |
|  void | [**setFeatures**](#function-setfeatures) (const QList&lt; QgsFeature &gt; & features) <br> |
|  void | [**setGeometry**](#function-setgeometry) ([**QfGeometry**](classQfGeometry.md) \* geometry) <br> |
|  void | [**setLinkedParentFeature**](#function-setlinkedparentfeature) (const QgsFeature & feature) <br> |
|  void | [**setLinkedRelation**](#function-setlinkedrelation) (const QgsRelation & relation) <br> |
|  void | [**setLinkedRelationOrderingField**](#function-setlinkedrelationorderingfield) (const QString & orderingField) <br> |
|  void | [**setModelMode**](#function-setmodelmode) (const ModelModes mode) <br> |
|  void | [**setPositionLocked**](#function-setpositionlocked) (bool positionLocked) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_Sets the_ _project_ _used to find the digitizing logs layer._ |
|  void | [**setTopSnappingResult**](#function-settopsnappingresult) (const [**QfSnappingResult**](classQfSnappingResult.md) & topSnappingResult) <br> |
|  void | [**setVertexModel**](#function-setvertexmodel) ([**QfVertexModel**](classQfVertexModel.md) \* model) <br>_Sets the vertex_ _model_ _is used to highlight vertices on the map._ |
|  Q\_INVOKABLE bool | [**suppressFeatureForm**](#function-suppressfeatureform) () const<br> |
|  [**QfSnappingResult**](classQfSnappingResult.md) | [**topSnappingResult**](#function-topsnappingresult-22) () const<br> |
|  Q\_INVOKABLE bool | [**updateAttributesFromFeature**](#function-updateattributesfromfeature) (const QgsFeature & feature) <br> |
|  Q\_INVOKABLE void | [**updateRubberband**](#function-updaterubberband) () const<br>_Update the linked geometry rubber band to match the feature's geometry._  |
|  [**QfVertexModel**](classQfVertexModel.md) \* | [**vertexModel**](#function-vertexmodel-22) () <br>_Returns the vertex model is used to highlight vertices on the map._  |




























## Public Types Documentation




### enum FeatureRoles 

```C++
enum QfFeatureModel::FeatureRoles {
    AttributeName = Qt::UserRole + 1,
    AttributeValue,
    Field,
    RememberAttribute,
    LinkedAttribute,
    AttributeAllowEdit
};
```




<hr>



### enum ModelModes 

```C++
enum QfFeatureModel::ModelModes {
    SingleFeatureModel = 1,
    MultiFeatureModel
};
```




<hr>
## Public Properties Documentation




### property appExpressionContextScopesGenerator [1/2]

```C++
QfAppExpressionContextScopesGenerator * QfFeatureModel::appExpressionContextScopesGenerator;
```




<hr>



### property attributeEditingLocked [1/2]

```C++
bool QfFeatureModel::attributeEditingLocked;
```




<hr>



### property batchMode [1/2]

```C++
bool QfFeatureModel::batchMode;
```




<hr>



### property currentLayer 

```C++
QgsVectorLayer * QfFeatureModel::currentLayer;
```




<hr>



### property feature [1/2]

```C++
QgsFeature QfFeatureModel::feature;
```




<hr>



### property featureAdditionLocked [1/2]

```C++
bool QfFeatureModel::featureAdditionLocked;
```




<hr>



### property featureDeletionLocked [1/2]

```C++
bool QfFeatureModel::featureDeletionLocked;
```




<hr>



### property features [1/2]

```C++
QList<QgsFeature> QfFeatureModel::features;
```




<hr>



### property geometry [1/2]

```C++
QfGeometry * QfFeatureModel::geometry;
```




<hr>



### property geometryEditingLocked [1/2]

```C++
bool QfFeatureModel::geometryEditingLocked;
```




<hr>



### property linkedParentFeature [1/2]

```C++
QgsFeature QfFeatureModel::linkedParentFeature;
```




<hr>



### property linkedRelation [1/2]

```C++
QgsRelation QfFeatureModel::linkedRelation;
```




<hr>



### property linkedRelationOrderingField [1/2]

```C++
QString QfFeatureModel::linkedRelationOrderingField;
```




<hr>



### property modelMode [1/2]

```C++
QfFeatureModel::ModelModes QfFeatureModel::modelMode;
```




<hr>



### property project [1/2]

```C++
QgsProject * QfFeatureModel::project;
```




<hr>



### property topSnappingResult [1/2]

```C++
QfSnappingResult QfFeatureModel::topSnappingResult;
```




<hr>



### property vertexModel [1/2]

```C++
QfVertexModel * QfFeatureModel::vertexModel;
```




<hr>
## Public Slots Documentation




### slot removeLayer 

```C++
void QfFeatureModel::removeLayer;
```




<hr>
## Public Signals Documentation




### signal appExpressionContextScopesGeneratorChanged 

```C++
void QfFeatureModel::appExpressionContextScopesGeneratorChanged;
```




<hr>



### signal attributeEditingLockedChanged 

```C++
void QfFeatureModel::attributeEditingLockedChanged;
```




<hr>



### signal batchModeChanged 

```C++
void QfFeatureModel::batchModeChanged;
```




<hr>



### signal currentLayerChanged 

```C++
void QfFeatureModel::currentLayerChanged;
```




<hr>



### signal featureAdditionLockedChanged 

```C++
void QfFeatureModel::featureAdditionLockedChanged;
```




<hr>



### signal featureChanged 

_Emitted when the model's single feature has been changed._ 
```C++
void QfFeatureModel::featureChanged;
```




<hr>



### signal featureDeletionLockedChanged 

```C++
void QfFeatureModel::featureDeletionLockedChanged;
```




<hr>



### signal featureUpdated 

_Emitted when the model's feature has been saved (i.e. updated) but not changed as a result._ 
```C++
void QfFeatureModel::featureUpdated;
```




<hr>



### signal featuresChanged 

_Emitted when the model's multi features list has been changed._ 
```C++
void QfFeatureModel::featuresChanged;
```




<hr>



### signal geometryChanged 

```C++
void QfFeatureModel::geometryChanged;
```




<hr>



### signal geometryEditingLockedChanged 

```C++
void QfFeatureModel::geometryEditingLockedChanged;
```




<hr>



### signal linkedParentFeatureChanged 

```C++
void QfFeatureModel::linkedParentFeatureChanged;
```




<hr>



### signal linkedRelationChanged 

```C++
void QfFeatureModel::linkedRelationChanged;
```




<hr>



### signal linkedRelationOrderingFieldChanged 

```C++
void QfFeatureModel::linkedRelationOrderingFieldChanged;
```




<hr>



### signal modelModeChanged 

```C++
void QfFeatureModel::modelModeChanged;
```




<hr>



### signal projectChanged 

```C++
void QfFeatureModel::projectChanged;
```




<hr>



### signal topSnappingResultChanged 

```C++
void QfFeatureModel::topSnappingResultChanged;
```




<hr>



### signal vertexModelChanged 

```C++
void QfFeatureModel::vertexModelChanged;
```




<hr>



### signal warning 

```C++
void QfFeatureModel::warning;
```




<hr>
## Public Functions Documentation




### function QfFeatureModel 

```C++
explicit QfFeatureModel::QfFeatureModel (
    QObject * parent=nullptr
) 
```




<hr>



### function appExpressionContextScopesGenerator [2/2]

```C++
QfAppExpressionContextScopesGenerator * QfFeatureModel::appExpressionContextScopesGenerator () const
```



Returns the application expression context scopes generator object 


        

<hr>



### function applyGeometry 

```C++
Q_INVOKABLE void QfFeatureModel::applyGeometry (
    bool fromVertexModel=false,
    bool skipTopologicalEditing=false
) 
```



Apply the geometry object or vertex model object's geometry to the feature geometry. 

**Parameters:**


* `fromVertexModel` set to TRUE to use the vertex model 
* `skipTopologicalEditing` set to TRUE to skip project-level topological editing rule 




        

<hr>



### function applyGeometryToVertexModel 

_Apply the feature geometry to a vertex model if present._ 
```C++
Q_INVOKABLE void QfFeatureModel::applyGeometryToVertexModel () 
```




<hr>



### function attributeEditingLocked [2/2]

```C++
inline bool QfFeatureModel::attributeEditingLocked () const
```




<hr>



### function batchMode [2/2]

```C++
inline bool QfFeatureModel::batchMode () const
```



Returns TRUE if the feature model is in batch mode. When enabled, the vector layer will remain in editing mode until batch mode is disabled. 


        

<hr>



### function changeGeometry 

```C++
Q_INVOKABLE bool QfFeatureModel::changeGeometry (
    const QgsGeometry & geometry
) 
```



Sets the _geometry_ of the feature. 


        

<hr>



### function create 

```C++
Q_INVOKABLE bool QfFeatureModel::create (
    bool flushBuffer=true
) 
```



Will create this feature as a new feature on the data source.


By setting _flushBuffer_ to FALSE, the created feature will remain in the edit buffer provided the vector layer was already in editing mode. The flushBuffer parameter will be ignored for layers containing relationships.




**Returns:**

TRUE if a feature was successfully created 





        

<hr>



### function createExpressionContext 

```C++
QgsExpressionContext QfFeatureModel::createExpressionContext () const
```




<hr>



### function data 

```C++
QVariant QfFeatureModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function deleteFeature 

```C++
Q_INVOKABLE bool QfFeatureModel::deleteFeature () 
```



Deletes the current feature from the data source. 


        

<hr>



### function feature [2/2]

```C++
QgsFeature QfFeatureModel::feature () const
```




<hr>



### function featureAdditionLocked [2/2]

```C++
inline bool QfFeatureModel::featureAdditionLocked () const
```




<hr>



### function featureDeletionLocked [2/2]

```C++
inline bool QfFeatureModel::featureDeletionLocked () const
```




<hr>



### function features [2/2]

```C++
QList< QgsFeature > QfFeatureModel::features () const
```



Return the features list for passing it around in QML 


        

<hr>



### function geometry [2/2]

```C++
QfGeometry * QfFeatureModel::geometry () 
```



Returns the geometry object that will drive the feature geometry. 


        

<hr>



### function geometryEditingLocked [2/2]

```C++
inline bool QfFeatureModel::geometryEditingLocked () const
```




<hr>



### function layer 

```C++
QgsVectorLayer * QfFeatureModel::layer () const
```




<hr>



### function linkedParentFeature [2/2]

```C++
QgsFeature QfFeatureModel::linkedParentFeature () const
```



A linked feature is a parent feature of a relation passing it's pk(s) to the created child features fk(s) 

**Returns:**

the parent feature linked to this feature 




**See also:** [**setLinkedParentFeature**](classQfFeatureModel.md#function-setlinkedparentfeature) 



        

<hr>



### function linkedRelation [2/2]

```C++
QgsRelation QfFeatureModel::linkedRelation () const
```



The relation connecting this feature to the parent, over which this feature has been loaded (e.g. over relation editor widget) 

**Returns:**

the relation connecting the parent 




**See also:** [**setLinkedRelation**](classQfFeatureModel.md#function-setlinkedrelation) 



        

<hr>



### function linkedRelationOrderingField [2/2]

```C++
QString QfFeatureModel::linkedRelationOrderingField () const
```



Returns the field name used for ordering in ordered relations. Empty string if not available. 

**See also:** [**setLinkedRelationOrderingField**](classQfFeatureModel.md#function-setlinkedrelationorderingfield) 



        

<hr>



### function modelMode [2/2]

```C++
ModelModes QfFeatureModel::modelMode () const
```




<hr>



### function positionLocked 

```C++
bool QfFeatureModel::positionLocked () const
```



Returns whether the position is locked to the GNSS 


        

<hr>



### function project [2/2]

_Returns the current project from which the digitizing logs will be sought._ 
```C++
inline QgsProject * QfFeatureModel::project () const
```




<hr>



### function refresh 

```C++
Q_INVOKABLE void QfFeatureModel::refresh () 
```



Will refresh the feature values and geometry from the data source. 


        

<hr>



### function rememberedAttributes 

```C++
QVector< bool > QfFeatureModel::rememberedAttributes () const
```




<hr>



### function reset 

```C++
Q_INVOKABLE void QfFeatureModel::reset () 
```



Will reset the feature to the original values and dismiss any buffered edits. 


        

<hr>



### function resetAttributes 

```C++
Q_INVOKABLE void QfFeatureModel::resetAttributes (
    bool partialReset=false
) 
```



Resets the attribute values of the current feature 

**Parameters:**


* `partialReset` when set to TRUE, only attributes with default or remembered values will be reset 




        

<hr>



### function resetFeature 

```C++
Q_INVOKABLE void QfFeatureModel::resetFeature () 
```




<hr>



### function resetFeatureId 

```C++
Q_INVOKABLE void QfFeatureModel::resetFeatureId () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfFeatureModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfFeatureModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function save 

```C++
Q_INVOKABLE bool QfFeatureModel::save (
    bool flushBuffer=true
) 
```



Will commit the edit buffer of this layer. May change in the future to only commit the changes buffered in this model.


By setting _flushBuffer_ to FALSE, the edits made to the feature will remain in the edit buffer provided the vector layer was already in editing mode.




**Returns:**

TRUE if a feature was successfully saved 





        

<hr>



### function setAppExpressionContextScopesGenerator 

```C++
void QfFeatureModel::setAppExpressionContextScopesGenerator (
    QfAppExpressionContextScopesGenerator * generator
) 
```



Sets the application expression context scopes _generator_ object 


        

<hr>



### function setBatchMode 

```C++
void QfFeatureModel::setBatchMode (
    bool batchMode
) 
```



Toggles the feature model batch mode. When enabled, the vector layer will remain in editing mode until batch mode is disabled. 


        

<hr>



### function setCurrentLayer 

```C++
void QfFeatureModel::setCurrentLayer (
    QgsVectorLayer * layer
) 
```




<hr>



### function setData 

```C++
bool QfFeatureModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setFeature 

```C++
void QfFeatureModel::setFeature (
    const QgsFeature & feature
) 
```




<hr>



### function setFeatures 

```C++
void QfFeatureModel::setFeatures (
    const QList< QgsFeature > & features
) 
```




<hr>



### function setGeometry 

```C++
void QfFeatureModel::setGeometry (
    QfGeometry * geometry
) 
```



Sets the geometry object that will drive the feature geometry. 

**Note:**

This is not the QgsGeometry of the feature. To change that, use the changeGeometry function. 





        

<hr>



### function setLinkedParentFeature 

```C++
void QfFeatureModel::setLinkedParentFeature (
    const QgsFeature & feature
) 
```



A linked feature is a parent feature of a relation passing it's pk(s) to the created child features fk(s) The fk fields are evaluated over the linked relation. 

**Parameters:**


* `feature` 



**See also:** linkedParentFeature 



        

<hr>



### function setLinkedRelation 

```C++
void QfFeatureModel::setLinkedRelation (
    const QgsRelation & relation
) 
```



The relation connecting this feature to the parent, over which this feature has been loaded (e.g. over relation editor widget) The relation is userd to evaluate the parents pk(s) and the childs fk(s) 

**Parameters:**


* `relation` 



**See also:** linkedRelation 



        

<hr>



### function setLinkedRelationOrderingField 

```C++
void QfFeatureModel::setLinkedRelationOrderingField (
    const QString & orderingField
) 
```



Sets the field name used for ordering in ordered relations. Empty string if not available. 

**See also:** linkedRelationOrderingField 



        

<hr>



### function setModelMode 

```C++
void QfFeatureModel::setModelMode (
    const ModelModes mode
) 
```




<hr>



### function setPositionLocked 

```C++
void QfFeatureModel::setPositionLocked (
    bool positionLocked
) 
```



Sets whether the position is locked to the GNSS 


        

<hr>



### function setProject 

_Sets the_ _project_ _used to find the digitizing logs layer._
```C++
void QfFeatureModel::setProject (
    QgsProject * project
) 
```




<hr>



### function setTopSnappingResult 

```C++
void QfFeatureModel::setTopSnappingResult (
    const QfSnappingResult & topSnappingResult
) 
```



Sets the top snapping result of the coordinate locator 

**Parameters:**


* `topSnappingResult` the top snapping result object 




        

<hr>



### function setVertexModel 

_Sets the vertex_ _model_ _is used to highlight vertices on the map._
```C++
void QfFeatureModel::setVertexModel (
    QfVertexModel * model
) 
```




<hr>



### function suppressFeatureForm 

```C++
Q_INVOKABLE bool QfFeatureModel::suppressFeatureForm () const
```



Returns whether the feature form should be suppressed when adding new features. 


        

<hr>



### function topSnappingResult [2/2]

```C++
QfSnappingResult QfFeatureModel::topSnappingResult () const
```



Returns the top snapping result of the coordinate locator 


        

<hr>



### function updateAttributesFromFeature 

```C++
Q_INVOKABLE bool QfFeatureModel::updateAttributesFromFeature (
    const QgsFeature & feature
) 
```



Update attributes to match that of a given feature 

**Parameters:**


* `feature` feature from which attributes will be taken 



**Returns:**

TRUE if one or more attribute was updated 





        

<hr>



### function updateRubberband 

_Update the linked geometry rubber band to match the feature's geometry._ 
```C++
Q_INVOKABLE void QfFeatureModel::updateRubberband () const
```




<hr>



### function vertexModel [2/2]

_Returns the vertex model is used to highlight vertices on the map._ 
```C++
QfVertexModel * QfFeatureModel::vertexModel () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qffeaturemodel.h`

