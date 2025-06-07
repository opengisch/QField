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

  public:
    explicit FeatureCheckListProxyModel( QObject *parent = nullptr );

    Q_INVOKABLE void sortCheckedFirst( const bool enabled );
};

#endif // FEATURECHECKLISTPROXYMODEL_H
