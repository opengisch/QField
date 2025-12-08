/***************************************************************************
 translatormanager.h - TranslatorManager

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

#ifndef TRANSLATORMANAGER_H
#define TRANSLATORMANAGER_H

class QTranslator;

/**
 * \brief Provides access to shared translator instances for runtime language switching.
 *
 * Translators are shared between startup (main.cpp) and runtime switching (AppInterface::changeLanguage).
 * \ingroup core
 */
class TranslatorManager
{
  public:
    //! Returns the global QField translator instance.
    QTranslator *qfieldTranslator();

    //! Returns the global Qt framework translator instance.
    QTranslator *qtTranslator();

    //! Returns the singleton instance of TranslatorManager.
    static TranslatorManager *instance();

  private:
    TranslatorManager();
    ~TranslatorManager() = default;

    QTranslator *mQfieldTranslator = nullptr;
    QTranslator *mQtTranslator = nullptr;
};

#endif // TRANSLATORMANAGER_H
