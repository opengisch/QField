# This file is **not yet** part of MXE. See LICENSE.md for licensing information.

# This recipe for MXE builds QGIS with a minimal set of components
# from a commit specified in $(PKG)_VERSION, if you update the commit,
# make sure to also update $(PKG)_CHECKSUM (openssl) digest


PKG             := qgis
$(PKG)_WEBSITE  := https://www.qgis.org
$(PKG)_DESCR    := QGIS
# QGIS commit:
$(PKG)_VERSION  := 7333adecaf12c34f34474d64a9b6e6446fdff179
$(PKG)_FILE     := $($(PKG)_VERSION).tar.gz
$(PKG)_SUBDIR   := QGIS-$($(PKG)_VERSION)
$(PKG)_URL      := https://github.com/QGIS/qgis/archive/$($(PKG)_VERSION).tar.gz
$(PKG)_CHECKSUM := 632b3cfdb976f1d79643e6f48ec02f6768bd97a189ef8894403bf018a71c9b15
$(PKG)_TARGETS  := $(BUILD) $(MXE_TARGETS)
$(PKG)_DEPS     := qca qtlocation qscintilla2 qwt gdal qtkeychain qtserialport qtwebkit qtwinextras libzip gsl libspatialindex exiv2


define $(PKG)_BUILD

    cd '$(BUILD_DIR)' && \
	$(TARGET)-cmake \
	 	-D CMAKE_BUILD_TYPE=Release \
        -D ENABLE_TESTS=OFF \
        -D WITH_QSPATIALITE=ON \
        -D WITH_APIDOC=OFF \
        -D WITH_QWTPOLAR=ON \
        -D WITH_ASTYLE=OFF \
        -D WITH_SERVER=OFF \
        -D WITH_3D=FALSE \
        -D WITH_GUI=FALSE \
        -D WITH_DESKTOP=FALSE \
        -D WITH_BINDINGS=FALSE \
        -D QT_LRELEASE_EXECUTABLE=$(PREFIX)/$(TARGET)/qt5/bin/lrelease \
        -D MXE=ON \
		'$(SOURCE_DIR)'

	$(MAKE) -C '$(BUILD_DIR)' -j '$(JOBS)'
	$(MAKE) -C '$(BUILD_DIR)' -j '$(JOBS)' install

endef
