

# Struct FeatureHistory::FeatureModifications



[**ClassList**](annotated.md) **>** [**FeatureHistory**](classFeatureHistory.md) **>** [**FeatureModifications**](structFeatureHistory_1_1FeatureModifications.md)



[More...](#detailed-description)

* `#include <featurehistory.h>`





















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
QList<OldNewFeaturePair> FeatureHistory::FeatureModifications::createdFeatures;
```




<hr>



### variable deletedFeatures 

```C++
QList<OldNewFeaturePair> FeatureHistory::FeatureModifications::deletedFeatures;
```




<hr>



### variable updatedFeatures 

```C++
QList<OldNewFeaturePair> FeatureHistory::FeatureModifications::updatedFeatures;
```




<hr>
## Public Functions Documentation




### function FeatureModifications 

```C++
inline FeatureHistory::FeatureModifications::FeatureModifications () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featurehistory.h`

