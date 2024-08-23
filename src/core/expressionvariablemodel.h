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
      VariableName = Qt::UserRole,
      VariableValue,
      VariableScopeRole,
      VariableEditable = Qt::EditRole
    };

    enum class VariableScope
    {
      ApplicationScope,
      ProjectScope
    };

    explicit ExpressionVariableModel( QObject *parent = nullptr );

    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    Q_INVOKABLE void addCustomVariable( const QString &varName, const QString &varVal, const int &rowIndex = -1 );

    Q_INVOKABLE void removeCustomVariable( int row );

    Q_INVOKABLE void save();

    Q_INVOKABLE void reloadVariables();

    Q_INVOKABLE void setName( int row, const QString &name );

    Q_INVOKABLE void setValue( int row, const QString &value );

    QHash<int, QByteArray> roleNames() const override;

    /**
     * Returns the current project used to retrieve variables from.
     */
    QgsProject *currentProject() const;

    /**
     * Sets the project used to retrieve variables from.
     */
    void setCurrentProject( QgsProject *project );

  signals:
    void currentProjectChanged();

  private slots:
    void onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles );

  private:
    QgsProject *mCurrentProject = nullptr;
};

#endif // EXPRESSIONVARIABLEMODEL_H
