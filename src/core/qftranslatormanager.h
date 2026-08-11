/***************************************************************************
 qftranslatormanager.h - QfTranslatorManager

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

#ifndef QFTRANSLATORMANAGER_H
#define QFTRANSLATORMANAGER_H

#include <memory>

class QTranslator;

/**
 * \brief Provides access to shared translator instances for runtime language switching.
 *
 * Translators are shared between startup (main.cpp) and runtime switching (QfAppInterface::changeLanguage).
 * \ingroup core
 */
class QfTranslatorManager
{
  public:
    //!  Constructor
    QfTranslatorManager();

    //! Copy constructor is deleted (singleton pattern)
    QfTranslatorManager( const QfTranslatorManager & ) = delete;

    //! Assignment operator is deleted (singleton pattern)
    QfTranslatorManager &operator=( const QfTranslatorManager & ) = delete;

    //! Returns the global QField translator instance.
    QTranslator *qfieldTranslator();

    //! Returns the global Qt framework translator instance.
    QTranslator *qtTranslator();

    //! Returns the singleton instance of QfTranslatorManager.
    static QfTranslatorManager *instance();

  private:
    std::unique_ptr<QTranslator> mQFieldTranslator;
    std::unique_ptr<QTranslator> mQtTranslator;
};

#endif // QFTRANSLATORMANAGER_H
