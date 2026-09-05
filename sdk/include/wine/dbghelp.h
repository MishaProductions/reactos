#pragma once
#include <psdk/dbghelp.h>

/* Wine extensions to dbghelp */
enum dhext_module_type
{
    DMT_UNKNOWN,        /* for lookup, not actually used for a module */
    DMT_ELF,            /* a real ELF shared module */
    DMT_MACHO,          /* a real Mach-O shared module */
    DMT_PE,             /* a native or builtin PE module */
};

/* only reporting the formats not exposed in regular IMAGHELP_MODULE_INFO */
enum dhext_debug_format
{
    DHEXT_FORMAT_DWARF2     = 0x0001,
    DHEXT_FORMAT_DWARF3     = 0x0002,
    DHEXT_FORMAT_DWARF4     = 0x0004,
    DHEXT_FORMAT_DWARF5     = 0x0008,
    DHEXT_FORMAT_STABS      = 0x0010,
};

struct dhext_module_information
{
    enum dhext_module_type      type;
    unsigned                    is_wine_builtin : 1,
                                is_virtual : 1,
                                has_file_image : 1;
    unsigned                    debug_format_bitmask;
};

extern BOOL WINAPI wine_get_module_information(HANDLE, DWORD64 base, struct dhext_module_information*, unsigned len);
