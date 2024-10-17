/***************************************************************************
  projectsimageprovider.h - ProjectsImageProvider

 ---------------------
 begin                : 15.07.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PROJECTSIMAGEPROVIDER_H
#define PROJECTSIMAGEPROVIDER_H

#include <QQuickImageProvider>

/**
 * \ingroup core
 */
class ProjectsImageProvider : public QQuickImageProvider
{
  public:
    explicit ProjectsImageProvider();

    QQmlImageProviderBase::ImageType imageType() const override { return QQmlImageProviderBase::Image; }
    QImage requestImage( const QString &id, QSize *size, const QSize &requestedSize ) override;
};

#endif // PROJECTSIMAGEPROVIDER_H
