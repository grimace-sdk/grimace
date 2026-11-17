# Define the import libraries, C runtime and C++ runtime. Build only what a program links.
include_guard(GLOBAL)

set(GRIMACE_IMPORTS_DIR "${CMAKE_BINARY_DIR}/grimace/imports")
# Import stubs are assembly, so they need the ARM variant: ARMv4I on CE 4.0 and later.
set(GRIMACE_IMPLIB_ARCH "${GRIMACE_ARCH}")
if(GRIMACE_ARCH STREQUAL "arm" AND GRIMACE_WIN32_WCE GREATER_EQUAL 400)
    set(GRIMACE_IMPLIB_ARCH armv4i)
endif()

# Work out the target's imports from the export lists in exports/: a <dll>.txt per DLL it can import from, and a
# <dll>.lazy naming the imports to bind on first call. Reconfigure when a list changes.
file(GLOB GRIMACE_MANIFEST_FILES "${GRIMACE_TOOLCHAIN_ROOT}/exports/*/*")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${GRIMACE_MANIFEST_FILES})
_grimace_run_tool(manifests.py GRIMACE_IMPORTS_SUMMARY
    --arch "${GRIMACE_ARCH}" --version "${GRIMACE_CE_VERSION}" "--platform=${GRIMACE_PLATFORM}"
    "--device=${GRIMACE_DEVICE}" --output "${GRIMACE_IMPORTS_DIR}"
)
message(STATUS "${GRIMACE_IMPORTS_SUMMARY}")

# GRIMACE_OS_SYMBOLS is a folder with the device ROM's symbols, nk.elf and rom.elf, for the gdb scripts. Take it
# from the environment unless set, from GRIMACE_SH3_OS_SYMBOLS for SH3.
if(NOT DEFINED GRIMACE_OS_SYMBOLS)
    if(GRIMACE_ARCH STREQUAL "sh3")
        set(GRIMACE_OS_SYMBOLS "$ENV{GRIMACE_SH3_OS_SYMBOLS}")
    else()
        set(GRIMACE_OS_SYMBOLS "$ENV{GRIMACE_OS_SYMBOLS}")
    endif()
endif()
if(GRIMACE_OS_SYMBOLS)
    get_filename_component(GRIMACE_OS_SYMBOLS "${GRIMACE_OS_SYMBOLS}" ABSOLUTE)
    if(NOT EXISTS "${GRIMACE_OS_SYMBOLS}/nk.elf" OR NOT EXISTS "${GRIMACE_OS_SYMBOLS}/rom.elf")
        message(WARNING "GRIMACE_OS_SYMBOLS is ${GRIMACE_OS_SYMBOLS}, which lacks nk.elf or rom.elf")
    endif()
endif()

# grimace::headers: the SDK headers, plus libc++'s for C++. -femulated-tls: thread_local variables are kept by
# runtime/c99/emutls.c, as the loader does not set up thread-local storage.
add_library(grimace_headers INTERFACE)
if(GRIMACE_LIBCXX)
    target_include_directories(grimace_headers SYSTEM INTERFACE
        "$<$<COMPILE_LANGUAGE:CXX>:${GRIMACE_TOOLCHAIN_ROOT}/include/grimace/libcxx>"
        "$<$<COMPILE_LANGUAGE:CXX>:${GRIMACE_LLVM_ROOT}/include/c++/v1>"
    )
endif()
target_include_directories(grimace_headers SYSTEM INTERFACE
    "${GRIMACE_TOOLCHAIN_ROOT}/include" "${GRIMACE_TOOLCHAIN_ROOT}/include/w32api"
)
target_compile_options(grimace_headers INTERFACE -femulated-tls)
add_library(grimace::headers ALIAS grimace_headers)

