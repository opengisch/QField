

# Struct FeatureExpressionValuesGatherer::Entry



[**ClassList**](annotated.md) **>** [**FeatureExpressionValuesGatherer**](classFeatureExpressionValuesGatherer.md) **>** [**Entry**](structFeatureExpressionValuesGatherer_1_1Entry.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  QgsFeature | [**feature**](#variable-feature)  <br> |
|  QgsFeatureId | [**featureId**](#variable-featureid)  <br> |
|  QVariantList | [**identifierFields**](#variable-identifierfields)  <br> |
|  QString | [**value**](#variable-value)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Entry**](#function-entry-13) () = default<br> |
|   | [**Entry**](#function-entry-23) (const QVariantList & \_identifierFields, const QString & \_value, const QgsFeature & \_feature) <br> |
|   | [**Entry**](#function-entry-33) (const QgsFeatureId & \_featureId, const QString & \_value, const QgsVectorLayer \* layer) <br> |
|  bool | [**operator()**](#function-operator) (const [**Entry**](structFeatureExpressionValuesGatherer_1_1Entry.md) & lhs, const [**Entry**](structFeatureExpressionValuesGatherer_1_1Entry.md) & rhs) const<br> |




























## Public Attributes Documentation




### variable feature 

```C++
QgsFeature FeatureExpressionValuesGatherer::Entry::feature;
```




<hr>



### variable featureId 

```C++
QgsFeatureId FeatureExpressionValuesGatherer::Entry::featureId;
```




<hr>



### variable identifierFields 

```C++
QVariantList FeatureExpressionValuesGatherer::Entry::identifierFields;
```




<hr>



### variable value 

```C++
QString FeatureExpressionValuesGatherer::Entry::value;
```




<hr>
## Public Functions Documentation




### function Entry [1/3]

```C++
FeatureExpressionValuesGatherer::Entry::Entry () = default
```




<hr>



### function Entry [2/3]

```C++
inline FeatureExpressionValuesGatherer::Entry::Entry (
    const QVariantList & _identifierFields,
    const QString & _value,
    const QgsFeature & _feature
) 
```




<hr>



### function Entry [3/3]

```C++
inline FeatureExpressionValuesGatherer::Entry::Entry (
    const QgsFeatureId & _featureId,
    const QString & _value,
    const QgsVectorLayer * layer
) 
```




<hr>



### function operator() 

```C++
bool FeatureExpressionValuesGatherer::Entry::operator() (
    const Entry & lhs,
    const Entry & rhs
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featureexpressionvaluesgatherer.h`

