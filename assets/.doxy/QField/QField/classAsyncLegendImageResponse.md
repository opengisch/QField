

# Class AsyncLegendImageResponse



[**ClassList**](annotated.md) **>** [**AsyncLegendImageResponse**](classAsyncLegendImageResponse.md)



_This class provides responses of asynchronously requested legend images for the layer tree model . Used for online WMS and AMS layers._ 

* `#include <legendimageprovider.h>`



Inherits the following classes: QQuickImageResponse


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AsyncLegendImageResponse**](#function-asynclegendimageresponse) (QgsRasterDataProvider \* dataProvider=nullptr, const QgsMapSettings \* mapSettings=nullptr) <br> |
|  QQuickTextureFactory \* | [**textureFactory**](#function-texturefactory) () override const<br> |




























## Public Functions Documentation




### function AsyncLegendImageResponse 

```C++
AsyncLegendImageResponse::AsyncLegendImageResponse (
    QgsRasterDataProvider * dataProvider=nullptr,
    const QgsMapSettings * mapSettings=nullptr
) 
```




<hr>



### function textureFactory 

```C++
QQuickTextureFactory * AsyncLegendImageResponse::textureFactory () override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/legendimageprovider.h`

