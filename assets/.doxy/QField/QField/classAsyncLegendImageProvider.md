

# Class AsyncLegendImageProvider



[**ClassList**](annotated.md) **>** [**AsyncLegendImageProvider**](classAsyncLegendImageProvider.md)



_This class prepares asynchrnous legend images requests. Used for online WMS and AMS layers._ 

* `#include <legendimageprovider.h>`



Inherits the following classes: QQuickAsyncImageProvider


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AsyncLegendImageProvider**](#function-asynclegendimageprovider) (QgsLayerTreeModel \* layerTreeModel) <br> |
|  QQuickImageResponse \* | [**requestImageResponse**](#function-requestimageresponse) (const QString & id, const QSize & requestedSize) override<br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |




























## Public Functions Documentation




### function AsyncLegendImageProvider 

```C++
explicit AsyncLegendImageProvider::AsyncLegendImageProvider (
    QgsLayerTreeModel * layerTreeModel
) 
```




<hr>



### function requestImageResponse 

```C++
QQuickImageResponse * AsyncLegendImageProvider::requestImageResponse (
    const QString & id,
    const QSize & requestedSize
) override
```




<hr>



### function setMapSettings 

```C++
void AsyncLegendImageProvider::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/legendimageprovider.h`

