# Define the project name.
project(adonthell)

cmake_minimum_required(VERSION 2.4.0)

add_definitions(-DVERSION="\\"0.4.0\\"")


##########################
# Manage option
##########################
if(NOT ADONTHELL_DATADIR)
       MESSAGE(FATAL_ERROR
		"The ADONTHELL_DATADIR has not been defined, you can do it using -DADONTHELL_DATADIR on the cmake command line.")
else(NOT ADONTHELL_DATADIR)
       message(STATUS
		"Adonthell data directory: ${ADONTHELL_DATADIR}")
       add_definitions(-DDATA_DIR="\\"${ADONTHELL_DATADIR}\\"")
endif(NOT ADONTHELL_DATADIR)


# FIXME: We need to manage the package lib directory !
 add_definitions(-DPKGLIBDIR="\\"\\"")


##########################
# SDL
##########################
find_package(SDL REQUIRED)
if(SDL_FOUND)
	MESSAGE(STATUS
		"SDL has been found:")
	MESSAGE(STATUS
		"\tInclude Directory: ${SDL_INCLUDE_DIR}")
	MESSAGE(STATUS
		"\tLibraries: ${SDL_LIBRARY}")
else(SDL_FOUND)
	MESSAGE(FATAL_ERROR
		SDL is required)
endif(SDL_FOUND)


##########################
# SDL_mixer
##########################
find_package(SDL_mixer REQUIRED)
if(SDLMIXER_FOUND)
	MESSAGE(STATUS
		"SDL_mixer has been found:")
	MESSAGE(STATUS
		"\tInclude Directory: ${SDLMIXER_INCLUDE_DIR}")
	MESSAGE(STATUS
		"\tLibraries: ${SDLMIXER_LIBRARY}")
else(SDLMIXER_FOUND)
	MESSAGE(FATAL_ERROR
		"SDL_mixer is required")
endif(SDLMIXER_FOUND)


#######################
# Python
#######################
find_package(PythonLibs REQUIRED)
if(PYTHON_INCLUDE_PATH)
	message(STATUS
		"Python has been found:")
	message(STATUS
		"\tInclude directory: ${PYTHON_INCLUDE_PATH}")
	message(STATUS
		"\tLibraries: ${PYTHON_LIBRARIES}")
else(PYTHON_INCLUDE_PATH)
	message(FATAL_ERROR
		"A Python installation is required")
endif(PYTHON_INCLUDE_PATH)



#######################
# SWIG
#######################
#find_package(SWIG REQUIRED)


#######################
# LibXML2
#######################
include(config/FindLibxml2.cmake)
if(LIBXML2_FOUND)
	message(STATUS
		"LibXml2 has been found:")
	message(STATUS
		"\tInclude directory: ${LIBXML2_INCLUDE_PATH}")
	message(STATUS
		"\tLibraries: ${LIBXML2_LIBRARIES}")	
else(LIBXML2_FOUND)
	message(FATAL_ERROR
		"LibXml2 is required.")
endif(LIBXML2_FOUND)



######################
# Build the config.h file !
######################
include (CheckFunctionExists)
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

# Now generate the configure file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)


# Add the root of the binary distribution to the include directories !
include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_subdirectory(src)
