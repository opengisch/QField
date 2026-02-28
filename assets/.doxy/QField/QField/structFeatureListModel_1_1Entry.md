

# Struct FeatureListModel::Entry



[**ClassList**](annotated.md) **>** [**Entry**](structFeatureListModel_1_1Entry.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  QString | [**displayString**](#variable-displaystring)  <br> |
|  QgsFeatureId | [**fid**](#variable-fid)  <br> |
|  double | [**fuzzyScore**](#variable-fuzzyscore)   = `0.0`<br> |
|  QVariant | [**group**](#variable-group)  <br> |
|  QVariant | [**key**](#variable-key)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Entry**](#function-entry-12) (const QString & displayString, const QVariant & key, const QVariant & group, const QgsFeatureId & fid) <br> |
|   | [**Entry**](#function-entry-22) () = default<br> |




























## Public Attributes Documentation




### variable displayString 

```C++
QString FeatureListModel::Entry::displayString;
```




<hr>



### variable fid 

```C++
QgsFeatureId FeatureListModel::Entry::fid;
```




<hr>



### variable fuzzyScore 

```C++
double FeatureListModel::Entry::fuzzyScore;
```




<hr>



### variable group 

```C++
QVariant FeatureListModel::Entry::group;
```




<hr>



### variable key 

```C++
QVariant FeatureListModel::Entry::key;
```




<hr>
## Public Functions Documentation




### function Entry [1/2]

```C++
inline Entry::Entry (
    const QString & displayString,
    const QVariant & key,
    const QVariant & group,
    const QgsFeatureId & fid
) 
```




<hr>



### function Entry [2/2]

```C++
Entry::Entry () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featurelistmodel.h`

