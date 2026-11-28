

# Struct QfIdentifyTool::PreparedLayer



[**ClassList**](annotated.md) **>** [**PreparedLayer**](structQfIdentifyTool_1_1PreparedLayer.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  QgsRectangle | [**extent**](#variable-extent)  <br> |
|  QgsFields | [**fields**](#variable-fields)  <br> |
|  bool | [**isValid**](#variable-isvalid)   = `false`<br> |
|  QString | [**layerId**](#variable-layerid)  <br> |
|  QString | [**layerName**](#variable-layername)  <br> |
|  Qgis::LayerType | [**layerType**](#variable-layertype)  <br> |
|  std::unique\_ptr&lt; QgsRasterDataProvider &gt; | [**rasterDataProvider**](#variable-rasterdataprovider)  <br> |
|  int | [**rasterHeight**](#variable-rasterheight)   = `1`<br> |
|  QgsPointXY | [**rasterPoint**](#variable-rasterpoint)  <br> |
|  int | [**rasterWidth**](#variable-rasterwidth)   = `1`<br> |
|  QgsRenderContext | [**renderContext**](#variable-rendercontext)  <br> |
|  QgsFeatureRequest | [**request**](#variable-request)  <br> |
|  std::unique\_ptr&lt; QgsVectorLayerFeatureSource &gt; | [**vectorFeatureSource**](#variable-vectorfeaturesource)  <br> |
|  std::unique\_ptr&lt; QgsFeatureRenderer &gt; | [**vectorRenderer**](#variable-vectorrenderer)  <br> |
|  std::unique\_ptr&lt; QgsVectorTileDataProvider &gt; | [**vectorTileDataProvider**](#variable-vectortiledataprovider)  <br> |
|  double | [**vectorTileScale**](#variable-vectortilescale)   = `0.0`<br> |












































## Public Attributes Documentation




### variable extent 

```C++
QgsRectangle QfIdentifyTool::PreparedLayer::extent;
```




<hr>



### variable fields 

```C++
QgsFields QfIdentifyTool::PreparedLayer::fields;
```




<hr>



### variable isValid 

```C++
bool QfIdentifyTool::PreparedLayer::isValid;
```




<hr>



### variable layerId 

```C++
QString QfIdentifyTool::PreparedLayer::layerId;
```




<hr>



### variable layerName 

```C++
QString QfIdentifyTool::PreparedLayer::layerName;
```




<hr>



### variable layerType 

```C++
Qgis::LayerType QfIdentifyTool::PreparedLayer::layerType;
```




<hr>



### variable rasterDataProvider 

```C++
std::unique_ptr<QgsRasterDataProvider> QfIdentifyTool::PreparedLayer::rasterDataProvider;
```




<hr>



### variable rasterHeight 

```C++
int QfIdentifyTool::PreparedLayer::rasterHeight;
```




<hr>



### variable rasterPoint 

```C++
QgsPointXY QfIdentifyTool::PreparedLayer::rasterPoint;
```




<hr>



### variable rasterWidth 

```C++
int QfIdentifyTool::PreparedLayer::rasterWidth;
```




<hr>



### variable renderContext 

```C++
QgsRenderContext QfIdentifyTool::PreparedLayer::renderContext;
```




<hr>



### variable request 

```C++
QgsFeatureRequest QfIdentifyTool::PreparedLayer::request;
```




<hr>



### variable vectorFeatureSource 

```C++
std::unique_ptr<QgsVectorLayerFeatureSource> QfIdentifyTool::PreparedLayer::vectorFeatureSource;
```




<hr>



### variable vectorRenderer 

```C++
std::unique_ptr<QgsFeatureRenderer> QfIdentifyTool::PreparedLayer::vectorRenderer;
```




<hr>



### variable vectorTileDataProvider 

```C++
std::unique_ptr<QgsVectorTileDataProvider> QfIdentifyTool::PreparedLayer::vectorTileDataProvider;
```




<hr>



### variable vectorTileScale 

```C++
double QfIdentifyTool::PreparedLayer::vectorTileScale;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfidentifytool.h`

