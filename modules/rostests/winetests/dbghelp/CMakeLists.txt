
add_executable(dbghelp_winetest dbghelp.c testlist.c)
target_compile_definitions(dbghelp_winetest PRIVATE WINETEST_USE_DBGSTR_LONGLONG __WINESRC__ strcasecmp=_stricmp)
set_module_type(dbghelp_winetest win32cui)
add_importlibs(dbghelp_winetest psapi dbghelp gdi32 user32 msvcrt kernel32 ntdll)
target_link_libraries(dbghelp_winetest oldnames)
add_rostests_file(TARGET dbghelp_winetest)
set_wine_module(dbghelp_winetest) # Required for winternl.h