# grimace::runtime: startup, DllMain, lazy import binding, compiler-rt's builtins, the C99 additions, and the
# runtime/libc functions the target's coredll lacks, as tools/runtime.py picks them. -fno-builtin stops clang
# turning a libc function into a call to itself.
file(GLOB_RECURSE GRIMACE_LIBC_FILES "${GRIMACE_TOOLCHAIN_ROOT}/runtime/libc/*")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${GRIMACE_LIBC_FILES})
_grimace_run_tool(runtime.py GRIMACE_LIBC_SUMMARY
    --arch "${GRIMACE_ARCH}" --imports "${GRIMACE_IMPORTS_DIR}" --output "${GRIMACE_IMPORTS_DIR}/libc-sources"
)
file(STRINGS "${GRIMACE_IMPORTS_DIR}/libc-sources" GRIMACE_LIBC_SOURCES)
file(GLOB GRIMACE_RUNTIME_SOURCES
    "${GRIMACE_TOOLCHAIN_ROOT}/runtime/c99/*.c" "${GRIMACE_TOOLCHAIN_ROOT}/runtime/compiler-rt/*.c"
    "${GRIMACE_TOOLCHAIN_ROOT}/runtime/*.c"
)
add_library(grimace_runtime STATIC EXCLUDE_FROM_ALL ${GRIMACE_RUNTIME_SOURCES} ${GRIMACE_LIBC_SOURCES})
target_include_directories(grimace_runtime PRIVATE "${GRIMACE_TOOLCHAIN_ROOT}/include")
target_compile_options(grimace_runtime PRIVATE -fno-builtin -w)
add_library(grimace::runtime ALIAS grimace_runtime)

# grimace::posix: the POSIX layer for ported code, opt in. Its headers shadow the SDK's.
file(GLOB GRIMACE_POSIX_SOURCES "${GRIMACE_TOOLCHAIN_ROOT}/runtime/posix/*.c")
add_library(grimace_posix STATIC EXCLUDE_FROM_ALL ${GRIMACE_POSIX_SOURCES})
target_include_directories(grimace_posix SYSTEM PUBLIC "${GRIMACE_TOOLCHAIN_ROOT}/include/grimace/posix")
target_compile_definitions(grimace_posix PUBLIC GRIMACE_POSIX)
target_link_libraries(grimace_posix PRIVATE grimace_headers)
add_library(grimace::posix ALIAS grimace_posix)

# grimace::cxx_runtime: new, delete and the C++ ABI support every C++ program links. grimace::rtti turns RTTI on
# for a target, as it is off by default.
file(GLOB GRIMACE_CXX_RUNTIME_SOURCES "${GRIMACE_TOOLCHAIN_ROOT}/runtime/cxx/*.cpp")
add_library(grimace_cxx_runtime STATIC EXCLUDE_FROM_ALL ${GRIMACE_CXX_RUNTIME_SOURCES})
target_link_libraries(grimace_cxx_runtime PRIVATE grimace_headers)
target_compile_options(grimace_cxx_runtime PRIVATE -frtti)
add_library(grimace::cxx_runtime ALIAS grimace_cxx_runtime)

add_library(grimace_rtti INTERFACE)
target_compile_options(grimace_rtti INTERFACE "$<$<COMPILE_LANGUAGE:CXX>:-frtti>")
add_library(grimace::rtti ALIAS grimace_rtti)

