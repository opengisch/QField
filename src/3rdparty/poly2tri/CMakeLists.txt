SET(POLY2TRI_SRCS
  common/shapes.cpp
  sweep/sweep_context.cpp
  sweep/cdt.cpp
  sweep/sweep.cpp
  sweep/advancing_front.cpp
)

SET (CMAKE_POSITION_INDEPENDENT_CODE TRUE)

ADD_LIBRARY(poly2tri STATIC ${POLY2TRI_SRCS})
