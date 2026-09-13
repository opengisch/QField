

# Struct QfCloudSubscriptionInformation



[**ClassList**](annotated.md) **>** [**QfCloudSubscriptionInformation**](structQfCloudSubscriptionInformation.md)


































## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**plan**](structQfCloudSubscriptionInformation.md#property-plan)  <br> |
| property QString | [**status**](structQfCloudSubscriptionInformation.md#property-status)  <br> |
| property double | [**storageThresholdCritical**](structQfCloudSubscriptionInformation.md#property-storagethresholdcritical)  <br> |
| property double | [**storageThresholdWarning**](structQfCloudSubscriptionInformation.md#property-storagethresholdwarning)  <br> |
| property double | [**storageTotal**](structQfCloudSubscriptionInformation.md#property-storagetotal)  <br> |
| property double | [**storageUsed**](structQfCloudSubscriptionInformation.md#property-storageused)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCloudSubscriptionInformation**](#function-qfcloudsubscriptioninformation-12) () = default<br> |
|   | [**QfCloudSubscriptionInformation**](#function-qfcloudsubscriptioninformation-22) (const QJsonObject & subscriptionInformation) <br> |
|  bool | [**operator==**](#function-operator) (const [**QfCloudSubscriptionInformation**](structQfCloudSubscriptionInformation.md) & other) const<br> |




























## Public Properties Documentation




### property plan 

```C++
QString QfCloudSubscriptionInformation::plan;
```




<hr>



### property status 

```C++
QString QfCloudSubscriptionInformation::status;
```




<hr>



### property storageThresholdCritical 

```C++
double QfCloudSubscriptionInformation::storageThresholdCritical;
```




<hr>



### property storageThresholdWarning 

```C++
double QfCloudSubscriptionInformation::storageThresholdWarning;
```




<hr>



### property storageTotal 

```C++
double QfCloudSubscriptionInformation::storageTotal;
```




<hr>



### property storageUsed 

```C++
double QfCloudSubscriptionInformation::storageUsed;
```




<hr>
## Public Functions Documentation




### function QfCloudSubscriptionInformation [1/2]

```C++
QfCloudSubscriptionInformation::QfCloudSubscriptionInformation () = default
```




<hr>



### function QfCloudSubscriptionInformation [2/2]

```C++
inline explicit QfCloudSubscriptionInformation::QfCloudSubscriptionInformation (
    const QJsonObject & subscriptionInformation
) 
```




<hr>



### function operator== 

```C++
inline bool QfCloudSubscriptionInformation::operator== (
    const QfCloudSubscriptionInformation & other
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfcloudutils.h`

