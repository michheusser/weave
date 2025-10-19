# Allow override via environment variable or CMake variable
if(DEFINED ENV{ONNXRUNTIME_ROOT})
    set(ONNXRuntime_ROOT $ENV{ONNXRUNTIME_ROOT})
elseif(NOT ONNXRuntime_ROOT)
    set(ONNXRuntime_ROOT "$ENV{HOME}/workspace/onnxruntime")
endif()

# Find header
find_path(ONNXRuntime_INCLUDE_DIR
        NAMES onnxruntime_cxx_api.h
        PATHS ${ONNXRuntime_ROOT}
        PATH_SUFFIXES include
        NO_DEFAULT_PATH
        DOC "ONNX Runtime include directory"
)

# Find library - different extensions for macOS vs Linux
if(APPLE)
    set(LIB_SUFFIXES lib)
    set(LIB_NAMES libonnxruntime.dylib onnxruntime)
else()
    set(LIB_SUFFIXES lib lib64)
    set(LIB_NAMES libonnxruntime.so onnxruntime)
endif()

find_library(ONNXRuntime_LIBRARY
        NAMES ${LIB_NAMES}
        PATHS ${ONNXRuntime_ROOT}
        PATH_SUFFIXES ${LIB_SUFFIXES}
        NO_DEFAULT_PATH
        DOC "ONNX Runtime library"
)

# Extract version from header
if(ONNXRuntime_INCLUDE_DIR AND EXISTS "${ONNXRuntime_INCLUDE_DIR}/onnxruntime_c_api.h")
    file(READ "${ONNXRuntime_INCLUDE_DIR}/onnxruntime_c_api.h" _ort_header_contents)
    string(REGEX MATCH "#define ORT_API_VERSION ([0-9]+)" _ort_version_match "${_ort_header_contents}")
    if(_ort_version_match)
        set(ONNXRuntime_VERSION_API ${CMAKE_MATCH_1})
        # Convert API version to semantic version (approximation)
        math(EXPR _major "${ONNXRuntime_VERSION_API} / 1000")
        math(EXPR _minor "(${ONNXRuntime_VERSION_API} % 1000) / 100")
        math(EXPR _patch "${ONNXRuntime_VERSION_API} % 100")
        set(ONNXRuntime_VERSION "${_major}.${_minor}.${_patch}")
    endif()
endif()

# Standard CMake package handling
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ONNXRuntime
        FOUND_VAR ONNXRuntime_FOUND
        REQUIRED_VARS ONNXRuntime_LIBRARY ONNXRuntime_INCLUDE_DIR
        VERSION_VAR ONNXRuntime_VERSION
)

# Create imported target
if(ONNXRuntime_FOUND)
    set(ONNXRuntime_LIBRARIES ${ONNXRuntime_LIBRARY})
    set(ONNXRuntime_INCLUDE_DIRS ${ONNXRuntime_INCLUDE_DIR})

    if(NOT TARGET ONNXRuntime::ONNXRuntime)
        add_library(ONNXRuntime::ONNXRuntime SHARED IMPORTED)

        get_filename_component(_lib_dir "${ONNXRuntime_LIBRARY}" DIRECTORY)

        set_target_properties(ONNXRuntime::ONNXRuntime PROPERTIES
                IMPORTED_LOCATION "${ONNXRuntime_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${ONNXRuntime_INCLUDE_DIR}"
                INTERFACE_LINK_OPTIONS "SHELL:-Wl,-rpath,${_lib_dir}"
        )
    endif()
    message(STATUS "ONNXRuntime found")
endif()

mark_as_advanced(ONNXRuntime_INCLUDE_DIR ONNXRuntime_LIBRARY)