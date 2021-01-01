TEMPLATE = lib

TARGET = tessellate

CONFIG = staticlib

QMAKE_CFLAGS += -fPIC

SOURCES = dict.c \
  geom.c \
  memalloc.c \
  mesh.c \
  normal.c \
  priorityq.c \
  render.c \
  sweep.c \
  tess.c \
  tessellate.c \
  tessmono.c
