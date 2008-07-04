# Copyright (c) 2006, Tristan Carel
# All rights reserved.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the University of California, Berkeley nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# internal macro
MACRO(PythonLibs_get_variable PythonVar OutVar)
  SET(PythonLibs_get_variable_command
    "import distutils.sysconfig, sys
sys.stdout.write(distutils.sysconfig.get_config_var(\"${PythonVar}\"))")
  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c ${PythonLibs_get_variable_command}
    OUTPUT_VARIABLE ${OutVar}
    ERROR_VARIABLE PythonLibs_get_variable_error
    RESULT_VARIABLE PythonLibs_get_variable_result
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  IF(NOT ${PythonLibs_get_variable_result} EQUAL 0)
    MESSAGE(SEND_ERROR "Command \"${PythonInterp_EXECUTABLE} -c ${PythonLibs_get_variable_command} failed with output:\n${PythonLibs_get_variable_error}")
  ENDIF(NOT ${PythonLibs_get_variable_result} EQUAL 0)
ENDMACRO(PythonLibs_get_variable)

# internal macro
MACRO(PythonLibs_get_site_package_dir OutVar)
  SET(PythonLibs_get_site_package_dir_command
    "import distutils.sysconfig, sys
sys.stdout.write(distutils.sysconfig.get_python_lib(0,0))")
  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c ${PythonLibs_get_site_package_dir_command}
    OUTPUT_VARIABLE ${OutVar}
    ERROR_VARIABLE PythonLibs_get_site_package_dir_error
    RESULT_VARIABLE PythonLibs_get_site_package_dir_result
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  IF(NOT ${PythonLibs_get_site_package_dir_result} EQUAL 0)
    MESSAGE(SEND_ERROR "Command \"${PythonInterp_EXECUTABLE} -c ${PythonLibs_get_site_package_dir_command} failed with output:\n${PythonLibs_get_site_package_dir_error}")
  ENDIF(NOT ${PythonLibs_get_site_package_dir_result} EQUAL 0)
ENDMACRO(PythonLibs_get_site_package_dir)

SET(PYTHON_SITE_PACKAGE_DIR "")
PythonLibs_get_site_package_dir (PYTHON_SITE_PACKAGE_DIR)

SET(PYTHON_EXTRA_LIBRARIES "")
IF(NOT WIN32)
  # Also link with Python dependencies (needed when using static version)
  PythonLibs_get_variable("LIBS" PythonLibs_extradeps)
  PythonLibs_get_variable("SYSLIBS" PythonLibs_sysextradeps)
  LIST(APPEND PYTHON_EXTRA_LIBRARIES ${PythonLibs_sysextradeps} ${PythonLibs_extradeps})
ENDIF(NOT WIN32)
