# https://github.com/kaldi-asr/kaldi/blob/master/cmake/FindnvToolsExt.cmake

# The following variables are optionally searched for defaults
#  nvToolsExt_ROOT_DIR:
#
# The following are set after configuration is done:
#  nvToolsExt_FOUND
#  nvToolsExt_INCLUDE_DIR
#  nvToolsExt_LIBRARIES
#  nvToolsExt_LIBRARY_DIR
#  nvToolsExt:                   a target

include(FindPackageHandleStandardArgs)

set(nvToolsExt_SEARCH_DIRS ${CUDA_TOOLKIT_ROOT_DIR})
if(WIN32)
    list(APPEND nvToolsExt_SEARCH_DIRS "C:/Program Files/NVIDIA Corporation/NvToolsExt")
endif()
set(nvToolsExt_SEARCH_DIRS ${nvToolsExt_ROOT_DIR} ${nvToolsExt_SEARCH_DIRS})


find_path(nvToolsExt_INCLUDE_DIR nvToolsExt.h HINTS ${nvToolsExt_SEARCH_DIRS} PATH_SUFFIXES include)

# 32bit not considered
set(nvToolsExt_LIBNAME nvToolsExt libnvToolsExt.so libnvToolsExt.a libnvToolsExt.so nvToolsExt64_1.lib)
find_library(nvToolsExt_LIBRARIES NAMES ${nvToolsExt_LIBNAME} HINTS ${nvToolsExt_SEARCH_DIRS}
    PATH_SUFFIXES lib lib64 cuda/lib cuda/lib64 lib/x64)

find_package_handle_standard_args(nvToolsExt REQUIRED_VARS nvToolsExt_INCLUDE_DIR nvToolsExt_LIBRARIES)

add_library(nvToolsExt INTERFACE)
target_include_directories(nvToolsExt INTERFACE ${nvToolsExt_INCLUDE_DIR})
target_link_libraries(nvToolsExt INTERFACE ${nvToolsExt_LIBRARIES})

unset(nvToolsExt_SEARCH_DIRS)
unset(nvToolsExt_LIBNAME)
