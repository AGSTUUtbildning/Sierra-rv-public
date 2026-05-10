#ifndef __ALTERA_AVALON_SIERRA_TCB_OFFSET_COMPAT_H__
#define __ALTERA_AVALON_SIERRA_TCB_OFFSET_COMPAT_H__

/*
 * NiosII-compatible offsets kept for source compatibility with the global
 * Sierra IP. The Nios V/g port uses the C tcb_t layout directly.
 */
#define TCB_TASK_ID            0
#define TCB_STARTADR           4
#define TCB_REG_AT             8
#define TCB_REG_RETURNVALUES  12
#define TCB_REG_ARG           20
#define TCB_REG_GPR           36
#define TCB_REG_ET           100
#define TCB_REG_GP           104
#define TCB_REG_FP           108
#define TCB_REG_EA           112
#define TCB_REG_RA           116
#define TCB_STACKPTR         120
#define TCB_STACKAREA        124
#define TCB_STACKSIZE        128

#endif /* __ALTERA_AVALON_SIERRA_TCB_OFFSET_COMPAT_H__ */
