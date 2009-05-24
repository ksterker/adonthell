AM_CXXFLAGS = -I$(top_srcdir)/src

SUBDIRS = data

EXTRA_DIST = README audiotest.py dialogtest.py gfxsave.py itemtest.py \
    worldtest.py configtest.py equipmenttest.py  gfxtest.py logtest.py \
    convert_data.py eventtest.py guitest.py questtest.py \
    convert_quests.py filereport.py inputtest.py searchtest.py \
    convert_graphics.py CMakeLists.txt README.worldtest smallworld.cc

noinst_PROGRAMS = callbacktest diskiotest guitest inputtest gfxtest worldtest \
    imagetest dialogtest path_test

callbacktest_SOURCES = callbacktest.cc
callbacktest_LDADD = -L$(top_builddir)/src/base/ -ladonthell_base

diskiotest_SOURCES = diskiotest.cc
diskiotest_LDADD = -L$(top_builddir)/src/base/ -ladonthell_base

guitest_CXXFLAGS = $(FT2_CFLAGS) -I$(top_srcdir)/src $(PY_CFLAGS)
guitest_SOURCES = guitest.cc
guitest_LDADD = \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS)\
	-L$(top_builddir)/src/gui/ -ladonthell_gui                 \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                 \
	-L$(top_builddir)/src/input/ -ladonthell_input             \
	-L$(top_builddir)/src/audio/ -ladonthell_audio             \
	-L$(top_builddir)/src/event/ -ladonthell_event             \
	-L$(top_builddir)/src/base/ -ladonthell_base               \
	-L$(top_builddir)/src/main -ladonthell_main                \
	-L$(top_builddir)/src/rpg/ -ladonthell_rpg			\
	-L${top_builddir}/src/world/ -ladonthell_world             \
	-L$(top_builddir)/src/py-runtime -ladonthell_py_runtime \
	$(FT2_LIBS)
    
dialogtest_CXXFLAGS = $(FT2_CFLAGS) -I$(top_srcdir)/src $(PY_CFLAGS)
dialogtest_SOURCES = dialogtest.cc
dialogtest_LDADD = \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS) \
	-L$(top_builddir)/src/gui/ -ladonthell_gui 			\
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx 			\
	-L$(top_builddir)/src/input/ -ladonthell_input			\
	-L$(top_builddir)/src/audio/ -ladonthell_audio			\
	-L$(top_builddir)/src/event/ -ladonthell_event			\
	-L$(top_builddir)/src/base/ -ladonthell_base			\
	-L$(top_builddir)/src/rpg/ -ladonthell_rpg			\
	-L${top_builddir}/src/world/ -ladonthell_world             	\
	-L$(top_builddir)/src/main/ -ladonthell_main			\
	-L$(top_builddir)/src/py-runtime -ladonthell_py_runtime \
	$(FT2_LIBS)

inputtest_SOURCES = inputtest.cc
inputtest_LDADD = \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS) \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                  \
	-L$(top_builddir)/src/input/ -ladonthell_input              \
	-L$(top_builddir)/src/audio/ -ladonthell_audio              \
	-L$(top_builddir)/src/event/ -ladonthell_event              \
	-L$(top_builddir)/src/base/ -ladonthell_base                \
	-L$(top_builddir)/src/main -ladonthell_main                 \
	-L${top_builddir}/src/world/ -ladonthell_world              \
	-L$(top_builddir)/src/py-runtime -ladonthell_py_runtime

gfxtest_SOURCES = gfxtest.cc
gfxtest_LDADD = -L$(top_builddir)/src/base/ -ladonthell_base        \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS) \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                  \
	-L$(top_builddir)/src/event/ -ladonthell_event              \
	-L$(top_builddir)/src/py-runtime -ladonthell_py_runtime

worldtest_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS) 
worldtest_SOURCES = worldtest.cc
worldtest_LDADD = $(PY_LIBS) \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS) \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                  \
    -L$(top_builddir)/src/gui/ -ladonthell_gui                  \
	-L$(top_builddir)/src/input/ -ladonthell_input              \
	-L$(top_builddir)/src/audio/ -ladonthell_audio              \
	-L$(top_builddir)/src/event/ -ladonthell_event              \
	-L$(top_builddir)/src/base/ -ladonthell_base                \
	-L$(top_builddir)/src/main -ladonthell_main                 \
	-L$(top_builddir)/src/rpg/ -ladonthell_rpg                  \
	-L${top_builddir}/src/world/ -ladonthell_world              \
	-L$(top_builddir)/src/py-runtime -ladonthell_py_runtime

path_test_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS) 
path_test_SOURCES = path_test.cc
path_test_LDADD = $(PY_LIBS) \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS) \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                  \
	-L$(top_builddir)/src/input/ -ladonthell_input              \
	-L$(top_builddir)/src/audio/ -ladonthell_audio              \
	-L$(top_builddir)/src/event/ -ladonthell_event              \
	-L$(top_builddir)/src/base/ -ladonthell_base                \
	-L$(top_builddir)/src/main -ladonthell_main                 \
	-L${top_builddir}/src/world/ -ladonthell_world              \
	-L$(top_builddir)/src/py-runtime -ladonthell_py_runtime

imagetest_SOURCES = imagetest.cc
imagetest_LDADD = $(PY_LIBS) \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS) \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                  \
	-L$(top_builddir)/src/input/ -ladonthell_input              \
	-L$(top_builddir)/src/audio/ -ladonthell_audio              \
	-L$(top_builddir)/src/event/ -ladonthell_event              \
	-L$(top_builddir)/src/base/ -ladonthell_base                \
	-L$(top_builddir)/src/main -ladonthell_main                 \
	-L${top_builddir}/src/world/ -ladonthell_world              \
	-L$(top_builddir)/src/py-runtime -ladonthell_py_runtime
