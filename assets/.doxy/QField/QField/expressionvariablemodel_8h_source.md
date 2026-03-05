

# File expressionvariablemodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**expressionvariablemodel.h**](expressionvariablemodel_8h.md)

[Go to the documentation of this file](expressionvariablemodel_8h.md)


```C++
/***************************************************************************
  expressionvariablemodel.h - ExpressionVariableModel

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
#ifndef EXPRESSIONVARIABLEMODEL_H
#define EXPRESSIONVARIABLEMODEL_H

#include <QStandardItemModel>
#include <qgsproject.h>

class ExpressionVariableModel : public QStandardItemModel
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

    explicit ExpressionVariableModel( QObject *parent = nullptr );

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

#endif // EXPRESSIONVARIABLEMODEL_H
```


