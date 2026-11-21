

# Struct QfIdentifyTool::IdentifyResult



[**ClassList**](annotated.md) **>** [**QfIdentifyTool**](classQfIdentifyTool.md) **>** [**IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  QgsFeature | [**feature**](#variable-feature)  <br> |
|  QgsMapLayer \* | [**layer**](#variable-layer)   = `nullptr`<br> |
|  QString | [**layerId**](#variable-layerid)  <br> |
|  QString | [**representationalLayerName**](#variable-representationallayername)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IdentifyResult**](#function-identifyresult) (const QString & layerId, const QgsFeature & feature, const QString & representationalLayerName=QString()) <br> |




























## Public Attributes Documentation




### variable feature 

```C++
QgsFeature QfIdentifyTool::IdentifyResult::feature;
```




<hr>



### variable layer 

```C++
QgsMapLayer* QfIdentifyTool::IdentifyResult::layer;
```




<hr>



### variable layerId 

```C++
QString QfIdentifyTool::IdentifyResult::layerId;
```




<hr>



### variable representationalLayerName 

```C++
QString QfIdentifyTool::IdentifyResult::representationalLayerName;
```




<hr>
## Public Functions Documentation




### function IdentifyResult 

```C++
inline QfIdentifyTool::IdentifyResult::IdentifyResult (
    const QString & layerId,
    const QgsFeature & feature,
    const QString & representationalLayerName=QString()
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfidentifytool.h`

