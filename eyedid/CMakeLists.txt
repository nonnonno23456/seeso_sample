cmake_minimum_required(VERSION 3.5)
project(eyedid)

set(CMAKE_CXX_STANDARD 14)

set(EYEDID_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(EYEDID_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/eyedid")
set(EYEDID_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(EYEDID_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

if("${CMAKE_BUILD_TYPE}" STREQUAL Release)
    set(BUILD_TYPE_LOWER release)
else()
    set(BUILD_TYPE_LOWER debug)
endif()

if(WIN32)
    set(EYEDID_DLL
        "${EYEDID_BIN_DIR}/eyedid/eyedid_core.dll"
        "${EYEDID_BIN_DIR}/third-parties/opencv_world410.dll"
        "${EYEDID_BIN_DIR}/third-parties/libcurl.dll"
        "${EYEDID_BIN_DIR}/third-parties/tbb12.dll"
    )
    set(EYEDID_DLL ${EYEDID_DLL} PARENT_SCOPE)

    add_library(eyedid STATIC
            ${EYEDID_DIR}/gaze_tracker.cc
            ${EYEDID_DIR}/framework/core_callback.cc
            ${EYEDID_DIR}/util/display_w32.cc
            )

    message("EYEDID dll: ${EYEDID_DLL}")
    set_target_properties(eyedid PROPERTIES WINDOWS_DLL_FILES "${EYEDID_DLL}")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if(NOT "${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "^(arm64|x86_64)")
        message(FATAL_ERROR "Unsupported host architecture")
    endif()

    MESSAGE(STATUS "CC CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
    set(EYEDID_CORE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/macos/${CMAKE_HOST_SYSTEM_PROCESSOR}/${BUILD_TYPE_LOWER}/libeyedid_core.dylib)
    add_library(libeyedidcore UNKNOWN IMPORTED)
    set_target_properties(libeyedidcore PROPERTIES IMPORTED_LOCATION "${EYEDID_CORE_LIB}")

    find_library(FoundationLib Foundation REQUIRED)
    find_library(FoundationLib CoreFoundation REQUIRED)
    find_library(FoundationLib CoreGraphics REQUIRED)
    find_library(FoundationLib Cocoa REQUIRED)

    add_library(eyedid STATIC
            ${EYEDID_DIR}/gaze_tracker.cc
            ${EYEDID_DIR}/framework/core_callback.cc
            ${EYEDID_DIR}/util/display_cocoa.mm
            )

    set_source_files_properties(${EYEDID_DIR}/util/display_cocoa.mm PROPERTIES
            COMPILE_FLAGS "-x objective-c++")

    target_link_libraries(eyedid PUBLIC
            libeyedidcore
            "-framework Foundation"
            "-framework CoreFoundation"
            "-framework CoreGraphics"
            "-framework Cocoa")

    if(CMAKE_HOST_SYSTEM_VERSION VERSION_GREATER_EQUAL 9)
        get_filename_component(binary_dir ${EYEDID_CORE_LIB} DIRECTORY)
        target_link_options(eyedid INTERFACE "LINKER:-rpath,${binary_dir}")
    else()
        message(AUTHOR_WARNING "Building for below macOS 10.5 requires double-configure CMake to run executables")
    endif()

elseif(UNIX)
    if(NOT "${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "^(arm64|x86_64)")
        message(FATAL_ERROR "Unsupported host architecture")
    endif()

    set(EYEDID_CORE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/${CMAKE_SYSTEM_PROCESSOR}/${BUILD_TYPE_LOWER}/libeyedid_core.so)
    add_library(libeyedidcore UNKNOWN IMPORTED)
    set_target_properties(libeyedidcore PROPERTIES IMPORTED_LOCATION "${EYEDID_CORE_LIB}")

    add_library(eyedid STATIC
            ${EYEDID_DIR}/gaze_tracker.cc
            ${EYEDID_DIR}/framework/core_callback.cc
            )

    target_link_libraries(eyedid PUBLIC ${EYEDID_CORE_LIB})

else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

target_include_directories(eyedid PUBLIC ${EYEDID_INCLUDE_DIR})

