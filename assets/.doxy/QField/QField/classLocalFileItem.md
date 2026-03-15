

# Class LocalFileItem



[**ClassList**](annotated.md) **>** [**LocalFileItem**](classLocalFileItem.md)


































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**checked**](classLocalFileItem.md#property-checked-12)  <br> |
| property QString | [**format**](classLocalFileItem.md#property-format-12)  <br> |
| property LocalFilesModel::ItemMetaType | [**metaType**](classLocalFileItem.md#property-metatype-12)  <br> |
| property QString | [**path**](classLocalFileItem.md#property-path-12)  <br> |
| property qint64 | [**size**](classLocalFileItem.md#property-size-12)  <br> |
| property QString | [**title**](classLocalFileItem.md#property-title-12)  <br> |
| property LocalFilesModel::ItemType | [**type**](classLocalFileItem.md#property-type-12)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LocalFileItem**](#function-localfileitem-12) () = default<br> |
|   | [**LocalFileItem**](#function-localfileitem-22) (LocalFilesModel::ItemMetaType metaType, LocalFilesModel::ItemType type, const QString & title, const QString & format, const QString & path, qint64 size=0, bool checked=false) <br> |
|  bool | [**checked**](#function-checked-22) () const<br> |
|  QString | [**format**](#function-format-22) () const<br> |
|  LocalFilesModel::ItemMetaType | [**metaType**](#function-metatype-22) () const<br> |
|  QString | [**path**](#function-path-22) () const<br> |
|  void | [**setChecked**](#function-setchecked) (bool checked) <br> |
|  qint64 | [**size**](#function-size-22) () const<br> |
|  QString | [**title**](#function-title-22) () const<br> |
|  LocalFilesModel::ItemType | [**type**](#function-type-22) () const<br> |




























## Public Properties Documentation




### property checked [1/2]

```C++
bool LocalFileItem::checked;
```




<hr>



### property format [1/2]

```C++
QString LocalFileItem::format;
```




<hr>



### property metaType [1/2]

```C++
LocalFilesModel::ItemMetaType LocalFileItem::metaType;
```




<hr>



### property path [1/2]

```C++
QString LocalFileItem::path;
```




<hr>



### property size [1/2]

```C++
qint64 LocalFileItem::size;
```




<hr>



### property title [1/2]

```C++
QString LocalFileItem::title;
```




<hr>



### property type [1/2]

```C++
LocalFilesModel::ItemType LocalFileItem::type;
```




<hr>
## Public Functions Documentation




### function LocalFileItem [1/2]

```C++
LocalFileItem::LocalFileItem () = default
```




<hr>



### function LocalFileItem [2/2]

```C++
inline LocalFileItem::LocalFileItem (
    LocalFilesModel::ItemMetaType metaType,
    LocalFilesModel::ItemType type,
    const QString & title,
    const QString & format,
    const QString & path,
    qint64 size=0,
    bool checked=false
) 
```




<hr>



### function checked [2/2]

```C++
inline bool LocalFileItem::checked () const
```




<hr>



### function format [2/2]

```C++
inline QString LocalFileItem::format () const
```




<hr>



### function metaType [2/2]

```C++
inline LocalFilesModel::ItemMetaType LocalFileItem::metaType () const
```




<hr>



### function path [2/2]

```C++
inline QString LocalFileItem::path () const
```




<hr>



### function setChecked 

```C++
inline void LocalFileItem::setChecked (
    bool checked
) 
```




<hr>



### function size [2/2]

```C++
inline qint64 LocalFileItem::size () const
```




<hr>



### function title [2/2]

```C++
inline QString LocalFileItem::title () const
```




<hr>



### function type [2/2]

```C++
inline LocalFilesModel::ItemType LocalFileItem::type () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/localfilesmodel.h`

