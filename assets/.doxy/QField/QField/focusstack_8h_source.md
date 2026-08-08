

# File focusstack.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**focusstack.h**](focusstack_8h.md)

[Go to the documentation of this file](focusstack_8h.md)


```C++
/***************************************************************************
  focusstack.h

 ---------------------
 begin                : 11.4.2018
 copyright            : (C) 2018 by david
 email                : david at opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef FOCUSSTACK_H
#define FOCUSSTACK_H

#include <QList>
#include <QQuickItem>

class FocusStack : public QObject
{
    Q_OBJECT

  public:
    explicit FocusStack( QObject *parent = nullptr );
    ~FocusStack() = default;

    Q_INVOKABLE void addFocusTaker( QObject *object );
    Q_INVOKABLE void forceActiveFocusOnLastTaker();

  private slots:
    void visibleChanged();
    void popupOpened();
    void popupClosed();

  private:
    QList<QPointer<QObject>> mStackList;
    void setFocused( QObject *object );
    void setUnfocused( QObject *object );
};

#endif // FOCUSSTACK_H
```


