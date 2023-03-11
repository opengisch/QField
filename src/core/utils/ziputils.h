/***************************************************************************
                        ziputils.h
                        ---------------
  begin                : March 2023
  copyright            : (C) 2023 by Mathieu Pellerin
  email                : mathieu@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   Original QgsZipUtils code by Paul Blottiere                           *
 *                                                                         *
 ***************************************************************************/

#ifndef ZIPUTILS_H
#define ZIPUTILS_H

#include "qfield_core_export.h"

#include <QStringList>

namespace ZipUtils
{

  /**
   * Unzip a zip file in an output directory.
   * \param zip The zip filename
   * \param dir The output directory
   * \param files The absolute path of unzipped files
   * \param consistencyCheck Perform additional stricter consistency checks on the archive, and error if they fail
   * \returns FALSE if the zip filename does not exist, the output directory
   * does not exist or is not writable.
   */
  QFIELD_CORE_EXPORT bool unzip( const QString &zipFilename, const QString &dir, QStringList &files, bool checkConsistency = true );

  /**
   * Returns the list of files within a \a zip file
   */
  QFIELD_CORE_EXPORT const QStringList files( const QString &zip );
}; // namespace ZipUtils

#endif //ZIPUTILS_H
