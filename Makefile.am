EXTRA_DIST = adonthell.pc.in autogen.sh README.Win32 \
    CMakeLists.txt config/FindLibxml2.cmake config/FindSWIG.cmake \
    config/UsePkgConfig.cmake config.h.cmake

SUBDIRS = libltdl src doc test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = adonthell.pc

$(pkgconfig_DATA): config.status

