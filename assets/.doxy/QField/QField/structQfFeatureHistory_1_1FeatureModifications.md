

# Struct QfFeatureHistory::FeatureModifications



[**ClassList**](annotated.md) **>** [**QfFeatureHistory**](classQfFeatureHistory.md) **>** [**FeatureModifications**](structQfFeatureHistory_1_1FeatureModifications.md)



[More...](#detailed-description)

* `#include <qffeaturehistory.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  QList&lt; OldNewFeaturePair &gt; | [**createdFeatures**](#variable-createdfeatures)  <br> |
|  QList&lt; OldNewFeaturePair &gt; | [**deletedFeatures**](#variable-deletedfeatures)  <br> |
|  QList&lt; OldNewFeaturePair &gt; | [**updatedFeatures**](#variable-updatedfeatures)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeatureModifications**](#function-featuremodifications) () <br> |




























## Detailed Description


Stores the created, updated and deleted features on each undo/redo step. 


    
## Public Attributes Documentation




### variable createdFeatures 

```C++
QList<OldNewFeaturePair> QfFeatureHistory::FeatureModifications::createdFeatures;
```




<hr>



### variable deletedFeatures 

```C++
QList<OldNewFeaturePair> QfFeatureHistory::FeatureModifications::deletedFeatures;
```




<hr>



### variable updatedFeatures 

```C++
QList<OldNewFeaturePair> QfFeatureHistory::FeatureModifications::updatedFeatures;
```




<hr>
## Public Functions Documentation




### function FeatureModifications 

```C++
inline QfFeatureHistory::FeatureModifications::FeatureModifications () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qffeaturehistory.h`

