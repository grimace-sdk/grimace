# Write a gdb script beside each program, <program>.elf.gdb, for an emulator's GDB stub (see README): load the
# symbols, search the build and GRIMACE_OS_SYMBOLS for DLLs, and define grimace-load to copy the program and
# the project's DLLs to \Windows. The scripts are written once at the end of configure, so they list every DLL
# the project defines, and are templated from grimace.gdb.in.
include_guard(GLOBAL)

function(_grimace_gdb_scripts)
    get_property(executables GLOBAL PROPERTY GRIMACE_EXECUTABLES)
    get_property(libraries GLOBAL PROPERTY GRIMACE_LIBRARIES)
    set(library_folders "")
    set(uploads "")
    foreach(library IN LISTS libraries)
        get_target_property(output ${library} GRIMACE_OUTPUT)
        list(APPEND library_folders "$<TARGET_FILE_DIR:${library}>")
        list(APPEND uploads "  remote put \"$<TARGET_FILE_DIR:${library}>/${output}\" /Windows/${output}")
    endforeach()
    set(os_symbols "")
    if(GRIMACE_OS_SYMBOLS)
        list(APPEND os_symbols
            "set $grimace_os_symbols = \"${GRIMACE_OS_SYMBOLS}\""
            "source ${GRIMACE_TOOLCHAIN_ROOT}/tools/gdbsymbols.py"
        )
        list(APPEND library_folders "${GRIMACE_OS_SYMBOLS}")
        list(APPEND uploads "  sharedlibrary")
    endif()
    list(JOIN os_symbols "\n" os_symbols)
    list(JOIN uploads "\n" uploads)
    set(architecture "")
    if(GRIMACE_ARCH STREQUAL "sh3")
        set(architecture "set architecture sh3")
    elseif(GRIMACE_ARCH STREQUAL "arm")
        set(architecture "set architecture armv4")
    endif()
    foreach(target IN LISTS executables)
        get_target_property(output ${target} GRIMACE_OUTPUT)
        set(search_path "$<TARGET_FILE_DIR:${target}>" ${library_folders})
        list(JOIN search_path ":" search_path)
        set(configured "${CMAKE_BINARY_DIR}/grimace/gdb/${target}.gdb")
        configure_file("${GRIMACE_TOOLCHAIN_ROOT}/cmake/grimace.gdb.in" "${configured}" @ONLY)
        file(GENERATE OUTPUT "$<TARGET_FILE:${target}>.gdb" INPUT "${configured}")
    endforeach()
endfunction()

# Record a program or DLL for the scripts, and arrange to write them once configure is done.
function(_grimace_add_gdb_script target output kind)
    set_target_properties(${target} PROPERTIES GRIMACE_OUTPUT "${output}")
    set_property(GLOBAL APPEND PROPERTY ${kind} ${target})
    get_property(deferred GLOBAL PROPERTY GRIMACE_GDB_SCRIPTS_DEFERRED)
    if(NOT deferred)
        set_property(GLOBAL PROPERTY GRIMACE_GDB_SCRIPTS_DEFERRED TRUE)
        cmake_language(DEFER DIRECTORY "${CMAKE_SOURCE_DIR}" CALL _grimace_gdb_scripts)
    endif()
endfunction()
