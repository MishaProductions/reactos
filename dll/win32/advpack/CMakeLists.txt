
spec2def(advpack.dll advpack.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    advpack.c
    files.c
    install.c
    reg.c
    precomp.h)

add_library(advpack MODULE
    ${SOURCE}
    advpack.rc
    ${CMAKE_CURRENT_BINARY_DIR}/advpack.def)

set_module_type(advpack win32dll)
target_link_libraries(advpack wine)
add_importlibs(advpack ole32 setupapi version advapi32 msvcrt kernel32 ntdll)
add_pch(advpack precomp.h SOURCE)
add_cd_file(TARGET advpack DESTINATION reactos/system32 FOR all)
set_wine_module_FIXME(advpack) # CORE-5743: No ARRAY_SIZE macro
