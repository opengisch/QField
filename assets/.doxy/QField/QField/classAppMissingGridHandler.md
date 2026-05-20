

# Class AppMissingGridHandler



[**ClassList**](annotated.md) **>** [**AppMissingGridHandler**](classAppMissingGridHandler.md)



[More...](#detailed-description)

* `#include <appcoordinateoperationhandlers.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**coordinateOperationCreationError**](classAppMissingGridHandler.md#signal-coordinateoperationcreationerror) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QString & error) <br> |
| signal void | [**fallbackOperationOccurred**](classAppMissingGridHandler.md#signal-fallbackoperationoccurred) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QString & desired) <br> |
| signal void | [**missingGridUsedByContextHandler**](classAppMissingGridHandler.md#signal-missinggridusedbycontexthandler) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QgsDatumTransform::TransformDetails & desired) <br> |
| signal void | [**missingPreferredGrid**](classAppMissingGridHandler.md#signal-missingpreferredgrid) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QgsDatumTransform::TransformDetails & preferredOperation, const QgsDatumTransform::TransformDetails & availableOperation) <br> |
| signal void | [**missingRequiredGrid**](classAppMissingGridHandler.md#signal-missingrequiredgrid) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QgsDatumTransform::GridDetails & grid) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AppMissingGridHandler**](#function-appmissinggridhandler) (QObject \* parent) <br> |




























## Detailed Description


Alerts users when a transform grids are required (or desired) for an operation between two CRSes, yet they are not available on the current system. Derived from the QgsAppCoordinateOperationHandlers class created by QGIS developer Nyall Dawson. 


    
## Public Signals Documentation




### signal coordinateOperationCreationError 

```C++
void AppMissingGridHandler::coordinateOperationCreationError;
```




<hr>



### signal fallbackOperationOccurred 

```C++
void AppMissingGridHandler::fallbackOperationOccurred;
```




<hr>



### signal missingGridUsedByContextHandler 

```C++
void AppMissingGridHandler::missingGridUsedByContextHandler;
```




<hr>



### signal missingPreferredGrid 

```C++
void AppMissingGridHandler::missingPreferredGrid;
```




<hr>



### signal missingRequiredGrid 

```C++
void AppMissingGridHandler::missingRequiredGrid;
```




<hr>
## Public Functions Documentation




### function AppMissingGridHandler 

```C++
explicit AppMissingGridHandler::AppMissingGridHandler (
    QObject * parent
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/appcoordinateoperationhandlers.h`

