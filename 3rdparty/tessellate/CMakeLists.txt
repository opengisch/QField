
project(libtess)

set(TESS_SRC
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

include_directories(include)

set (CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_library(tess STATIC ${TESS_SRC})
if(NOT MSVC)
    target_link_libraries(tess m)
endif()


set_property(TARGET tess PROPERTY AUTOMOC OFF)
set_property(TARGET tess PROPERTY AUTOUIC OFF)
set_property(TARGET tess PROPERTY AUTOGEN OFF)
set_property(TARGET tess PROPERTY AUTORCC OFF)


# add_executable(tess_example main.c)
# target_link_libraries(tess_example tess)
