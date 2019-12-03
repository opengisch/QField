###########################################################################
#    mxe.Dockerfile
#    ---------------------
#    Date                 : November 2019
#    Copyright            : (C) 2019 by Alessandro Pasotti
#    Email                : elpaso at itopen dot it
###########################################################################
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#                                                                         #
###########################################################################
#
# Recipe for a QField building environment with all dependencies
# built and installed
#
# Base image recipe and docs are available from
# https://github.com/qgis/QGIS/tree/master/ms-windows/mxe
# This image adds a recipe for QGIS builds (qgis.mk) to the base image
# and builds a minimal QGIS that is a required dependency for QField.
#
# Image should be uploaded to docker hub as as elpaso/mxe-qfield-builder:XXXXXXX
# and tagged as :latest.
# XXXXXXX is typically the QGIS commit of the build as specified in
# qgis.mk.
#
# To build and tag the image from the current directory:
#
# docker build -t elpaso/mxe-qfield-builder:XXXXXXX -f mxe.Dockerfile .
# docker tag elpaso/mxe-qfield-builder:XXXXXXXX elpaso/mxe-qfield-builder:latest
#

FROM elpaso/mxe-qt5-builder:latest

# Number of cores for the build
ENV NCORES 16

# Additional dependencies for QField
RUN make MXE_TARGETS=x86_64-w64-mingw32.shared.posix -j ${NCORES} \
    qtquickcontrols2 \
    qtlocation \
    qtgraphicaleffects

# Copy QGIS MXE recipe into the image
COPY qgis.mk src/qgis.mk

# Build 64bit QGIS
RUN make MXE_TARGETS=x86_64-w64-mingw32.shared.posix -j ${NCORES} qgis

# Clean
RUN rm -rf ./log && rm -rf ./pkg
