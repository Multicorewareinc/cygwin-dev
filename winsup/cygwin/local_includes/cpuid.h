/* cpuid.h: Define cpuid instruction

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#ifndef CPUID_H
#define CPUID_H

static inline void __attribute ((always_inline))
cpuid (uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, uint32_t ain,
       uint32_t cin = 0)
{
#if defined(__x86_64__)
  asm volatile ("cpuid"
    : "=a" (*a), "=b" (*b), "=c" (*c), "=d" (*d)
    : "a" (ain), "c" (cin));

#elif defined(__aarch64__)
  *a = *b = *c = *d = 0;
  switch (ain)
    {
    case 0x00000000:
      *a = 0x00000001;
      *b = 0x72413341; /* pseudo vendor "A3Ar" */
      *d = 0x34366863; /* "ch64"               */
      *c = 0x6E695700; /* "Win\0"              */
      break;
    case 0x00000001:
      {
        uint32_t ecx = 0, edx = 0;
        if (IsProcessorFeaturePresent (PF_ARM_V8_CRYPTO_INSTRUCTIONS_AVAILABLE))
          ecx |= (1u << 25); /* repurposed: AES present  */
        if (IsProcessorFeaturePresent (PF_ARM_V8_CRC32_INSTRUCTIONS_AVAILABLE))
          ecx |= (1u << 20); /* repurposed: CRC32 present */
        if (IsProcessorFeaturePresent (PF_ARM_NEON_INSTRUCTIONS_AVAILABLE))
          edx |= (1u << 23); /* repurposed: NEON present  */
        if (IsProcessorFeaturePresent (PF_ARM_V8_INSTRUCTIONS_AVAILABLE))
          edx |= (1u << 0);  /* FP present                */
        *c = ecx;
        *d = edx;
      }
      break;
    case 0x00000007:
      if (cin == 0)
        {
          uint32_t ebx = 0;
          if (IsProcessorFeaturePresent (PF_ARM_V8_CRYPTO_INSTRUCTIONS_AVAILABLE))
            ebx |= (1u << 29);
          *b = ebx;
        }
		/* Sub-leaf cin > 0: return all-zeros (unsupported), which is
        already the case from the *a = *b = *c = *d = 0 at entry.  */
      break;
    case 0x80000000:
      *a = 0x80000001;
      break;
    default:
      break;
    }
#endif
}


#if defined(__x86_64__) || defined(__aarch64__)
static inline bool __attribute ((always_inline))
can_set_flag (uint32_t long flag)
{
#if defined(__x86_64__)
  uint32_t long r1, r2;
  asm volatile ("pushfq\n"
		"popq %0\n"
		"movq %0, %1\n"
		"xorq %2, %0\n"
		"pushq %0\n"
		"popfq\n"
		"pushfq\n"
		"popq %0\n"
		"pushq %1\n"
		"popfq\n"
		: "=&r" (r1), "=&r" (r2)
		: "ir" (flag)
  );
  return ((r1 ^ r2) & flag) != 0;
#elif defined(__aarch64__)
  (void) flag;
  return true;
#endif
}
#else
#error unimplemented for this target
#endif

#endif // !CPUID_H
