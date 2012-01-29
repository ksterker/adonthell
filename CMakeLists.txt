# Define the project name.
project(adonthell)

cmake_minimum_required(VERSION 2.4.3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/config")

# -- for CMake 2.6.x
if(COMMAND cmake_policy)
# -- pass fully qualified libraries directly to linker
    cmake_policy(SET CMP0003 NEW)
# -- use automatic escaping in add_definitions
    cmake_policy(SET CMP0005 NEW)
    set(ESCAPE "")
else(COMMAND cmake_policy)

# -- pre 2.6 needs escaping in add_definitions
if(CMAKE_GENERATOR MATCHES ".*Makefiles")
    set(ESCAPE "\\\"")
else(CMAKE_GENERATOR MATCHES ".*Makefiles")
    set(ESCAPE "\\\\\"")
endif(CMAKE_GENERATOR MATCHES ".*Makefiles")

endif(COMMAND cmake_policy)

# Set version
set (VERSION "0.4.0" CACHE INTERNAL "")

#Add Build types
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)
IF(NOT DEVBUILD)
  SET(DEVBUILD "OFF" CACHE STRING
      "Enable developer build, this builds unit tests."
      FORCE)
ENDIF(NOT DEVBUILD)

add_definitions(-DVERSION="${ESCAPE}${VERSION}${ESCAPE}" -DHAVE_CONFIG_H=1)

IF(CMAKE_BUILD_TYPE MATCHES "Release")
    add_definitions(-DGOOGLE_STRIP_LOG=1)
ENDIF(CMAKE_BUILD_TYPE MATCHES "Release")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions("-fno-strict-aliasing")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(APPLE)
   set(CMAKE_SHARED_LINKER_FLAGS "-single_module" CACHE INTERNAL "")
endif(APPLE)

##########################
# Manage option
##########################

if(NOT ADONTHELL_DATADIR)
       if(WIN32)
		set(ADONTHELL_DATADIR ".")
       else(WIN32)
              SET(ADONTHELL_DATADIR "${CMAKE_INSTALL_PREFIX}/share/games/adonthell")
       endif(WIN32)
endif(NOT ADONTHELL_DATADIR)
message(STATUS
	"Adonthell data directory: ${ADONTHELL_DATADIR}")
add_definitions(-DDATA_DIR="${ESCAPE}${ADONTHELL_DATADIR}${ESCAPE}")


