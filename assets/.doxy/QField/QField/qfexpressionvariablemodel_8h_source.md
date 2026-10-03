

# File qfexpressionvariablemodel.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfexpressionvariablemodel.h**](qfexpressionvariablemodel_8h.md)

[Go to the documentation of this file](qfexpressionvariablemodel_8h.md)


```C++
/***************************************************************************
  qfexpressionvariablemodel.h - QfExpressionVariableModel

 ---------------------
 begin                : 29.9.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFEXPRESSIONVARIABLEMODEL_H
#define QFEXPRESSIONVARIABLEMODEL_H

#include <QStandardItemModel>
#include <qgsproject.h>

class QfExpressionVariableModel : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *currentProject READ currentProject WRITE setCurrentProject NOTIFY currentProjectChanged )

  public:
    enum Roles
    {
      VariableEditableRole = Qt::EditRole,
      VariableNameRole = Qt::UserRole,
      VariableValueRole = Qt::UserRole + 1,
      VariableScopeRole = Qt::UserRole + 2,
      VariableOriginalNameRole = Qt::UserRole + 3,
    };

    enum class VariableScope
    {
      GlobalScope,
      ProjectScope
    };

    Q_ENUM( VariableScope )

    explicit QfExpressionVariableModel( QObject *parent = nullptr );

    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    Q_INVOKABLE int addVariable( VariableScope scope, const QString &name, const QString &value );

    Q_INVOKABLE void removeVariable( VariableScope scope, const QString &name );

    Q_INVOKABLE void save();

    Q_INVOKABLE void reloadVariables();

    QHash<int, QByteArray> roleNames() const override;

    QgsProject *currentProject() const;

    void setCurrentProject( QgsProject *project );

  signals:
    void currentProjectChanged();

  private slots:
    void onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles );

  private:
    void appendVariable( VariableScope scope, const QString &name, const QString &value, bool editable );

    QgsProject *mCurrentProject = nullptr;

    QList<QPair<VariableScope, QString>> mRemovedVariables;
};

#endif // QFEXPRESSIONVARIABLEMODEL_H
```


