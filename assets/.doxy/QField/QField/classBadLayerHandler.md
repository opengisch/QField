

# Class BadLayerHandler



[**ClassList**](annotated.md) **>** [**BadLayerHandler**](classBadLayerHandler.md)








Inherits the following classes: QStandardItemModel,  QgsProjectBadLayerHandler














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsProject \* | [**project**](classBadLayerHandler.md#property-project-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**badLayersFound**](classBadLayerHandler.md#signal-badlayersfound)  <br> |
| signal void | [**projectChanged**](classBadLayerHandler.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BadLayerHandler**](#function-badlayerhandler) (QObject \* parent=nullptr) <br> |
|  void | [**handleBadLayers**](#function-handlebadlayers) (const QList&lt; QDomNode &gt; & layers) override<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br> |




























## Public Types Documentation




### enum Roles 

```C++
enum BadLayerHandler::Roles {
    DataSourceRole = Qt::UserRole,
    LayerNameRole
};
```




<hr>
## Public Properties Documentation




### property project [1/2]

```C++
QgsProject * BadLayerHandler::project;
```




<hr>
## Public Signals Documentation




### signal badLayersFound 

```C++
void BadLayerHandler::badLayersFound;
```




<hr>



### signal projectChanged 

```C++
void BadLayerHandler::projectChanged;
```




<hr>
## Public Functions Documentation




### function BadLayerHandler 

```C++
explicit BadLayerHandler::BadLayerHandler (
    QObject * parent=nullptr
) 
```




<hr>



### function handleBadLayers 

```C++
void BadLayerHandler::handleBadLayers (
    const QList< QDomNode > & layers
) override
```




<hr>



### function project [2/2]

```C++
QgsProject * BadLayerHandler::project () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > BadLayerHandler::roleNames () override const
```




<hr>



### function setProject 

```C++
void BadLayerHandler::setProject (
    QgsProject * project
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/badlayerhandler.h`

