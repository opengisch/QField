

# Class QfAppMissingGridHandler



[**ClassList**](annotated.md) **>** [**QfAppMissingGridHandler**](classQfAppMissingGridHandler.md)



[More...](#detailed-description)

* `#include <qfappcoordinateoperationhandlers.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**coordinateOperationCreationError**](classQfAppMissingGridHandler.md#signal-coordinateoperationcreationerror) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QString & error) <br> |
| signal void | [**fallbackOperationOccurred**](classQfAppMissingGridHandler.md#signal-fallbackoperationoccurred) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QString & desired) <br> |
| signal void | [**missingGridUsedByContextHandler**](classQfAppMissingGridHandler.md#signal-missinggridusedbycontexthandler) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QgsDatumTransform::TransformDetails & desired) <br> |
| signal void | [**missingPreferredGrid**](classQfAppMissingGridHandler.md#signal-missingpreferredgrid) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QgsDatumTransform::TransformDetails & preferredOperation, const QgsDatumTransform::TransformDetails & availableOperation) <br> |
| signal void | [**missingRequiredGrid**](classQfAppMissingGridHandler.md#signal-missingrequiredgrid) (const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs, const QgsDatumTransform::GridDetails & grid) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAppMissingGridHandler**](#function-qfappmissinggridhandler) (QObject \* parent) <br> |




























## Detailed Description


Alerts users when a transform grids are required (or desired) for an operation between two CRSes, yet they are not available on the current system. Derived from the QgsAppCoordinateOperationHandlers class created by QGIS developer Nyall Dawson. 


    
## Public Signals Documentation




### signal coordinateOperationCreationError 

```C++
void QfAppMissingGridHandler::coordinateOperationCreationError;
```




<hr>



### signal fallbackOperationOccurred 

```C++
void QfAppMissingGridHandler::fallbackOperationOccurred;
```




<hr>



### signal missingGridUsedByContextHandler 

```C++
void QfAppMissingGridHandler::missingGridUsedByContextHandler;
```




<hr>



### signal missingPreferredGrid 

```C++
void QfAppMissingGridHandler::missingPreferredGrid;
```




<hr>



### signal missingRequiredGrid 

```C++
void QfAppMissingGridHandler::missingRequiredGrid;
```




<hr>
## Public Functions Documentation




### function QfAppMissingGridHandler 

```C++
explicit QfAppMissingGridHandler::QfAppMissingGridHandler (
    QObject * parent
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qfappcoordinateoperationhandlers.h`

