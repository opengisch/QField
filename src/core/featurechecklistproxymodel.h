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
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FEATURECHECKLISTPROXYMODEL_H
#define FEATURECHECKLISTPROXYMODEL_H

#include <QSortFilterProxyModel>

class FeatureCheckListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY( QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged )
    Q_PROPERTY( bool sortCheckedFirst READ sortCheckedFirst WRITE setSortCheckedFirst NOTIFY sortCheckedFirstChanged )

  public:
    explicit FeatureCheckListProxyModel( QObject *parent = nullptr );

    /**
     * Returns the current search term used to filter items.
     */
    QString searchTerm() const;

    /**
     * Sets the search term for filtering and updates the filter.
     */
    void setSearchTerm( const QString &searchTerm );

    /**
     * Returns whether checked items are sorted to the top.
     */
    bool sortCheckedFirst() const;

    /**
     * Enables or disables sorting checked items first.
     */
    void setSortCheckedFirst( bool enabled );

  protected:
    /**
     * Determines whether a row should be accepted based on the current filter settings.
     * Checks if the data in the row matches the current search term.
     */
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

    /**
     * Compares two items for sorting.
     * If mSortCheckedFirst checked, checked items are ordered before unchecked.
     * When search term filtering is active, items starting with the search term are prioritized.
     * Otherwise, items are sorted alphabetically.
     */
    bool lessThan( const QModelIndex &left, const QModelIndex &right ) const override;


  signals:
    void searchTermChanged();
    void sortCheckedFirstChanged();

  private:
    QString mSearchTerm;
    bool mSortCheckedFirst;
};
#endif // FEATURECHECKLISTPROXYMODEL_H
