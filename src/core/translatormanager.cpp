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
  : mQfieldTranslator( new QTranslator() )
  , mQtTranslator( new QTranslator() )
{
}

QTranslator *TranslatorManager::qfieldTranslator()
{
  return mQfieldTranslator;
}

QTranslator *TranslatorManager::qtTranslator()
{
  return mQtTranslator;
}

TranslatorManager *TranslatorManager::instance()
{
  static TranslatorManager *sInstance = new TranslatorManager();
  return sInstance;
}
