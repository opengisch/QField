

# Class QfBadLayerHandler



[**ClassList**](annotated.md) **>** [**QfBadLayerHandler**](classQfBadLayerHandler.md)








Inherits the following classes: QStandardItemModel,  QgsProjectBadLayerHandler














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsProject \* | [**project**](classQfBadLayerHandler.md#property-project-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**badLayersFound**](classQfBadLayerHandler.md#signal-badlayersfound)  <br> |
| signal void | [**projectChanged**](classQfBadLayerHandler.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfBadLayerHandler**](#function-qfbadlayerhandler) (QObject \* parent=nullptr) <br> |
|  void | [**handleBadLayers**](#function-handlebadlayers) (const QList&lt; QDomNode &gt; & layers) override<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br> |




























## Public Types Documentation




### enum Roles 

```C++
enum QfBadLayerHandler::Roles {
    DataSourceRole = Qt::UserRole,
    LayerNameRole
};
```




<hr>
## Public Properties Documentation




### property project [1/2]

```C++
QgsProject * QfBadLayerHandler::project;
```




<hr>
## Public Signals Documentation




### signal badLayersFound 

```C++
void QfBadLayerHandler::badLayersFound;
```




<hr>



### signal projectChanged 

```C++
void QfBadLayerHandler::projectChanged;
```




<hr>
## Public Functions Documentation




### function QfBadLayerHandler 

```C++
explicit QfBadLayerHandler::QfBadLayerHandler (
    QObject * parent=nullptr
) 
```




<hr>



### function handleBadLayers 

```C++
void QfBadLayerHandler::handleBadLayers (
    const QList< QDomNode > & layers
) override
```




<hr>



### function project [2/2]

```C++
QgsProject * QfBadLayerHandler::project () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfBadLayerHandler::roleNames () override const
```




<hr>



### function setProject 

```C++
void QfBadLayerHandler::setProject (
    QgsProject * project
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qfbadlayerhandler.h`

