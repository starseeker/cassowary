# Minimum required version of CMake
cmake_minimum_required(VERSION 2.8)
if(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

# Set CMake project name
project(CASSOWARY)

# Set version
set(CASSOWARY_VERSION 0.7)

#---------------------------------------------------------------------
# CMake derives much of its functionality from modules, typically
# stored in one directory - let CMake know where to find them.  If we
# are a subbuild, let the parent's CMAKE_MODULE_PATH supply files before
set(CASSOWARY_CMAKE_DIR "${CASSOWARY_SOURCE_DIR}/CMake")
if(CASSOWARY_IS_SUBBUILD)
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CASSOWARY_CMAKE_DIR}")
else(CASSOWARY_IS_SUBBUILD)
  set(CMAKE_MODULE_PATH "${CASSOWARY_CMAKE_DIR};${CMAKE_MODULE_PATH}")
endif(CASSOWARY_IS_SUBBUILD)

#---------------------------------------------------------------------
# Define relative install locations.  Don't set these if they have already
# been set by some other means (like a higher level CMakeLists.txt file
# including this one).

# The location in which to install BRL-CAD executables.
if(NOT BIN_DIR)
  set(BIN_DIR bin)
endif(NOT BIN_DIR)

# The location in which to install BRL-CAD header files.
if(NOT INCLUDE_DIR)
  set(INCLUDE_DIR include)
endif(NOT INCLUDE_DIR)

# The location in which to install BRL-CAD libraries.
if(NOT LIB_DIR)
  set(LIB_DIR lib)
endif(NOT LIB_DIR)

# The location in which to install BRL-CAD configuration files.
if(NOT CONF_DIR)
  set(CONF_DIR etc)
endif(NOT CONF_DIR)

# The location in which to install BRL-CAD data files
if(NOT DATA_DIR)
  set(DATA_DIR share)
endif(NOT DATA_DIR)

# The location in which to install BRL-CAD Manual pages
if(NOT MAN_DIR)
  set(MAN_DIR ${DATA_DIR}/man)
endif(NOT MAN_DIR)

# The location in which to install BRL-CAD documentation files
if(NOT DOC_DIR)
  set(DOC_DIR ${DATA_DIR}/doc)
endif(NOT DOC_DIR)

# Make sure no absolute paths have been supplied to these variables
set(INSTALL_DIRS BIN INCLUDE LIB CONF DATA MAN DOC)
foreach(instdir ${INSTALL_DIRS})
  get_filename_component(instdir_full ${${instdir}_DIR} ABSOLUTE)
  if("${${instdir}_DIR}" STREQUAL "${instdir_full}")
    message(FATAL_ERROR "Error - absolute path supplied for ${instdir}_DIR.  This path must be relative - e.g. \"bin\" instead of \"/usr/bin\"")
    set(HAVE_INSTALL_DIR_FULL_PATH 1)
  endif("${${instdir}_DIR}" STREQUAL "${instdir_full}")
endforeach(instdir ${INSTALL_DIRS})

#---------------------------------------------------------------------
# The following logic is what allows binaries to run successfully in
# the build directory AND install directory.  Thanks to plplot for
# identifying the necessity of setting CMAKE_INSTALL_NAME_DIR on OSX.
# Documentation of these options is available at
# http://www.cmake.org/Wiki/CMake_RPATH_handling

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# the RPATH/INSTALL_NAME_DIR to be used when installing
if (NOT APPLE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_DIR}:\$ORIGIN/../${LIB_DIR}")
endif(NOT APPLE)
# On OSX, we need to set INSTALL_NAME_DIR instead of RPATH
# http://www.cmake.org/cmake/help/cmake-2-8-docs.html#variable:CMAKE_INSTALL_NAME_DIR
set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${LIB_DIR}")

# add the automatically determined parts of the RPATH which point to
# directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#---------------------------------------------------------------------
# Output directories - this is where built library and executable
# files will be placed after building but prior to install.  The
# necessary variables change between single and multi configuration
# build systems, so it is necessary to handle both cases on a
# conditional basis.

if(NOT CMAKE_CONFIGURATION_TYPES)
  # If we're not doing multi-configuration, just set the three main
  # variables to the correct values.
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CASSOWARY_BINARY_DIR}/${LIB_DIR} CACHE INTERNAL "Single output directory for building all libraries.")
  endif(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CASSOWARY_BINARY_DIR}/${LIB_DIR} CACHE INTERNAL "Single output directory for building all archives.")
  endif(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CASSOWARY_BINARY_DIR}/${BIN_DIR} CACHE INTERNAL "Single output directory for building all executables.")
  endif(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
else(NOT CMAKE_CONFIGURATION_TYPES)
  # Multi-configuration is more difficult.  Not only do we need to
  # properly set the output directories, but we also need to
  # identify the "toplevel" directory for each configuration so
  # we can place files, documentation, etc. in the correct
  # relative positions.  Because files may be placed by CMake
  # without a build target to put them in their proper relative build
  # directory position using these paths, we must fully qualify them
  # without using CMAKE_CFG_INTDIR.
  #
  # We define directories that may not be quite "standard"
  # for a particular build tool - for example, native VS2010 projects use
  # another directory to denote CPU type being compiled for - but CMake only
  # supports multi-configuration setups having multiple configurations,
  # not multiple compilers.
  #
  # One additional wrinkle we must watch for here is the case where
  # a multi-configuration setup uses "." for its internal directory -
  # if that's the case, we need to just set the various config output
  # directories to the same value.
  set(CFG_ROOT ${CASSOWARY_BINARY_DIR})
  foreach(CFG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
      set(CFG_ROOT ${CASSOWARY_BINARY_DIR}/${CFG_TYPE})
    endif(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
    string(TOUPPER "${CFG_TYPE}" CFG_TYPE_UPPER)
    if(NOT "CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
      set("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}" ${CFG_ROOT}/${LIB_DIR} CACHE INTERNAL "Single output directory for building ${CFG_TYPE} libraries.")
    endif(NOT "CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
    if(NOT "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
      set("CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}" ${CFG_ROOT}/${LIB_DIR} CACHE INTERNAL "Single output directory for building ${CFG_TYPE} archives.")
    endif(NOT "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
    if(NOT "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
      set("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}" ${CFG_ROOT}/${BIN_DIR} CACHE INTERNAL "Single output directory for building ${CFG_TYPE} executables.")
    endif(NOT "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
    if(NOT "CMAKE_BINARY_DIR_${CFG_TYPE_UPPER}")
      set("CMAKE_BINARY_DIR_${CFG_TYPE_UPPER}" ${CFG_ROOT} CACHE INTERNAL "Toplevel binary dir for ${CFG_TYPE} building.")
    endif(NOT "CMAKE_BINARY_DIR_${CFG_TYPE_UPPER}")
    if(NOT "CASSOWARY_BINARY_DIR_${CFG_TYPE_UPPER}")
      set("CASSOWARY_BINARY_DIR_${CFG_TYPE_UPPER}" ${CFG_ROOT} CACHE INTERNAL "Toplevel binary dir for ${CFG_TYPE} building.")
    endif(NOT "CASSOWARY_BINARY_DIR_${CFG_TYPE_UPPER}")
  endforeach()
endif(NOT CMAKE_CONFIGURATION_TYPES)

#------------------------------------------------------------------------------

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

configure_file(include/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)

add_subdirectory(src)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