# With GRIMACE_LIBCXX, from toolchain.cmake, build the parts of libc++'s library that work without
# exceptions or locales: grimace::cxx_stdlib, which every C++ program links, and grimace::iostreams, which a
# program links to use <iostream>. runtime/libcxx supplies what the sources expect from the OS.
set(GRIMACE_CXX_LIBRARIES grimace::cxx_runtime)
if(GRIMACE_LIBCXX)
    set(GRIMACE_LIBCXX_SOURCE "${GRIMACE_LLVM_ROOT}/share/libcxx/src")

    set(GRIMACE_CXX_STDLIB_SOURCES
        algorithm any bind call_once chrono error_category expected functional hash memory optional random_shuffle
        stdexcept string system_error variant vector
    )
    list(TRANSFORM GRIMACE_CXX_STDLIB_SOURCES PREPEND "${GRIMACE_LIBCXX_SOURCE}/")
    list(TRANSFORM GRIMACE_CXX_STDLIB_SOURCES APPEND ".cpp")
    file(GLOB GRIMACE_LIBCXX_SUPPORT_SOURCES "${GRIMACE_TOOLCHAIN_ROOT}/runtime/libcxx/*.cpp")
    add_library(grimace_cxx_stdlib STATIC EXCLUDE_FROM_ALL
        ${GRIMACE_LIBCXX_SUPPORT_SOURCES} ${GRIMACE_CXX_STDLIB_SOURCES}
    )
    target_link_libraries(grimace_cxx_stdlib PRIVATE grimace_headers)
    target_include_directories(grimace_cxx_stdlib PRIVATE "${GRIMACE_LIBCXX_SOURCE}")
    target_compile_definitions(grimace_cxx_stdlib PRIVATE _LIBCPP_BUILDING_LIBRARY)
    target_compile_options(grimace_cxx_stdlib PRIVATE -std=c++23 -frtti -w)
    set_source_files_properties("${GRIMACE_LIBCXX_SOURCE}/string.cpp"
        PROPERTIES COMPILE_DEFINITIONS GRIMACE_C99_SWPRINTF
    )
    set_source_files_properties("${GRIMACE_LIBCXX_SOURCE}/chrono.cpp"
        PROPERTIES COMPILE_DEFINITIONS _LIBCPP_HAS_TIMESPEC_GET
    )
    add_library(grimace::cxx_stdlib ALIAS grimace_cxx_stdlib)
    list(PREPEND GRIMACE_CXX_LIBRARIES grimace::cxx_stdlib)

    set(GRIMACE_CXX_IOSTREAMS_SOURCES fstream ios ios.instantiations iostream locale ostream strstream)
    list(TRANSFORM GRIMACE_CXX_IOSTREAMS_SOURCES PREPEND "${GRIMACE_LIBCXX_SOURCE}/")
    list(TRANSFORM GRIMACE_CXX_IOSTREAMS_SOURCES APPEND ".cpp")
    add_library(grimace_cxx_iostreams STATIC EXCLUDE_FROM_ALL ${GRIMACE_CXX_IOSTREAMS_SOURCES})
    target_link_libraries(grimace_cxx_iostreams PRIVATE grimace_headers)
    target_include_directories(grimace_cxx_iostreams PRIVATE "${GRIMACE_LIBCXX_SOURCE}")
    target_compile_definitions(grimace_cxx_iostreams PRIVATE _LIBCPP_BUILDING_LIBRARY GRIMACE_IOSTREAMS)
    target_compile_options(grimace_cxx_iostreams PRIVATE -std=c++23 -frtti -w)
    add_library(grimace_iostreams INTERFACE)
    target_compile_definitions(grimace_iostreams INTERFACE GRIMACE_IOSTREAMS)
    target_link_libraries(grimace_iostreams INTERFACE grimace_cxx_iostreams)
    add_library(grimace::iostreams ALIAS grimace_iostreams)
endif()

# grimace::<dll>: an import library per DLL, weak stubs from tools/mkimplib.py, so a program only carries the
# imports it calls and may define a name itself.
file(GLOB GRIMACE_EXPORT_LISTS "${GRIMACE_IMPORTS_DIR}/*.txt")
foreach(exports IN LISTS GRIMACE_EXPORT_LISTS)
    get_filename_component(dll "${exports}" NAME_WLE)
    string(TOUPPER "${dll}" dll_upper)
    set(lazy "${GRIMACE_IMPORTS_DIR}/${dll}.lazy")
    set(stubs "${GRIMACE_IMPORTS_DIR}/${dll}.S")
    add_custom_command(
        OUTPUT "${stubs}"
        COMMAND "${Python3_EXECUTABLE}" "${GRIMACE_TOOLCHAIN_ROOT}/tools/mkimplib.py" "${exports}" "${dll_upper}.dll"
                "${stubs}" --arch ${GRIMACE_IMPLIB_ARCH} --lazy "${lazy}"
        DEPENDS "${exports}" "${lazy}" "${GRIMACE_TOOLCHAIN_ROOT}/tools/mkimplib.py"
        VERBATIM
    )
    add_library(grimace_${dll} STATIC EXCLUDE_FROM_ALL "${stubs}")
    add_library(grimace::${dll} ALIAS grimace_${dll})
endforeach()
