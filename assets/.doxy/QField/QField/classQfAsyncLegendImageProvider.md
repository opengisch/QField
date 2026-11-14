

# Class QfAsyncLegendImageProvider



[**ClassList**](annotated.md) **>** [**QfAsyncLegendImageProvider**](classQfAsyncLegendImageProvider.md)



_This class prepares asynchrnous legend images requests. Used for online WMS and AMS layers._ 

* `#include <qflegendimageprovider.h>`



Inherits the following classes: QQuickAsyncImageProvider


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAsyncLegendImageProvider**](#function-qfasynclegendimageprovider) (QgsLayerTreeModel \* layerTreeModel) <br> |
|  QQuickImageResponse \* | [**requestImageResponse**](#function-requestimageresponse) (const QString & id, const QSize & requestedSize) override<br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |




























## Public Functions Documentation




### function QfAsyncLegendImageProvider 

```C++
explicit QfAsyncLegendImageProvider::QfAsyncLegendImageProvider (
    QgsLayerTreeModel * layerTreeModel
) 
```




<hr>



### function requestImageResponse 

```C++
QQuickImageResponse * QfAsyncLegendImageProvider::requestImageResponse (
    const QString & id,
    const QSize & requestedSize
) override
```




<hr>



### function setMapSettings 

```C++
void QfAsyncLegendImageProvider::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qflegendimageprovider.h`

