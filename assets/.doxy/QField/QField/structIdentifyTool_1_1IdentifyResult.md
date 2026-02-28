

# Struct IdentifyTool::IdentifyResult



[**ClassList**](annotated.md) **>** [**IdentifyTool**](classIdentifyTool.md) **>** [**IdentifyResult**](structIdentifyTool_1_1IdentifyResult.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  QgsFeature | [**feature**](#variable-feature)  <br> |
|  QgsMapLayer \* | [**layer**](#variable-layer)  <br> |
|  QString | [**representationalLayerName**](#variable-representationallayername)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IdentifyResult**](#function-identifyresult) (QgsMapLayer \* layer, const QgsFeature & feature, const QString & representationalLayerName=QString()) <br> |




























## Public Attributes Documentation




### variable feature 

```C++
QgsFeature IdentifyTool::IdentifyResult::feature;
```




<hr>



### variable layer 

```C++
QgsMapLayer* IdentifyTool::IdentifyResult::layer;
```




<hr>



### variable representationalLayerName 

```C++
QString IdentifyTool::IdentifyResult::representationalLayerName;
```




<hr>
## Public Functions Documentation




### function IdentifyResult 

```C++
inline IdentifyTool::IdentifyResult::IdentifyResult (
    QgsMapLayer * layer,
    const QgsFeature & feature,
    const QString & representationalLayerName=QString()
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/identifytool.h`

