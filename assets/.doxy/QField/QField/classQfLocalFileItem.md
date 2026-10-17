

# Class QfLocalFileItem



[**ClassList**](annotated.md) **>** [**QfLocalFileItem**](classQfLocalFileItem.md)


































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**checked**](classQfLocalFileItem.md#property-checked-12)  <br> |
| property QString | [**format**](classQfLocalFileItem.md#property-format-12)  <br> |
| property QfLocalFilesModel::ItemMetaType | [**metaType**](classQfLocalFileItem.md#property-metatype-12)  <br> |
| property QString | [**path**](classQfLocalFileItem.md#property-path-12)  <br> |
| property qint64 | [**size**](classQfLocalFileItem.md#property-size-12)  <br> |
| property QString | [**title**](classQfLocalFileItem.md#property-title-12)  <br> |
| property QfLocalFilesModel::ItemType | [**type**](classQfLocalFileItem.md#property-type-12)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLocalFileItem**](#function-qflocalfileitem-12) () = default<br> |
|   | [**QfLocalFileItem**](#function-qflocalfileitem-22) (QfLocalFilesModel::ItemMetaType metaType, QfLocalFilesModel::ItemType type, const QString & title, const QString & format, const QString & path, qint64 size=0, bool checked=false) <br> |
|  bool | [**checked**](#function-checked-22) () const<br> |
|  QString | [**format**](#function-format-22) () const<br> |
|  QfLocalFilesModel::ItemMetaType | [**metaType**](#function-metatype-22) () const<br> |
|  QString | [**path**](#function-path-22) () const<br> |
|  void | [**setChecked**](#function-setchecked) (bool checked) <br> |
|  qint64 | [**size**](#function-size-22) () const<br> |
|  QString | [**title**](#function-title-22) () const<br> |
|  QfLocalFilesModel::ItemType | [**type**](#function-type-22) () const<br> |




























## Public Properties Documentation




### property checked [1/2]

```C++
bool QfLocalFileItem::checked;
```




<hr>



### property format [1/2]

```C++
QString QfLocalFileItem::format;
```




<hr>



### property metaType [1/2]

```C++
QfLocalFilesModel::ItemMetaType QfLocalFileItem::metaType;
```




<hr>



### property path [1/2]

```C++
QString QfLocalFileItem::path;
```




<hr>



### property size [1/2]

```C++
qint64 QfLocalFileItem::size;
```




<hr>



### property title [1/2]

```C++
QString QfLocalFileItem::title;
```




<hr>



### property type [1/2]

```C++
QfLocalFilesModel::ItemType QfLocalFileItem::type;
```




<hr>
## Public Functions Documentation




### function QfLocalFileItem [1/2]

```C++
QfLocalFileItem::QfLocalFileItem () = default
```




<hr>



### function QfLocalFileItem [2/2]

```C++
inline QfLocalFileItem::QfLocalFileItem (
    QfLocalFilesModel::ItemMetaType metaType,
    QfLocalFilesModel::ItemType type,
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
inline bool QfLocalFileItem::checked () const
```




<hr>



### function format [2/2]

```C++
inline QString QfLocalFileItem::format () const
```




<hr>



### function metaType [2/2]

```C++
inline QfLocalFilesModel::ItemMetaType QfLocalFileItem::metaType () const
```




<hr>



### function path [2/2]

```C++
inline QString QfLocalFileItem::path () const
```




<hr>



### function setChecked 

```C++
inline void QfLocalFileItem::setChecked (
    bool checked
) 
```




<hr>



### function size [2/2]

```C++
inline qint64 QfLocalFileItem::size () const
```




<hr>



### function title [2/2]

```C++
inline QString QfLocalFileItem::title () const
```




<hr>



### function type [2/2]

```C++
inline QfLocalFilesModel::ItemType QfLocalFileItem::type () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qflocalfilesmodel.h`

