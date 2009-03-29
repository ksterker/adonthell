prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=${exec_prefix}/lib
includedir=${prefix}/include
datadir=${prefix}/share/adonthell

Name: Adonthell
Description: Role Playing Game Engine
Version: @VERSION@
Libs: -L${libdir} -ladonthell_py_runtime -ladonthell_base -ladonthell_python -ladonthell_gfx -ladonthell_input -ladonthell_event -ladonthell_rpg -ladonthell_world
Cflags: -I${includedir}/adonthell