# FIXME: We need to manage the package lib directory for different plattforms!
add_definitions(-DPKGLIBDIR="${ESCAPE}${CMAKE_INSTALL_PREFIX}/lib/adonthell${ESCAPE}")

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
	SET(LIB_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")


##########################
# Endianness
##########################

include(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)
if (WORDS_BIGENDIAN)
       ADD_DEFINITIONS("-D__BIG_ENDIAN__")
endif (WORDS_BIGENDIAN)

include(config/UsePkgConfig.cmake)


##########################
# SDL
##########################

PKGCONFIG_FOUND(sdl2 SDL_FOUND)
IF(SDL_FOUND)
	PKGCONFIG(sdl2 SDL_INCLUDE_DIR SDL_LIBADD SDL_LIBRARY SDL_CFLAGS)
	EXECUTE_PROCESS(COMMAND ${PKGCONFIG_EXECUTABLE} sdl2 --modversion OUTPUT_VARIABLE SDL_VERSION)
	SET(SDL_INCLUDE_DIR ${SDL_INCLUDE_DIR}/SDL2)
ELSE(SDL_FOUND)
    PKGCONFIG_FOUND(sdl SDL_FOUND)
    IF(NOT SDL_FOUND)
    	find_package(SDL REQUIRED)
        EXECUTE_PROCESS(COMMAND sdl-config --version OUTPUT_VARIABLE SDL_VERSION)
    ENDIF(NOT SDL_FOUND)
ENDIF(SDL_FOUND)

IF(SDL_FOUND)
    # check sdl version
    IF(SDL_VERSION)
        STRING(SUBSTRING ${SDL_VERSION} 0 3 SDL_VERSION)
    ELSE(SDL_VERSION)
        SET(SDL_VERSION 1.2)
    ENDIF(SDL_VERSION)

	MESSAGE(STATUS
		"SDL ${SDL_VERSION} has been found:")
	MESSAGE(STATUS
		"\tCFLAGS : ${SDL_INCLUDE_DIR}")
	MESSAGE(STATUS
		"\tLDFLAGS: ${SDL_LIBRARY}")
ELSE(SDL_FOUND)
	MESSAGE(FATAL_ERROR
		"SDL is required")
ENDIF(SDL_FOUND)


##########################
# SDL_mixer
##########################

IF(SDL_VERSION STREQUAL "2.0")
    find_package(SDL2_mixer REQUIRED)
ELSE(SDL_VERSION STREQUAL "2.0")
    find_package(SDL_mixer REQUIRED)
ENDIF(SDL_VERSION STREQUAL "2.0")

if(SDLMIXER_FOUND)
	# take a lucky guess
	if(${SDLMIXER_INCLUDE_DIR} MATCHES "NOTFOUND")
		set(SDLMIXER_INCLUDE_DIR ${SDL_INCLUDE_DIR})
	endif(${SDLMIXER_INCLUDE_DIR} MATCHES "NOTFOUND")

	MESSAGE(STATUS
		"SDL Mixer has been found:")
	MESSAGE(STATUS
		"\tCFLAGS : ${SDLMIXER_INCLUDE_DIR}")
	MESSAGE(STATUS
		"\tLDFLAGS: ${SDLMIXER_LIBRARY}")
else(SDLMIXER_FOUND)
	MESSAGE(FATAL_ERROR
		"SDL Mixer is required")
endif(SDLMIXER_FOUND)


#######################
# Python
#######################

find_package(PythonLibs REQUIRED)
if(PYTHON_INCLUDE_PATH)
	message(STATUS
		"Python has been found:")
	message(STATUS
		"\tCFLAGS : ${PYTHON_INCLUDE_PATH}")
	message(STATUS
		"\tLDFLAGS: ${PYTHON_LIBRARIES}")
else(PYTHON_INCLUDE_PATH)
	message(FATAL_ERROR
		"A Python installation is required")
endif(PYTHON_INCLUDE_PATH)

find_package(PythonInterp)
if(PYTHONINTERP_FOUND)
	EXEC_PROGRAM(${PYTHON_EXECUTABLE}
		ARGS "-c \"import sys;vi = sys.version_info; print \\\"python%s.%s\\\" %(vi[0],vi[1])\""
		OUTPUT_VARIABLE PYTHON_VERSION_DIR
	)
	message(STATUS
		"\tVERSION: ${PYTHON_VERSION_DIR}")
else(PYTHONINTERP_FOUND)
	message(FATAL_ERROR
		"Impossible to find the Python Interpreter")
endif(PYTHONINTERP_FOUND)

include(config/PythonExtraLibs.cmake)
	message(STATUS
		"\tLDEXTRA: ${PYTHON_EXTRA_LIBRARIES}")
	message(STATUS
		"\tSITEPKG: ${PYTHON_SITE_PACKAGE_DIR}")

#######################
# libZ
#######################
FIND_PACKAGE(ZLIB)

#######################
# PNG
#######################

PKGCONFIG_FOUND(libpng PNG_FOUND)
IF(PNG_FOUND)
	PKGCONFIG(libpng PNG_INCLUDE_DIR PNG_LIBADD PNG_LIBRARY PNG_CFLAGS)
ELSE(PNG_FOUND)
	find_package(PNG REQUIRED)
ENDIF(PNG_FOUND)

if(PNG_FOUND)
	MESSAGE(STATUS
		"PNG has been found:")
	MESSAGE(STATUS
		"\tCFLAGS : ${PNG_INCLUDE_DIR}")
	MESSAGE(STATUS
		"\tLDFLAGS: ${PNG_LIBRARY}")
else(PNG_FOUND)
	MESSAGE(FATAL_ERROR
		"PNG is required")
endif(PNG_FOUND)


#######################
# SWIG
#######################

find_package(SWIG REQUIRED)
if(SWIG_FOUND)
	MESSAGE(STATUS
		"SWIG is present on the system.")
else(SWIG_FOUND)
	# try to see if we have the swig programm and assume for now that this is sufficient
	FIND_PROGRAM(SWIG_EXECUTABLE NAMES swig PATHS /usr/local/bin )
	if(SWIG_EXECUTABLE)
		MESSAGE(STATUS "SWIG appears to be present on the system.")
		set(SWIG_USE_FILE ${CMAKE_ROOT}/Modules/UseSWIG.cmake)
		set(SWIG_FOUND YES)
	else(SWIG_EXECUTABLE)
		MESSAGE(FATAL_ERROR "Impossible to locate SWIG.")
	endif(SWIG_EXECUTABLE)
endif(SWIG_FOUND)


#######################
# LibXML2
#######################

PKGCONFIG_FOUND(libxml-2.0 LIBXML2_FOUND)
IF(LIBXML2_FOUND)
	PKGCONFIG(libxml-2.0 LIBXML2_INCLUDE_PATH LIBXML2_LIBADD LIBXML2_LIBRARIES LIBXML2_CFLAGS)
	IF (NOT LIBXML2_INCLUDE_PATH MATCHES ".*/libxml2$")
		SET(LIBXML2_INCLUDE_PATH "${LIBXML2_INCLUDE_PATH}/libxml2")
	ENDIF (NOT LIBXML2_INCLUDE_PATH MATCHES ".*/libxml2$")
ELSE(LIBXML2_FOUND)
	find_package(Libxml2)
ENDIF(LIBXML2_FOUND)

if(LIBXML2_FOUND)
	message(STATUS
		"LibXml2 has been found:")
	message(STATUS
		"\tCFLAGS : ${LIBXML2_INCLUDE_PATH}")
	message(STATUS
		"\tLDFLAGS: ${LIBXML2_LIBRARIES}")
else(LIBXML2_FOUND)
	message(FATAL_ERROR
		"LibXml2 is required.")
endif(LIBXML2_FOUND)

#######################
# Freetype2
#######################

PKGCONFIG_FOUND(freetype2 LIBFREETYPE2_FOUND)
IF(LIBFREETYPE2_FOUND)
	PKGCONFIG(freetype2 LIBFT_INCLUDE_PATH LIBFT_LIBADD LIBFT_LIBRARIES LIBFT_CFLAGS)
ELSE(LIBFREETYPE2_FOUND)
	find_package(FindFreetype2)
ENDIF(LIBFREETYPE2_FOUND)

if(LIBFREETYPE2_FOUND)
	message(STATUS
		"LibFreetype2 has been found:")
	message(STATUS
		"\tCFLAGS : ${LIBFT_CFLAGS}")
	message(STATUS
		"\tLDFLAGS: ${LIBFT_LIBRARIES}")
else(LIBFREETYPE2_FOUND)
	message(FATAL_ERROR
		"LibFreetype2 is required.")
endif(LIBFREETYPE2_FOUND)

#######################
# google-log
#######################

PKGCONFIG_FOUND(libglog LIBGLOG_FOUND)
IF(LIBGLOG_FOUND)
  PKGCONFIG(libglog LIBGLOG_INCLUDE_PATH LIBGLOG_LIBADD LIBGLOG_LIBRARIES LIBGLOG_CFLAGS)
  PKGCONFIG_VERSION(libglog GLOG_VERSION)
	message(STATUS
		"libglog has been found:")
	message(STATUS
		"\tCFLAGS : ${LIBGLOG_CFLAGS}")
	message(STATUS
		"\tLDFLAGS: ${LIBGLOG_LIBRARIES}")
    include_directories(${LIBGLOG_INCLUDE_PATH})
    SET(HAVE_GLOG_H 1)
ELSE(LIBGLOG_FOUND)
    message(STATUS
        "libglog not found.  Logging disabled")
ENDIF(LIBGLOG_FOUND)

#############################################################################
# If this is a developer build, turn on testing and check for gtest and gmock
#############################################################################

if(DEVBUILD)
  ENABLE_TESTING()
  PKGCONFIG_FOUND(libgtest LIBGTEST_FOUND)
  IF(LIBGTEST_FOUND)
    PKGCONFIG(libgtest LIBGTEST_INCLUDE_PATH LIBGTEST_LIBADD LIBGTEST_LIBRARIES LIBGTEST_CFLAGS)
    PKGCONFIG_VERSION(libgtest GLOG_VERSION)
	message(STATUS
	  "libgtest has been found:")
	message(STATUS
	  "\tCFLAGS : ${LIBGTEST_CFLAGS}")
	message(STATUS
	  "\tLDFLAGS: ${LIBGTEST_LIBRARIES}")
  ELSE(LIBGTEST_FOUND)
    message(FATAL_ERROR
      "libgtest is required.")
  ENDIF(LIBGTEST_FOUND)

  PKGCONFIG_FOUND(libgmock LIBGMOCK_FOUND)
  IF(LIBGMOCK_FOUND)
    PKGCONFIG(libgmock LIBGMOCK_INCLUDE_PATH LIBGMOCK_LIBADD LIBGMOCK_LIBRARIES LIBGMOCK_CFLAGS)
    PKGCONFIG_VERSION(libgmock GLOG_VERSION)
	message(STATUS
	  "libgmock has been found:")
	message(STATUS
	  "\tCFLAGS : ${LIBGMOCK_CFLAGS}")
	message(STATUS
	  "\tLDFLAGS: ${LIBGMOCK_LIBRARIES}")
  ELSE(LIBGMOCK_FOUND)
    message(FATAL_ERROR
      "libgmock is required.")
  ENDIF(LIBGMOCK_FOUND)

  SET(TEST_LIBRARIES ${LIBGTEST_LIBRARIES} ${LIBGMOCK_LIBRARIES})
endif(DEVBUILD)

######################
# Build the config.h file !
######################

include (CheckIncludeFiles)
check_include_files (dlfcn.h HAVE_DLFCN_H)
check_include_files (inttypes.h HAVE_INTTYPES_H)
# FIXME: missing check on the z library.
check_include_files (memory.h HAVE_MEMORY_H)
check_include_files (malloc.h HAVE_MALLOC_H)
check_include_files (stdint.h HAVE_STDINT_H)
check_include_files (stdlib.h HAVE_STDLIB_H)
check_include_files (strings.h HAVE_STRINGS_H)
check_include_files (sys/stat.h HAVE_SYS_STAT_H)
check_include_files (sys/types.h HAVE_SYS_TYPES_H)
check_include_files (unistd.h HAVE_UNISTD_H)
check_include_files ("stdlib.h;stdarg.h;string.h" STDC_HEADERS)

include (CheckFunctionExists)
check_function_exists(nanosleep HAVE_NANOSLEEP)

include (CheckLibraryExists)
check_library_exists(ltdl lt_dlerror "" HAVE_LIBLTDL)

if(NOT HAVE_LIBLTDL)
       message(FATAL_ERROR
		"Libltdl is required.")
endif(NOT HAVE_LIBLTDL)

# Now generate the configure file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Generate pkgconfig file
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/adonthell.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/adonthell.pc @ONLY)

# install pkgconfig file
install (
    FILES ${CMAKE_CURRENT_BINARY_DIR}/adonthell.pc
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig
    COMPONENT "pkgconfig"
)


# Add the root of the binary distribution to the include directories !
# Do it in order to fetch config.h, however, we may generate config.h in the
# src directory.
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(test)


################
# Source Package
################

set(CPACK_GENERATOR "")
set(CPACK_SOURCE_GENERATOR "TGZ")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Adonthell ${VERSION}")
set(CPACK_PACKAGE_CONTACT adonthell-general@nongun.org)
set(CPACK_PACKAGE_VENDOR com.linuxgames.adonthell)
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README)
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 4)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_VERSION "${VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/COPYING)

set(CPACK_SOURCE_PACKAGE_FILE_NAME "adonthell-${VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "adonthell-${VERSION}")
set(CPACK_SOURCE_IGNORE_FILES
    "~$"
    "\\\\.cvsignore"
    "CVS/"
)

include(CPack)
