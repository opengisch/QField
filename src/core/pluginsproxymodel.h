/***************************************************************************
 pluginsproxymodel.h - PluginsProxyModel

 ---------------------
 begin                : June 2025
 copyright            : (C) 2025 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PLUGINSPROXYMODEL_H
#define PLUGINSPROXYMODEL_H

#include <QSortFilterProxyModel>

/**
 * \ingroup core
 */
class PluginsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

  public:
    enum PluginFilter
    {
      InstalledPlugin,
      AvailablePlugins,
    };
    Q_ENUM( PluginFilter )

    explicit PluginsProxyModel( QObject *parent = nullptr );
};

Q_DECLARE_METATYPE( PluginsProxyModel::PluginFilter )

#endif // PLUGINSPROXYMODEL_H
