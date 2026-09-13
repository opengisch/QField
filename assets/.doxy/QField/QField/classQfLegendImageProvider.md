

# Class QfLegendImageProvider



[**ClassList**](annotated.md) **>** [**QfLegendImageProvider**](classQfLegendImageProvider.md)



_This class provides legend images for the layer tree model._ 

* `#include <qflegendimageprovider.h>`



Inherits the following classes: QQuickImageProvider


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLegendImageProvider**](#function-qflegendimageprovider) (QgsLayerTreeModel \* layerTreeModel) <br> |
|  QPixmap | [**requestPixmap**](#function-requestpixmap) (const QString & id, QSize \* size, const QSize & requestedSize) override<br> |




























## Public Functions Documentation




### function QfLegendImageProvider 

```C++
explicit QfLegendImageProvider::QfLegendImageProvider (
    QgsLayerTreeModel * layerTreeModel
) 
```




<hr>



### function requestPixmap 

```C++
QPixmap QfLegendImageProvider::requestPixmap (
    const QString & id,
    QSize * size,
    const QSize & requestedSize
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qflegendimageprovider.h`

