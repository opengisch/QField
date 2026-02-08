

# Struct ReferencingFeatureListModelBase::Entry



[**ClassList**](annotated.md) **>** [**Entry**](structReferencingFeatureListModelBase_1_1Entry.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  QString | [**displayString**](#variable-displaystring)  <br> |
|  QString | [**nmDisplayString**](#variable-nmdisplaystring)  <br> |
|  QgsFeature | [**nmReferencedFeature**](#variable-nmreferencedfeature)  <br> |
|  QgsFeature | [**referencingFeature**](#variable-referencingfeature)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Entry**](#function-entry-12) (const QString & displayString, const QgsFeature & referencingFeature, const QString & nmDisplayString=QString(), const QgsFeature & nmReferencedFeature=QgsFeature()) <br> |
|   | [**Entry**](#function-entry-22) () = default<br> |




























## Public Attributes Documentation




### variable displayString 

```C++
QString ReferencingFeatureListModelBase::Entry::displayString;
```




<hr>



### variable nmDisplayString 

```C++
QString ReferencingFeatureListModelBase::Entry::nmDisplayString;
```




<hr>



### variable nmReferencedFeature 

```C++
QgsFeature ReferencingFeatureListModelBase::Entry::nmReferencedFeature;
```




<hr>



### variable referencingFeature 

```C++
QgsFeature ReferencingFeatureListModelBase::Entry::referencingFeature;
```




<hr>
## Public Functions Documentation




### function Entry [1/2]

```C++
inline Entry::Entry (
    const QString & displayString,
    const QgsFeature & referencingFeature,
    const QString & nmDisplayString=QString(),
    const QgsFeature & nmReferencedFeature=QgsFeature()
) 
```




<hr>



### function Entry [2/2]

```C++
Entry::Entry () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/referencingfeaturelistmodel.h`

