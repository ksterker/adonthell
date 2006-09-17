#
# this module look for libxml (http://www.xmlsoft.org) support
# it will define the following values
#
# LIBXML2_INCLUDE_PATH  = where libxml/xpath.h can be found
# LIBXML2_LIBRARIES      = the library to link against libxml2
# FOUND_LIBXML2        = set to 1 if libxml2 is found
#
IF(EXISTS ${PROJECT_CMAKE}/Libxml2Config.cmake)
  INCLUDE(${PROJECT_CMAKE}/Libxml2Config.cmake)
ENDIF(EXISTS ${PROJECT_CMAKE}/Libxml2Config.cmake)

IF(Libxml2_INCLUDE_DIRS)

  FIND_PATH(LIBXML2_INCLUDE_PATH libxml/xpath.h ${Libxml2_INCLUDE_DIRS})
  FIND_LIBRARY(LIBXML2_LIBRARIES xml2 ${Libxml2_LIBRARY_DIRS})

ELSE(Libxml2_INCLUDE_DIRS)

  SET(TRIAL_LIBRARY_PATHS
    $ENV{LIBXML2_HOME}/lib
    /usr/lib
    /usr/local/lib
    /sw/lib
  ) 
  SET(TRIAL_INCLUDE_PATHS
    $ENV{LIBXML2_HOME}/include/libxml2
    /usr/include/libxml2
    /usr/local/include/libxml2
    /sw/include/libxml2
  ) 

  FIND_LIBRARY(LIBXML2_LIBRARIES xml2 ${TRIAL_LIBRARY_PATHS})
  FIND_PATH(LIBXML2_INCLUDE_PATH libxml/xpath.h ${TRIAL_INCLUDE_PATHS})

ENDIF(Libxml2_INCLUDE_DIRS)

IF(LIBXML2_INCLUDE_PATH AND LIBXML2_LIBRARIES)
  SET(LIBXML2_FOUND 1 CACHE BOOL "Found libxml2 library")
ELSE(LIBXML2_INCLUDE_PATH AND LIBXML2_LIBRARIES)
  SET(LIBXML2_FOUND 0 CACHE BOOL "Not fount libxml2 library")
ENDIF(LIBXML2_INCLUDE_PATH AND LIBXML2_LIBRARIES)

MARK_AS_ADVANCED(
  LIBXML2_INCLUDE_PATH 
  LIBXML2_LIBRARIES 
  LIBXML2_FOUND
  )
