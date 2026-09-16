
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/wine)
add_executable(taskschd_winetest scheduler.c)
set_module_type(taskschd_winetest win32cui)
add_importlibs(taskschd_winetest advapi32 ole32 oleaut32 user32 gdi32 msvcrt kernel32)
target_link_libraries(taskschd_winetest oldnames uuid winetestlib)
add_rostests_file(TARGET taskschd_winetest)
