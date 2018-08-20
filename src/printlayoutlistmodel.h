/***************************************************************************
  printlayoutlistmodel.h

 ---------------------
 begin                : 20.8.2018
 copyright            : (C) 2018 by david
 email                : david at opengisch dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PRINTLAYOUTLISTMODEL_H
#define PRINTLAYOUTLISTMODEL_H

#include <QAbstractListModel>

#include <qgsprintlayout.h>
#include <qgslayoutmanager.h>
#include <qgslayoutpagecollection.h>

class QStringList;
class QgsProject;

class PrintLayoutListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QgsProject* project READ project WRITE setProject )

    enum Roles
    {
      TitleRow,
      IndexRow
    };

    //! at the moment only contains the title string - probably in future more attributes needed
    struct PrintLayout
    {
      PrintLayout(const QString& title )
        : title( title )
      {}

      QString title;
    };

  public:
    PrintLayoutListModel( QObject* parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;

    QgsProject* project() const;
    void setProject( QgsProject* project );

  private:
    void reloadModel();

    QgsProject* mProject;
    QList<PrintLayout> mPrintLayouts;
};

#endif // PRINTLAYOUTLISTMODEL_H
