
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(BEFORE
    ${REACTOS_BINARY_DIR}/dll/win32/netapi32)
add_rpc_file(client schrpc.idl)
spec2def(taskschd.dll taskschd.spec)

list(APPEND SOURCE
    folder_collection.c
    folder.c
    regtask.c
    task.c
    taskschd.c
    ${CMAKE_CURRENT_BINARY_DIR}/schrpc_c.c
    ${CMAKE_CURRENT_BINARY_DIR}/taskschd_stubs.c)

add_library(taskschd MODULE
    ${SOURCE}
    ${CMAKE_CURRENT_BINARY_DIR}/taskschd.def)

set_module_type(taskschd win32dll)
add_idl_reg_scripts(taskschd regtypelib taskschd_tlb.idl)
target_link_libraries(taskschd uuid wine wine_dll_canunload wine_dll_register oldnames)
add_importlibs(taskschd ole32 oleaut32 advapi32 rpcrt4 xmllite msvcrt kernel32 kernel32_vista ntdll)
add_dependencies(taskschd netapi32 wineheaders)
add_cd_file(TARGET taskschd DESTINATION reactos/system32 FOR all)
set_wine_module_FIXME(taskschd) # CORE-5743: No CONST_VTABLE
