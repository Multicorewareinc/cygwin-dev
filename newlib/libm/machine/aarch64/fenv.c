/*-
 * Copyright (c) 2004 David Schultz <das@FreeBSD.ORG>
 * Copyright (c) 2013 Andrew Turner <andrew@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#define	__fenv_static
#include <fenv.h>
#include <machine/fenv-fp.h>

/*
 * Hopefully the system ID byte is immutable, so it's valid to use
 * this as a default environment.
 */


#ifdef __GNUC_GNU_INLINE__
#error "This file must be compiled with C99 'inline' semantics"
#endif

extern inline int feclearexcept(int __excepts);
extern inline int fegetexceptflag(fexcept_t *__flagp, int __excepts);
extern inline int fesetexceptflag(const fexcept_t *__flagp, int __excepts);
extern inline int feraiseexcept(int __excepts);
extern inline int fetestexcept(int __excepts);
extern inline int fegetround(void);
extern inline int fesetround(int __round);
extern inline int fegetenv(fenv_t *__envp);
extern inline int feholdexcept(fenv_t *__envp);
extern inline int fesetenv(const fenv_t *__envp);
extern inline int feupdateenv(const fenv_t *__envp);
extern inline int feenableexcept(int __mask);
extern inline int fedisableexcept(int __mask);
extern inline int fegetexcept(void);

/* These are writable so we can initialise them at startup.  */
static fenv_t fe_nomask_env;

/* These pointers provide the outside world with read-only access to them.  */
const fenv_t *_fe_nomask_env = &fe_nomask_env;

/*  Mask and shift amount for precision bits.  */
#define FE_CW_PREC_MASK		(0x0300)
#define FE_CW_PREC_SHIFT	(8)

/*  Returns the currently selected precision, represented by one of the
   values of the defined precision macros.  */
int
fegetprec (void)
{
  unsigned short cw;

  /* Get control word.  */
  // TODO
  //__asm__ volatile ("fnstcw %0" : "=m" (cw) : );

  return (cw & FE_CW_PREC_MASK) >> FE_CW_PREC_SHIFT;
}

/* http://www.open-std.org/jtc1/sc22//WG14/www/docs/n752.htm:

   The fesetprec function establishes the precision represented by its
   argument prec.  If the argument does not match a precision macro, the
   precision is not changed.

   The fesetprec function returns a nonzero value if and only if the
   argument matches a precision macro (that is, if and only if the requested
   precision can be established). */
int
fesetprec (int prec)
{
  unsigned short cw;

  /* Will succeed for any valid value of the input parameter.  */
  switch (prec)
    {
    case FE_FLTPREC:
    case FE_DBLPREC:
    case FE_LDBLPREC:
      break;
    default:
      return 0;
    }

  /* Get control word.  */
  // TODO
  //__asm__ volatile ("fnstcw %0" : "=m" (cw) : );

  /* Twiddle bits.  */
  cw &= ~FE_CW_PREC_MASK;
  cw |= (prec << FE_CW_PREC_SHIFT);

  /* Set back into FPU state.  */
  // TODO
  //__asm__ volatile ("fldcw %0" :: "m" (cw));

  /* Indicate success.  */
  return 1;
}
