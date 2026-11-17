# Entry point for a grimace project: include(Grimace) once the toolchain file has chosen the target. Define the
# runtime libraries, then the commands a project calls: grimace_add_executable(), grimace_add_library() and
# grimace_package().
include_guard(GLOBAL)

get_filename_component(GRIMACE_TOOLCHAIN_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
enable_language(C)
enable_language(ASM)
enable_language(CXX)

# Run one of tools/*.py at configure time and return its output. Reconfigure when the tool changes.
function(_grimace_run_tool tool out_variable)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${GRIMACE_TOOLCHAIN_ROOT}/tools/${tool}")
    execute_process(
        COMMAND "${Python3_EXECUTABLE}" "${GRIMACE_TOOLCHAIN_ROOT}/tools/${tool}" ${ARGN}
        OUTPUT_VARIABLE output OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_VARIABLE error ERROR_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE result
    )
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "tools/${tool} failed\n${error}")
    endif()
    set(${out_variable} "${output}" PARENT_SCOPE)
endfunction()

include(GrimaceRuntime)
include(GrimaceGdb)
include(GrimaceTargets)
include(GrimacePackage)
