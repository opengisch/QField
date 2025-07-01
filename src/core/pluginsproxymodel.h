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
    Q_PROPERTY( QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged )
    Q_PROPERTY( PluginFilter filter READ filter WRITE setFilter NOTIFY filterChanged )

  public:
    enum PluginFilter
    {
      LocalPlugins,
      PublicPlugins,
    };
    Q_ENUM( PluginFilter )

    explicit PluginsProxyModel( QObject *parent = nullptr );

    /**
     * Returns the current search term used to filter items.
     */
    QString searchTerm() const;

    /**
     * Sets the search term for filtering and updates the filter.
     */
    void setSearchTerm( const QString &searchTerm );

    /**
     * Returns the current plugin filter.
     */
    PluginFilter filter() const;

    /**
     * Sets the plugin \a filter.
     */
    void setFilter( PluginFilter filter );

  protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  signals:
    void searchTermChanged();
    void filterChanged();

  private:
    QString mSearchTerm;
    PluginFilter mFilter = LocalPlugins;
};

Q_DECLARE_METATYPE( PluginsProxyModel::PluginFilter )

#endif // PLUGINSPROXYMODEL_H
