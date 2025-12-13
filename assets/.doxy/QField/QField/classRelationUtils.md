

# Class RelationUtils



[**ClassList**](annotated.md) **>** [**RelationUtils**](classRelationUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RelationUtils**](#function-relationutils) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QgsRelation | [**createRelation**](#function-createrelation) (const QgsVectorLayer \* referencedLayer, const QgsVectorLayer \* referencingLayer, const QVariantMap & fieldPairs) <br> |
|  Q\_INVOKABLE QgsRelation | [**resolveReferencingRelation**](#function-resolvereferencingrelation) (QgsProject \* project, QgsVectorLayer \* layer, const QString & fieldName, const QString & relationId) <br> |




























## Public Functions Documentation




### function RelationUtils 

```C++
explicit RelationUtils::RelationUtils (
    QObject * parent=nullptr
) 
```




<hr>



### function createRelation 

```C++
Q_INVOKABLE QgsRelation RelationUtils::createRelation (
    const QgsVectorLayer * referencedLayer,
    const QgsVectorLayer * referencingLayer,
    const QVariantMap & fieldPairs
) 
```



Creates a relation between referenced and referencing layers. Note: Layers must be added to the project first




**Parameters:**


* `referencedLayer` The referenced (parent) layer 
* `referencingLayer` The referencing (child) layer 
* `fieldPairs` A map where keys are referenced field names and values are referencing field names 



**Returns:**

A QgsRelation object 





        

<hr>



### function resolveReferencingRelation 

```C++
Q_INVOKABLE QgsRelation RelationUtils::resolveReferencingRelation (
    QgsProject * project,
    QgsVectorLayer * layer,
    const QString & fieldName,
    const QString & relationId
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/relationutils.h`

