
spec2def(uiautomationcore.dll uiautomationcore.spec ADD_IMPORTLIB)
list(APPEND SOURCE
    uia_client.c
    uia_com_client.c
    uia_event.c
    uia_ids.c
    uia_main.c
    uia_provider.c
    uia_utils.c)

list(APPEND PCH_SKIP_SOURCE
    ${CMAKE_CURRENT_BINARY_DIR}/uiautomationcore_stubs.c)

add_library(uiautomationcore MODULE
    ${SOURCE}
    ${PCH_SKIP_SOURCE}
    uiautomationcore.rc
    ${CMAKE_CURRENT_BINARY_DIR}/uiautomationcore.def)
add_idl_headers(uiautomationcore_idlheader uia_classes.idl)

set_module_type(uiautomationcore win32dll)
target_link_libraries(uiautomationcore uuid wine ${PSEH_LIB} oldnames)
add_delay_importlibs(uiautomationcore advpack)
add_importlibs(uiautomationcore ole32 oleaut32 oleacc user32 kernel32 msvcrt kernel32 ntdll)
add_pch(uiautomationcore precomp.h "${PCH_SKIP_SOURCE}")
add_dependencies(uiautomationcore uiautomationcore_idlheader)
add_cd_file(TARGET uiautomationcore DESTINATION reactos/system32 FOR all)
set_wine_module_FIXME(uiautomationcore) # CORE-5743: No ARRAY_SIZE macro
