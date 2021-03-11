/***************************************************************************
                        qfield.h
                        ---------------
  begin                : Jun 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFIELD_H
#define QFIELD_H


/**
 * Contains "polyfills" for backporting c++ features from standards > c++17 and Qt global methods
 * added later than our minimum version, as well nice some sugar.
 */
namespace qfield
{

  /**
   * Can be used to iterate over `QMap`s in a convenient way.
   * 
   * \code{.cpp}
   * for (auto [key, value]: asKeyValueRange(map))
   * {
   *   // do something with `key` and `value`
   * }
   * \endcode
   */
  template <typename T>
  class asKeyValueRange
  {
    public:
      explicit asKeyValueRange( T &data )
        : mData{data}
      {
      }

      auto begin() { return mData.keyValueBegin(); }

      auto end() { return mData.keyValueEnd(); }

    private:
      T &mData;
  };
};

#endif // QFIELD_H
