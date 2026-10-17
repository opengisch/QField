

# Class QfNtripSourceTableFetcher



[**ClassList**](annotated.md) **>** [**QfNtripSourceTableFetcher**](classQfNtripSourceTableFetcher.md)



_A class to fetch information such as mount points from an NTRIP server's source table._ 

* `#include <qfntripsourcetablefetcher.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isFetching**](classQfNtripSourceTableFetcher.md#property-isfetching-12)  <br> |
| property QList&lt; [**QfNtripMountPoint**](classQfNtripMountPoint.md) &gt; | [**mountPoints**](classQfNtripSourceTableFetcher.md#property-mountpoints-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**fetchError**](classQfNtripSourceTableFetcher.md#signal-fetcherror) (const QString & message) <br>_Emitted when an error has occured during a fetching operation._  |
| signal void | [**isFetchingChanged**](classQfNtripSourceTableFetcher.md#signal-isfetchingchanged)  <br>_Emitted when a fetching operation has begun or ended._  |
| signal void | [**mountPointsChanged**](classQfNtripSourceTableFetcher.md#signal-mountpointschanged)  <br>_Emitted when the list of collected mount points has changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfNtripSourceTableFetcher**](#function-qfntripsourcetablefetcher) (QObject \* parent=nullptr) <br>_The NTRIP source table fetcher constructor._  |
|  Q\_INVOKABLE void | [**cancel**](#function-cancel) () <br>_Cancels any ongoing fetching operation._  |
|  Q\_INVOKABLE void | [**fetch**](#function-fetch) (const [**QfNtripSettings**](classQfNtripSettings.md) & ntripSettings) <br> |
|  bool | [**isFetching**](#function-isfetching-22) () const<br>_Returns TRUE if on ongoing source table fetching operation is ongoing._  |
|  QList&lt; [**QfNtripMountPoint**](classQfNtripMountPoint.md) &gt; | [**mountPoints**](#function-mountpoints-22) () const<br>_Returns the mount points collected during the last source table fetching operation._  |
|   | [**~QfNtripSourceTableFetcher**](#function-qfntripsourcetablefetcher) () noexcept override<br>_The NTRIP source table fetcher destructor._  |




























## Public Properties Documentation




### property isFetching [1/2]

```C++
bool QfNtripSourceTableFetcher::isFetching;
```




<hr>



### property mountPoints [1/2]

```C++
QList<QfNtripMountPoint> QfNtripSourceTableFetcher::mountPoints;
```




<hr>
## Public Signals Documentation




### signal fetchError 

_Emitted when an error has occured during a fetching operation._ 
```C++
void QfNtripSourceTableFetcher::fetchError;
```




<hr>



### signal isFetchingChanged 

_Emitted when a fetching operation has begun or ended._ 
```C++
void QfNtripSourceTableFetcher::isFetchingChanged;
```




<hr>



### signal mountPointsChanged 

_Emitted when the list of collected mount points has changed._ 
```C++
void QfNtripSourceTableFetcher::mountPointsChanged;
```




<hr>
## Public Functions Documentation




### function QfNtripSourceTableFetcher 

_The NTRIP source table fetcher constructor._ 
```C++
explicit QfNtripSourceTableFetcher::QfNtripSourceTableFetcher (
    QObject * parent=nullptr
) 
```




<hr>



### function cancel 

_Cancels any ongoing fetching operation._ 
```C++
Q_INVOKABLE void QfNtripSourceTableFetcher::cancel () 
```




<hr>



### function fetch 

```C++
Q_INVOKABLE void QfNtripSourceTableFetcher::fetch (
    const QfNtripSettings & ntripSettings
) 
```



Fetches details from an NTRIP server source table 

**Parameters:**


* `ntripSettings` The NTRIP settings of the server 




        

<hr>



### function isFetching [2/2]

_Returns TRUE if on ongoing source table fetching operation is ongoing._ 
```C++
inline bool QfNtripSourceTableFetcher::isFetching () const
```




<hr>



### function mountPoints [2/2]

_Returns the mount points collected during the last source table fetching operation._ 
```C++
inline QList< QfNtripMountPoint > QfNtripSourceTableFetcher::mountPoints () const
```




<hr>



### function ~QfNtripSourceTableFetcher 

_The NTRIP source table fetcher destructor._ 
```C++
QfNtripSourceTableFetcher::~QfNtripSourceTableFetcher () noexcept override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfntripsourcetablefetcher.h`

