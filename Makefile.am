EXTRA_DIST = adonthell.pc.in adonthell.pc.cmake autogen.sh README.Win32 \
    CMakeLists.txt config/FindLibxml2.cmake config/FindSWIG.cmake \
    config/UsePkgConfig.cmake config/PythonExtraLibs.cmake config.h.cmake \
    scripts/make-bundle.sh README.MacOSX

SUBDIRS = libltdl src doc test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = adonthell.pc

$(pkgconfig_DATA): config.status

