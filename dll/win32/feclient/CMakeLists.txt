
spec2def(feclient.dll feclient.spec)

list(APPEND SOURCE
    ${CMAKE_CURRENT_BINARY_DIR}/feclient_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/feclient.def)

add_library(feclient MODULE ${SOURCE})
set_module_type(feclient win32dll UNICODE ENTRYPOINT 0)
target_link_libraries(feclient wine)

add_importlibs(feclient user32 kernel32 ntdll)
add_cd_file(TARGET feclient DESTINATION reactos/system32 FOR all)
set_wine_module(feclient)
