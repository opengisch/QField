
PROJECT(libtess)

SET(TESS_SRC
  dict.c  
  geom.c  
  # main.c  
  memalloc.c  
  mesh.c  
  normal.c  
  priorityq.c  
  # priorityq-heap.c  
  render.c  
  sweep.c  
  tess.c  
  tessellate.c  
  tessmono.c
)

INCLUDE_DIRECTORIES(include)

SET (CMAKE_POSITION_INDEPENDENT_CODE TRUE)

ADD_LIBRARY(tess STATIC ${TESS_SRC})
IF(NOT MSVC)
    TARGET_LINK_LIBRARIES(tess m)
ENDIF(NOT MSVC)


SET_PROPERTY(TARGET tess PROPERTY AUTOMOC OFF)
SET_PROPERTY(TARGET tess PROPERTY AUTOUIC OFF)
SET_PROPERTY(TARGET tess PROPERTY AUTOGEN OFF)
SET_PROPERTY(TARGET tess PROPERTY AUTORCC OFF)


# ADD_EXECUTABLE(tess_example main.c)
# TARGET_LINK_LIBRARIES(tess_example tess)
