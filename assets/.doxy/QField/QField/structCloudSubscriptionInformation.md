

# Struct CloudSubscriptionInformation



[**ClassList**](annotated.md) **>** [**CloudSubscriptionInformation**](structCloudSubscriptionInformation.md)


































## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**plan**](structCloudSubscriptionInformation.md#property-plan)  <br> |
| property QString | [**status**](structCloudSubscriptionInformation.md#property-status)  <br> |
| property double | [**storageTotal**](structCloudSubscriptionInformation.md#property-storagetotal)  <br> |
| property double | [**storageUsed**](structCloudSubscriptionInformation.md#property-storageused)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CloudSubscriptionInformation**](#function-cloudsubscriptioninformation-12) () = default<br> |
|   | [**CloudSubscriptionInformation**](#function-cloudsubscriptioninformation-22) (const QJsonObject & subscriptionInformation) <br> |
|  bool | [**operator==**](#function-operator) (const [**CloudSubscriptionInformation**](structCloudSubscriptionInformation.md) & other) const<br> |




























## Public Properties Documentation




### property plan 

```C++
QString CloudSubscriptionInformation::plan;
```




<hr>



### property status 

```C++
QString CloudSubscriptionInformation::status;
```




<hr>



### property storageTotal 

```C++
double CloudSubscriptionInformation::storageTotal;
```




<hr>



### property storageUsed 

```C++
double CloudSubscriptionInformation::storageUsed;
```




<hr>
## Public Functions Documentation




### function CloudSubscriptionInformation [1/2]

```C++
CloudSubscriptionInformation::CloudSubscriptionInformation () = default
```




<hr>



### function CloudSubscriptionInformation [2/2]

```C++
inline explicit CloudSubscriptionInformation::CloudSubscriptionInformation (
    const QJsonObject & subscriptionInformation
) 
```




<hr>



### function operator== 

```C++
inline bool CloudSubscriptionInformation::operator== (
    const CloudSubscriptionInformation & other
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfieldcloudutils.h`

