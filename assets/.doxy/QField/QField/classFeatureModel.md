

# Class FeatureModel



[**ClassList**](annotated.md) **>** [**FeatureModel**](classFeatureModel.md)








Inherits the following classes: QAbstractListModel












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**RememberValues**](structFeatureModel_1_1RememberValues.md) <br>_keeping the information what attributes are remembered and the last edited feature_  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureRoles**](#enum-featureroles)  <br> |
| enum  | [**ModelModes**](#enum-modelmodes)  <br> |








## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**removeLayer**](classFeatureModel.md#slot-removelayer) (QObject \* layer) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property [**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classFeatureModel.md#property-appexpressioncontextscopesgenerator-12)  <br> |
| property bool | [**attributeEditingLocked**](classFeatureModel.md#property-attributeeditinglocked-12)  <br> |
| property bool | [**batchMode**](classFeatureModel.md#property-batchmode-12)  <br> |
| property QgsVectorLayer \* | [**currentLayer**](classFeatureModel.md#property-currentlayer)  <br> |
| property QgsFeature | [**feature**](classFeatureModel.md#property-feature-12)  <br> |
| property bool | [**featureAdditionLocked**](classFeatureModel.md#property-featureadditionlocked-12)  <br> |
| property bool | [**featureDeletionLocked**](classFeatureModel.md#property-featuredeletionlocked-12)  <br> |
| property QList&lt; QgsFeature &gt; | [**features**](classFeatureModel.md#property-features-12)  <br> |
| property [**Geometry**](classGeometry.md) \* | [**geometry**](classFeatureModel.md#property-geometry-12)  <br> |
| property bool | [**geometryEditingLocked**](classFeatureModel.md#property-geometryeditinglocked-12)  <br> |
| property QgsFeature | [**linkedParentFeature**](classFeatureModel.md#property-linkedparentfeature-12)  <br> |
| property QgsRelation | [**linkedRelation**](classFeatureModel.md#property-linkedrelation-12)  <br> |
| property QString | [**linkedRelationOrderingField**](classFeatureModel.md#property-linkedrelationorderingfield-12)  <br> |
| property FeatureModel::ModelModes | [**modelMode**](classFeatureModel.md#property-modelmode-12)  <br> |
| property QgsProject \* | [**project**](classFeatureModel.md#property-project-12)  <br> |
| property [**SnappingResult**](classSnappingResult.md) | [**topSnappingResult**](classFeatureModel.md#property-topsnappingresult-12)  <br> |
| property [**VertexModel**](classVertexModel.md) \* | [**vertexModel**](classFeatureModel.md#property-vertexmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**appExpressionContextScopesGeneratorChanged**](classFeatureModel.md#signal-appexpressioncontextscopesgeneratorchanged)  <br> |
| signal void | [**attributeEditingLockedChanged**](classFeatureModel.md#signal-attributeeditinglockedchanged)  <br> |
| signal void | [**batchModeChanged**](classFeatureModel.md#signal-batchmodechanged)  <br> |
| signal void | [**currentLayerChanged**](classFeatureModel.md#signal-currentlayerchanged)  <br> |
| signal void | [**featureAdditionLockedChanged**](classFeatureModel.md#signal-featureadditionlockedchanged)  <br> |
| signal void | [**featureChanged**](classFeatureModel.md#signal-featurechanged)  <br>_Emitted when the model's single feature has been changed._  |
| signal void | [**featureDeletionLockedChanged**](classFeatureModel.md#signal-featuredeletionlockedchanged)  <br> |
| signal void | [**featureUpdated**](classFeatureModel.md#signal-featureupdated)  <br>_Emitted when the model's feature has been saved (i.e. updated) but not changed as a result._  |
| signal void | [**featuresChanged**](classFeatureModel.md#signal-featureschanged)  <br>_Emitted when the model's multi features list has been changed._  |
| signal void | [**geometryChanged**](classFeatureModel.md#signal-geometrychanged)  <br> |
| signal void | [**geometryEditingLockedChanged**](classFeatureModel.md#signal-geometryeditinglockedchanged)  <br> |
| signal void | [**linkedParentFeatureChanged**](classFeatureModel.md#signal-linkedparentfeaturechanged)  <br> |
| signal void | [**linkedRelationChanged**](classFeatureModel.md#signal-linkedrelationchanged)  <br> |
| signal void | [**linkedRelationOrderingFieldChanged**](classFeatureModel.md#signal-linkedrelationorderingfieldchanged)  <br> |
| signal void | [**modelModeChanged**](classFeatureModel.md#signal-modelmodechanged)  <br> |
| signal void | [**projectChanged**](classFeatureModel.md#signal-projectchanged)  <br> |
| signal void | [**topSnappingResultChanged**](classFeatureModel.md#signal-topsnappingresultchanged)  <br> |
| signal void | [**vertexModelChanged**](classFeatureModel.md#signal-vertexmodelchanged)  <br> |
| signal void | [**warning**](classFeatureModel.md#signal-warning) (const QString & text) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeatureModel**](#function-featuremodel) (QObject \* parent=nullptr) <br> |
|  [**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](#function-appexpressioncontextscopesgenerator-22) () const<br> |
|  Q\_INVOKABLE void | [**applyGeometry**](#function-applygeometry) (bool fromVertexModel=false) <br> |
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
|  [**Geometry**](classGeometry.md) \* | [**geometry**](#function-geometry-22) () <br> |
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
|  void | [**setAppExpressionContextScopesGenerator**](#function-setappexpressioncontextscopesgenerator) ([**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* generator) <br> |
|  void | [**setBatchMode**](#function-setbatchmode) (bool batchMode) <br> |
|  void | [**setCurrentLayer**](#function-setcurrentlayer) (QgsVectorLayer \* layer) <br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role=Qt::EditRole) override<br> |
|  void | [**setFeature**](#function-setfeature) (const QgsFeature & feature) <br> |
|  void | [**setFeatures**](#function-setfeatures) (const QList&lt; QgsFeature &gt; & features) <br> |
|  void | [**setGeometry**](#function-setgeometry) ([**Geometry**](classGeometry.md) \* geometry) <br> |
|  void | [**setLinkedParentFeature**](#function-setlinkedparentfeature) (const QgsFeature & feature) <br> |
|  void | [**setLinkedRelation**](#function-setlinkedrelation) (const QgsRelation & relation) <br> |
|  void | [**setLinkedRelationOrderingField**](#function-setlinkedrelationorderingfield) (const QString & orderingField) <br> |
|  void | [**setModelMode**](#function-setmodelmode) (const ModelModes mode) <br> |
|  void | [**setPositionLocked**](#function-setpositionlocked) (bool positionLocked) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_Sets the_ _project_ _used to find the digitizing logs layer._ |
|  void | [**setTopSnappingResult**](#function-settopsnappingresult) (const [**SnappingResult**](classSnappingResult.md) & topSnappingResult) <br> |
|  void | [**setVertexModel**](#function-setvertexmodel) ([**VertexModel**](classVertexModel.md) \* model) <br>_Sets the vertex_ _model_ _is used to highlight vertices on the map._ |
|  Q\_INVOKABLE bool | [**suppressFeatureForm**](#function-suppressfeatureform) () const<br> |
|  [**SnappingResult**](classSnappingResult.md) | [**topSnappingResult**](#function-topsnappingresult-22) () const<br> |
|  Q\_INVOKABLE bool | [**updateAttributesFromFeature**](#function-updateattributesfromfeature) (const QgsFeature & feature) <br> |
|  Q\_INVOKABLE void | [**updateRubberband**](#function-updaterubberband) () const<br>_Update the linked geometry rubber band to match the feature's geometry._  |
|  [**VertexModel**](classVertexModel.md) \* | [**vertexModel**](#function-vertexmodel-22) () <br>_Returns the vertex model is used to highlight vertices on the map._  |




























## Public Types Documentation




### enum FeatureRoles 

```C++
enum FeatureModel::FeatureRoles {
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
enum FeatureModel::ModelModes {
    SingleFeatureModel = 1,
    MultiFeatureModel
};
```




<hr>
## Public Properties Documentation




### property appExpressionContextScopesGenerator [1/2]

```C++
AppExpressionContextScopesGenerator * FeatureModel::appExpressionContextScopesGenerator;
```




<hr>



### property attributeEditingLocked [1/2]

```C++
bool FeatureModel::attributeEditingLocked;
```




<hr>



### property batchMode [1/2]

```C++
bool FeatureModel::batchMode;
```




<hr>



### property currentLayer 

```C++
QgsVectorLayer * FeatureModel::currentLayer;
```




<hr>



### property feature [1/2]

```C++
QgsFeature FeatureModel::feature;
```




<hr>



### property featureAdditionLocked [1/2]

```C++
bool FeatureModel::featureAdditionLocked;
```




<hr>



### property featureDeletionLocked [1/2]

```C++
bool FeatureModel::featureDeletionLocked;
```




<hr>



### property features [1/2]

```C++
QList<QgsFeature> FeatureModel::features;
```




<hr>



### property geometry [1/2]

```C++
Geometry * FeatureModel::geometry;
```




<hr>



### property geometryEditingLocked [1/2]

```C++
bool FeatureModel::geometryEditingLocked;
```




<hr>



### property linkedParentFeature [1/2]

```C++
QgsFeature FeatureModel::linkedParentFeature;
```




<hr>



### property linkedRelation [1/2]

```C++
QgsRelation FeatureModel::linkedRelation;
```




<hr>



### property linkedRelationOrderingField [1/2]

```C++
QString FeatureModel::linkedRelationOrderingField;
```




<hr>



### property modelMode [1/2]

```C++
FeatureModel::ModelModes FeatureModel::modelMode;
```




<hr>



### property project [1/2]

```C++
QgsProject * FeatureModel::project;
```




<hr>



### property topSnappingResult [1/2]

```C++
SnappingResult FeatureModel::topSnappingResult;
```




<hr>



### property vertexModel [1/2]

```C++
VertexModel * FeatureModel::vertexModel;
```




<hr>
## Public Slots Documentation




### slot removeLayer 

```C++
void FeatureModel::removeLayer;
```




<hr>
## Public Signals Documentation




### signal appExpressionContextScopesGeneratorChanged 

```C++
void FeatureModel::appExpressionContextScopesGeneratorChanged;
```




<hr>



### signal attributeEditingLockedChanged 

```C++
void FeatureModel::attributeEditingLockedChanged;
```




<hr>



### signal batchModeChanged 

```C++
void FeatureModel::batchModeChanged;
```




<hr>



### signal currentLayerChanged 

```C++
void FeatureModel::currentLayerChanged;
```




<hr>



### signal featureAdditionLockedChanged 

```C++
void FeatureModel::featureAdditionLockedChanged;
```




<hr>



### signal featureChanged 

_Emitted when the model's single feature has been changed._ 
```C++
void FeatureModel::featureChanged;
```




<hr>



### signal featureDeletionLockedChanged 

```C++
void FeatureModel::featureDeletionLockedChanged;
```




<hr>



### signal featureUpdated 

_Emitted when the model's feature has been saved (i.e. updated) but not changed as a result._ 
```C++
void FeatureModel::featureUpdated;
```




<hr>



### signal featuresChanged 

_Emitted when the model's multi features list has been changed._ 
```C++
void FeatureModel::featuresChanged;
```




<hr>



### signal geometryChanged 

```C++
void FeatureModel::geometryChanged;
```




<hr>



### signal geometryEditingLockedChanged 

```C++
void FeatureModel::geometryEditingLockedChanged;
```




<hr>



### signal linkedParentFeatureChanged 

```C++
void FeatureModel::linkedParentFeatureChanged;
```




<hr>



### signal linkedRelationChanged 

```C++
void FeatureModel::linkedRelationChanged;
```




<hr>



### signal linkedRelationOrderingFieldChanged 

```C++
void FeatureModel::linkedRelationOrderingFieldChanged;
```




<hr>



### signal modelModeChanged 

```C++
void FeatureModel::modelModeChanged;
```




<hr>



### signal projectChanged 

```C++
void FeatureModel::projectChanged;
```




<hr>



### signal topSnappingResultChanged 

```C++
void FeatureModel::topSnappingResultChanged;
```




<hr>



### signal vertexModelChanged 

```C++
void FeatureModel::vertexModelChanged;
```




<hr>



### signal warning 

```C++
void FeatureModel::warning;
```




<hr>
## Public Functions Documentation




### function FeatureModel 

```C++
explicit FeatureModel::FeatureModel (
    QObject * parent=nullptr
) 
```




<hr>



### function appExpressionContextScopesGenerator [2/2]

```C++
AppExpressionContextScopesGenerator * FeatureModel::appExpressionContextScopesGenerator () const
```



Returns the application expression context scopes generator object 


        

<hr>



### function applyGeometry 

```C++
Q_INVOKABLE void FeatureModel::applyGeometry (
    bool fromVertexModel=false
) 
```



Apply the geometry object or vertex model object's geometry to the feature geometry. 

**Parameters:**


* `fromVertexModel` set to TRUE to use the vertex model 




        

<hr>



### function applyGeometryToVertexModel 

_Apply the feature geometry to a vertex model if present._ 
```C++
Q_INVOKABLE void FeatureModel::applyGeometryToVertexModel () 
```




<hr>



### function attributeEditingLocked [2/2]

```C++
inline bool FeatureModel::attributeEditingLocked () const
```




<hr>



### function batchMode [2/2]

```C++
inline bool FeatureModel::batchMode () const
```



Returns TRUE if the feature model is in batch mode. When enabled, the vector layer will remain in editing mode until batch mode is disabled. 


        

<hr>



### function changeGeometry 

```C++
Q_INVOKABLE bool FeatureModel::changeGeometry (
    const QgsGeometry & geometry
) 
```



Sets the _geometry_ of the feature. 


        

<hr>



### function create 

```C++
Q_INVOKABLE bool FeatureModel::create (
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
QgsExpressionContext FeatureModel::createExpressionContext () const
```




<hr>



### function data 

```C++
QVariant FeatureModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function deleteFeature 

```C++
Q_INVOKABLE bool FeatureModel::deleteFeature () 
```



Deletes the current feature from the data source. 


        

<hr>



### function feature [2/2]

```C++
QgsFeature FeatureModel::feature () const
```



Return the feature wrapped in a QVariant for passing it around in QML 


        

<hr>



### function featureAdditionLocked [2/2]

```C++
inline bool FeatureModel::featureAdditionLocked () const
```




<hr>



### function featureDeletionLocked [2/2]

```C++
inline bool FeatureModel::featureDeletionLocked () const
```




<hr>



### function features [2/2]

```C++
QList< QgsFeature > FeatureModel::features () const
```



Return the features list for passing it around in QML 


        

<hr>



### function geometry [2/2]

```C++
Geometry * FeatureModel::geometry () 
```



Returns the geometry object that will drive the feature geometry. 


        

<hr>



### function geometryEditingLocked [2/2]

```C++
inline bool FeatureModel::geometryEditingLocked () const
```




<hr>



### function layer 

```C++
QgsVectorLayer * FeatureModel::layer () const
```




<hr>



### function linkedParentFeature [2/2]

```C++
QgsFeature FeatureModel::linkedParentFeature () const
```



A linked feature is a parent feature of a relation passing it's pk(s) to the created child features fk(s) 

**Returns:**

the parent feature linked to this feature 




**See also:** [**setLinkedParentFeature**](classFeatureModel.md#function-setlinkedparentfeature) 



        

<hr>



### function linkedRelation [2/2]

```C++
QgsRelation FeatureModel::linkedRelation () const
```



The relation connecting this feature to the parent, over which this feature has been loaded (e.g. over relation editor widget) 

**Returns:**

the relation connecting the parent 




**See also:** [**setLinkedRelation**](classFeatureModel.md#function-setlinkedrelation) 



        

<hr>



### function linkedRelationOrderingField [2/2]

```C++
QString FeatureModel::linkedRelationOrderingField () const
```



Returns the field name used for ordering in ordered relations. Empty string if not available. 

**See also:** [**setLinkedRelationOrderingField**](classFeatureModel.md#function-setlinkedrelationorderingfield) 



        

<hr>



### function modelMode [2/2]

```C++
ModelModes FeatureModel::modelMode () const
```




<hr>



### function positionLocked 

```C++
bool FeatureModel::positionLocked () const
```



Returns whether the position is locked to the GNSS 


        

<hr>



### function project [2/2]

_Returns the current project from which the digitizing logs will be sought._ 
```C++
inline QgsProject * FeatureModel::project () const
```




<hr>



### function refresh 

```C++
Q_INVOKABLE void FeatureModel::refresh () 
```



Will refresh the feature values and geometry from the data source. 


        

<hr>



### function rememberedAttributes 

```C++
QVector< bool > FeatureModel::rememberedAttributes () const
```




<hr>



### function reset 

```C++
Q_INVOKABLE void FeatureModel::reset () 
```



Will reset the feature to the original values and dismiss any buffered edits. 


        

<hr>



### function resetAttributes 

```C++
Q_INVOKABLE void FeatureModel::resetAttributes (
    bool partialReset=false
) 
```



Resets the attribute values of the current feature 

**Parameters:**


* `partialReset` when set to TRUE, only attributes with default or remembered values will be reset 




        

<hr>



### function resetFeature 

```C++
Q_INVOKABLE void FeatureModel::resetFeature () 
```




<hr>



### function resetFeatureId 

```C++
Q_INVOKABLE void FeatureModel::resetFeatureId () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > FeatureModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int FeatureModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function save 

```C++
Q_INVOKABLE bool FeatureModel::save (
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
void FeatureModel::setAppExpressionContextScopesGenerator (
    AppExpressionContextScopesGenerator * generator
) 
```



Sets the application expression context scopes _generator_ object 


        

<hr>



### function setBatchMode 

```C++
void FeatureModel::setBatchMode (
    bool batchMode
) 
```



Toggles the feature model batch mode. When enabled, the vector layer will remain in editing mode until batch mode is disabled. 


        

<hr>



### function setCurrentLayer 

```C++
void FeatureModel::setCurrentLayer (
    QgsVectorLayer * layer
) 
```




<hr>



### function setData 

```C++
bool FeatureModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setFeature 

```C++
void FeatureModel::setFeature (
    const QgsFeature & feature
) 
```




<hr>



### function setFeatures 

```C++
void FeatureModel::setFeatures (
    const QList< QgsFeature > & features
) 
```




<hr>



### function setGeometry 

```C++
void FeatureModel::setGeometry (
    Geometry * geometry
) 
```



Sets the geometry object that will drive the feature geometry. 

**Note:**

This is not the QgsGeometry of the feature. To change that, use the changeGeometry function. 





        

<hr>



### function setLinkedParentFeature 

```C++
void FeatureModel::setLinkedParentFeature (
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
void FeatureModel::setLinkedRelation (
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
void FeatureModel::setLinkedRelationOrderingField (
    const QString & orderingField
) 
```



Sets the field name used for ordering in ordered relations. Empty string if not available. 

**See also:** linkedRelationOrderingField 



        

<hr>



### function setModelMode 

```C++
void FeatureModel::setModelMode (
    const ModelModes mode
) 
```




<hr>



### function setPositionLocked 

```C++
void FeatureModel::setPositionLocked (
    bool positionLocked
) 
```



Sets whether the position is locked to the GNSS 


        

<hr>



### function setProject 

_Sets the_ _project_ _used to find the digitizing logs layer._
```C++
void FeatureModel::setProject (
    QgsProject * project
) 
```




<hr>



### function setTopSnappingResult 

```C++
void FeatureModel::setTopSnappingResult (
    const SnappingResult & topSnappingResult
) 
```



Sets the top snapping result of the coordinate locator 

**Parameters:**


* `topSnappingResult` the top snapping result object 




        

<hr>



### function setVertexModel 

_Sets the vertex_ _model_ _is used to highlight vertices on the map._
```C++
void FeatureModel::setVertexModel (
    VertexModel * model
) 
```




<hr>



### function suppressFeatureForm 

```C++
Q_INVOKABLE bool FeatureModel::suppressFeatureForm () const
```



Returns whether the feature form should be suppressed when adding new features. 


        

<hr>



### function topSnappingResult [2/2]

```C++
SnappingResult FeatureModel::topSnappingResult () const
```



Returns the top snapping result of the coordinate locator 


        

<hr>



### function updateAttributesFromFeature 

```C++
Q_INVOKABLE bool FeatureModel::updateAttributesFromFeature (
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
Q_INVOKABLE void FeatureModel::updateRubberband () const
```




<hr>



### function vertexModel [2/2]

_Returns the vertex model is used to highlight vertices on the map._ 
```C++
VertexModel * FeatureModel::vertexModel () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featuremodel.h`

