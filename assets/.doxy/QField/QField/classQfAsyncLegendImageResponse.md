

# Class QfAsyncLegendImageResponse



[**ClassList**](annotated.md) **>** [**QfAsyncLegendImageResponse**](classQfAsyncLegendImageResponse.md)



_This class provides responses of asynchronously requested legend images for the layer tree model . Used for online WMS and AMS layers._ 

* `#include <qflegendimageprovider.h>`



Inherits the following classes: QQuickImageResponse


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAsyncLegendImageResponse**](#function-qfasynclegendimageresponse) (QgsRasterDataProvider \* dataProvider=nullptr, const QgsMapSettings \* mapSettings=nullptr) <br> |
|  QQuickTextureFactory \* | [**textureFactory**](#function-texturefactory) () override const<br> |
|   | [**~QfAsyncLegendImageResponse**](#function-qfasynclegendimageresponse) () <br> |




























## Public Functions Documentation




### function QfAsyncLegendImageResponse 

```C++
QfAsyncLegendImageResponse::QfAsyncLegendImageResponse (
    QgsRasterDataProvider * dataProvider=nullptr,
    const QgsMapSettings * mapSettings=nullptr
) 
```




<hr>



### function textureFactory 

```C++
QQuickTextureFactory * QfAsyncLegendImageResponse::textureFactory () override const
```




<hr>



### function ~QfAsyncLegendImageResponse 

```C++
QfAsyncLegendImageResponse::~QfAsyncLegendImageResponse () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qflegendimageprovider.h`

