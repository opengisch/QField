/***************************************************************************
                        test_sggeometry.cpp
                        --------------------
  begin                : 2022
  copyright            : (C) 2022 by Matthias Kuhn
  email                : matthias@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "catch2.h"
#include "qgssggeometry.h"

TEST_CASE( "SgGeometry" )
{
  // Check it doesn't crash on curves
  QgsGeometry geom = QgsGeometry::fromWkt( "CurvePolygon (CompoundCurve ((2527406.92703950637951493 1151573.31886140280403197, 2527406.92719999980181456 1151573.31890000007115304, 2527412.52009999984875321 1151573.73900000005960464, 2527413.38810000009834766 1151570.10510000004433095),CircularString (2527413.38810000009834766 1151570.10510000004433095, 2527413.600049186963588 1151569.38525660359300673, 2527413.85110000008717179 1151568.67809999990276992, 2527414.18413733039051294 1151567.78600968676619232, 2527414.57810000004246831 1151566.91910000005736947, 2527414.78706829901784658 1151566.54615262267179787, 2527415.01910000015050173 1151566.18709999998100102 )) )" );
  QgsSGGeometry sgGeom = QgsSGGeometry( geom, QColor(), 1, QgsRectangle(), 1 );
}
