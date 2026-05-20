

# Class NtripSourceTableFetcher



[**ClassList**](annotated.md) **>** [**NtripSourceTableFetcher**](classNtripSourceTableFetcher.md)



_A class to fetch information such as mount points from an NTRIP server's source table._ 

* `#include <ntripsourcetablefetcher.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isFetching**](classNtripSourceTableFetcher.md#property-isfetching-12)  <br> |
| property QList&lt; [**NtripMountPoint**](classNtripMountPoint.md) &gt; | [**mountPoints**](classNtripSourceTableFetcher.md#property-mountpoints-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**fetchError**](classNtripSourceTableFetcher.md#signal-fetcherror) (const QString & message) <br>_Emitted when an error has occured during a fetching operation._  |
| signal void | [**isFetchingChanged**](classNtripSourceTableFetcher.md#signal-isfetchingchanged)  <br>_Emitted when a fetching operation has begun or ended._  |
| signal void | [**mountPointsChanged**](classNtripSourceTableFetcher.md#signal-mountpointschanged)  <br>_Emitted when the list of collected mount points has changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**NtripSourceTableFetcher**](#function-ntripsourcetablefetcher) (QObject \* parent=nullptr) <br>_The NTRIP source table fetcher constructor._  |
|  Q\_INVOKABLE void | [**cancel**](#function-cancel) () <br>_Cancels any ongoing fetching operation._  |
|  Q\_INVOKABLE void | [**fetch**](#function-fetch) (const [**NtripSettings**](classNtripSettings.md) & ntripSettings) <br> |
|  bool | [**isFetching**](#function-isfetching-22) () const<br>_Returns TRUE if on ongoing source table fetching operation is ongoing._  |
|  QList&lt; [**NtripMountPoint**](classNtripMountPoint.md) &gt; | [**mountPoints**](#function-mountpoints-22) () const<br>_Returns the mount points collected during the last source table fetching operation._  |
|   | [**~NtripSourceTableFetcher**](#function-ntripsourcetablefetcher) () noexcept override<br>_The NTRIP source table fetcher destructor._  |




























## Public Properties Documentation




### property isFetching [1/2]

```C++
bool NtripSourceTableFetcher::isFetching;
```




<hr>



### property mountPoints [1/2]

```C++
QList<NtripMountPoint> NtripSourceTableFetcher::mountPoints;
```




<hr>
## Public Signals Documentation




### signal fetchError 

_Emitted when an error has occured during a fetching operation._ 
```C++
void NtripSourceTableFetcher::fetchError;
```




<hr>



### signal isFetchingChanged 

_Emitted when a fetching operation has begun or ended._ 
```C++
void NtripSourceTableFetcher::isFetchingChanged;
```




<hr>



### signal mountPointsChanged 

_Emitted when the list of collected mount points has changed._ 
```C++
void NtripSourceTableFetcher::mountPointsChanged;
```




<hr>
## Public Functions Documentation




### function NtripSourceTableFetcher 

_The NTRIP source table fetcher constructor._ 
```C++
explicit NtripSourceTableFetcher::NtripSourceTableFetcher (
    QObject * parent=nullptr
) 
```




<hr>



### function cancel 

_Cancels any ongoing fetching operation._ 
```C++
Q_INVOKABLE void NtripSourceTableFetcher::cancel () 
```




<hr>



### function fetch 

```C++
Q_INVOKABLE void NtripSourceTableFetcher::fetch (
    const NtripSettings & ntripSettings
) 
```



Fetches details from an NTRIP server source table 

**Parameters:**


* `ntripSettings` The NTRIP settings of the server 




        

<hr>



### function isFetching [2/2]

_Returns TRUE if on ongoing source table fetching operation is ongoing._ 
```C++
inline bool NtripSourceTableFetcher::isFetching () const
```




<hr>



### function mountPoints [2/2]

_Returns the mount points collected during the last source table fetching operation._ 
```C++
inline QList< NtripMountPoint > NtripSourceTableFetcher::mountPoints () const
```




<hr>



### function ~NtripSourceTableFetcher 

_The NTRIP source table fetcher destructor._ 
```C++
NtripSourceTableFetcher::~NtripSourceTableFetcher () noexcept override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/ntripsourcetablefetcher.h`

