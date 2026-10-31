/***************************************************************************
 qfvideobarcodemodel.h - QfVideoBarcodeModel

 ---------------------
 begin                : October 2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFVIDEOBARCODEMODEL_H
#define QFVIDEOBARCODEMODEL_H

#include <QAbstractListModel>
#include <QRectF>

/**
 * \ingroup core
 *
 * A list model of the barcodes seen in a video feed.
 */
class QfVideoBarcodeModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum Roles
    {
      BarcodeText = Qt::UserRole + 1,
      BarcodeBounds,
    };

    struct Barcode
    {
        Barcode() = default;

        Barcode( const QString &text, const QRectF &bounds )
          : text( text )
          , bounds( bounds )
        {}

        QString text;
        QRectF bounds;
    };

    explicit QfVideoBarcodeModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    /**
     * Updates the model with the \a barcodes found in a video frame.
     */
    void updateBarcodes( const QList<Barcode> &barcodes );

    /**
     * Removes all barcodes from the model.
     */
    Q_INVOKABLE void clear();

  private:
    int rowForText( const QString &text ) const;

    struct TrackedBarcode
    {
        TrackedBarcode() = default;

        explicit TrackedBarcode( const Barcode &barcode )
          : barcode( barcode )
        {}

        Barcode barcode;
        int missedFrameCount = 0;
    };

    static constexpr int sMaximumMissedFrameCount = 4;

    QList<TrackedBarcode> mVisibleBarcodes;
};

#endif // QFVIDEOBARCODEMODEL_H
