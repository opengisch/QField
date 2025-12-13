/***************************************************************************
 translatormanager.cpp - TranslatorManager

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
#include "translatormanager.h"

#include <QTranslator>

TranslatorManager::TranslatorManager()
  : mQFieldTranslator( std::make_unique<QTranslator>() )
  , mQtTranslator( std::make_unique<QTranslator>() )
{
}

QTranslator *TranslatorManager::qfieldTranslator()
{
  return mQFieldTranslator.get();
}

QTranslator *TranslatorManager::qtTranslator()
{
  return mQtTranslator.get();
}

TranslatorManager *TranslatorManager::instance()
{
  static TranslatorManager *sInstance = new TranslatorManager();
  return sInstance;
}
