

# File qfmarkupitem.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfmarkupitem.h**](qfmarkupitem_8h.md)

[Go to the documentation of this file](qfmarkupitem_8h.md)


```C++
/***************************************************************************
 qfmarkupitem.h - QfMarkupItem

 ---------------------
 begin                : 12.09.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFMARKUPITEM_H
#define QFMARKUPITEM_H

#include <QColor>
#include <QObject>
#include <qgsgeometry.h>

class QfMarkupItem
{
    Q_GADGET

    Q_PROPERTY( QString uuid READ uuid )
    Q_PROPERTY( QString label READ label )
    Q_PROPERTY( QString description READ description )
    Q_PROPERTY( QgsGeometry geometry READ geometry )
    Q_PROPERTY( QColor color READ color )

  public:
    QfMarkupItem() = default;
    explicit QfMarkupItem( const QString &label, const QString &description, const QgsGeometry &geometry, const QColor &color );

    QString uuid() const { return mUuid; }

    QString label() const { return mLabel; }

    QString description() const { return mDescription; }

    QgsGeometry geometry() const { return mGeometry; }

    QColor color() const { return mColor; }

    QfMarkupItem( const QfMarkupItem &other );

  private:
    QString mUuid;
    QString mLabel;
    QString mDescription;
    QgsGeometry mGeometry;
    QColor mColor;

    friend class QfMarkupCollection;
};

#endif // QFMARKUPITEM_H
```


