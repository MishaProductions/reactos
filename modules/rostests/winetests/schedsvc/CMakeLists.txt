

add_rpc_files(client atsvc.idl schrpc.idl)
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/wine)
add_executable(schedsvc_winetest atsvcapi.c rpcapi.c ${CMAKE_CURRENT_BINARY_DIR}/atsvc_c.c ${CMAKE_CURRENT_BINARY_DIR}/schrpc_c.c)
set_module_type(schedsvc_winetest win32cui)
add_importlibs(schedsvc_winetest advapi32 ole32 oleaut32 rpcrt4 user32 gdi32 msvcrt kernel32)
target_link_libraries(schedsvc_winetest uuid winetestlib)
add_rostests_file(TARGET schedsvc_winetest)
