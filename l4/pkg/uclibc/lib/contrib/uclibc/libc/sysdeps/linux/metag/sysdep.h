/*
 * Assembler macros for Meta.
 *
 * Licensed under LGPL v2.1 or later, see the file COPYING.LIB in this tarball.
 */

#include <common/sysdep.h>

#include <features.h>
#include <libc-internal.h>

#ifdef	__ASSEMBLER__

#ifdef SHARED
#define PLTJMP(_x)	_x##@PLT
#else
#define PLTJMP(_x)	_x
#endif

#undef	PSEUDO_END
#define	PSEUDO_END(name)						\
  SYSCALL_ERROR_HANDLER							\
  END (name)

#if defined NOT_IN_libc
# define SYSCALL_ERROR __local_syscall_error
# ifdef RTLD_PRIVATE_ERRNO
#  define SYSCALL_ERROR_HANDLER					\
__local_syscall_error:						\
       NEG	D0Re0, D0Re0;					\
       ADDT	D1Re0, CPC1, #HI(_rtld_errno);			\
       ADD	D1Re0, D1Re0, #LO(_rtld_errno) + 4;		\
       SETD	[D1Re0], D0Re0;					\
       NEG	D0Re0, #0x1;					\
       MOV	PC, D1RtP;
# else
#  define SYSCALL_ERROR_HANDLER					\
__local_syscall_error:						\
	MOV	D1Re0, D1RtP;					\
	SETL	[A0StP++], D0Re0, D1Re0;			\
	CALLR	D1RtP, PLTJMP(___errno_location);		\
	GETD	D1Re0, [A0StP+#-8];				\
	NEG	D1Re0, D1Re0;					\
	SETD	[D0Re0], D1Re0;					\
	NEG	D0Re0, #0x1;					\
	GETD	D1RtP, [A0StP+#-4];				\
	SUB	A0StP, A0StP, #0x8;				\
	MOV	PC, D1RtP;
# endif
#else
# define SYSCALL_ERROR_HANDLER	/* Nothing here; code in sysdep.S is used.  */
# define SYSCALL_ERROR ___syscall_error
#endif

#endif /* __ASSEMBLER __*/

/* Pointer mangling is not yet supported for META.  */
#define PTR_MANGLE(var) (void) (var)
#define PTR_DEMANGLE(var) (void) (var)
