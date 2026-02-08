

# Class LegendImageProvider



[**ClassList**](annotated.md) **>** [**LegendImageProvider**](classLegendImageProvider.md)



_This class provides legend images for the layer tree model._ 

* `#include <legendimageprovider.h>`



Inherits the following classes: QQuickImageProvider


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LegendImageProvider**](#function-legendimageprovider) (QgsLayerTreeModel \* layerTreeModel) <br> |
|  QPixmap | [**requestPixmap**](#function-requestpixmap) (const QString & id, QSize \* size, const QSize & requestedSize) override<br> |




























## Public Functions Documentation




### function LegendImageProvider 

```C++
explicit LegendImageProvider::LegendImageProvider (
    QgsLayerTreeModel * layerTreeModel
) 
```




<hr>



### function requestPixmap 

```C++
QPixmap LegendImageProvider::requestPixmap (
    const QString & id,
    QSize * size,
    const QSize & requestedSize
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/legendimageprovider.h`

