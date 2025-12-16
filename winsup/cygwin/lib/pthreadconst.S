#if defined(__i386__)
#  define SYM(x) _##x
#else
#  define SYM(x) x
#endif

        .section .rdata,"dr"
        .p2align 3

        .globl  SYM(__pthread_recursive_mutex_initializer_np)
SYM(__pthread_recursive_mutex_initializer_np):
        .quad   18

        .globl  SYM(__pthread_normal_mutex_initializer_np)
SYM(__pthread_normal_mutex_initializer_np):
        .quad   19

        .globl  SYM(__pthread_errorcheck_mutex_initializer_np)
SYM(__pthread_errorcheck_mutex_initializer_np):
        .quad   20

        .globl  SYM(__pthread_cond_initializer)
SYM(__pthread_cond_initializer):
        .quad   21

        .globl  SYM(__pthread_rwlock_initializer)
SYM(__pthread_rwlock_initializer):
        .quad   22
