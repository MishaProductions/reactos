
#include <syscalls.inc>

#define SVC_(name, argcount) SyscallId = SyscallId + 1
#include <sysfuncs_nt60.h>
#undef SVC_
#undef SYSFUNCS_NT5_ONLY

/* Now do the NT6 ones proper */
#define SYSFUNCS_NT6_ONLY

#ifdef _M_ARM

    TEXTAREA

#define SVC_(name, argcount) STUB_U name

#include <sysfuncs_nt60.h>

    END

#else

.code

#define SVC_(name, argcount) STUB_U name, argcount

#include <sysfuncs_nt60.h>

END
#endif
