

# File snappingresult.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**snappingresult.h**](snappingresult_8h.md)

[Go to the documentation of this file](snappingresult_8h.md)


```C++
/***************************************************************************
  snappingresult.h - SnappingResult

 ---------------------
 begin                : 8.10.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SNAPPINGRESULT_H
#define SNAPPINGRESULT_H

#include <qgspoint.h>
#include <qgspointlocator.h>
#include <qgsvectorlayer.h>

class SnappingResult
{
    Q_GADGET
    Q_PROPERTY( QgsPoint point READ point WRITE setPoint )
    Q_PROPERTY( bool isValid READ isValid )

  public:
    enum Type
    {
      Invalid = 0,               
      Vertex = 1,                
      Edge = 2,                  
      Area = 4,                  
      All = Vertex | Edge | Area 
    };

    Q_DECLARE_FLAGS( Types, Type )

    
    SnappingResult();

    SnappingResult( Type t, QgsVectorLayer *vl, QgsFeatureId fid, double dist, const QgsPoint &pt, int vertexIndex = 0, const QgsPoint *edgePoints = nullptr );

    bool operator==( const SnappingResult &other ) const;
    bool operator!=( const SnappingResult &other ) const { return !operator==( other ); }

    explicit SnappingResult( const QgsPointLocator::Match &match );

    Type type() const;

    bool isValid() const;
    bool hasVertex() const;
    bool hasEdge() const;
    bool hasArea() const;

    double distance() const;

    QgsPoint point() const;

    void setPoint( const QgsPoint point );

    int vertexIndex() const;

    QgsVectorLayer *layer() const;

    QgsFeatureId featureId() const;

    void edgePoints( QgsPoint &pt1, QgsPoint &pt2 ) const;

  private:
    Type mType;
    double mDist;
    QgsPoint mPoint;
    QPointer<QgsVectorLayer> mLayer;
    QgsFeatureId mFid;
    int mVertexIndex; // e.g. vertex index
    QgsPoint mEdgePoints[2];
};

#endif // SNAPPINGRESULT_H
```


