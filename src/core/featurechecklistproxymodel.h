/***************************************************************************
  featurechecklistproxymodel.h - FeatureCheckListProxyModel

 ---------------------
 begin                : Jun 2025
 copyright            : (C) 2025 by Mohsen Dehghanzadeh
 email                : mohsen (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.
 *                                                                         *
 ***************************************************************************/

#ifndef FEATURECHECKLISTPROXYMODEL_H
#define FEATURECHECKLISTPROXYMODEL_H

#include <QSortFilterProxyModel>

class FeatureCheckListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY( QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged )

  public:
    explicit FeatureCheckListProxyModel( QObject *parent = nullptr );

    QString searchTerm() const;
    void setSearchTerm( const QString &searchTerm );

    Q_INVOKABLE void sortCheckedFirst( const bool enabled );

  signals:
    void searchTermChanged();

  protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  private:
    QString mSearchTerm;
};

#endif // FEATURECHECKLISTPROXYMODEL_H
