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

QTranslator *TranslatorManager::qfieldTranslator()
{
  static QTranslator *translator = new QTranslator();
  return translator;
}

QTranslator *TranslatorManager::qtTranslator()
{
  static QTranslator *translator = new QTranslator();
  return translator;
}
