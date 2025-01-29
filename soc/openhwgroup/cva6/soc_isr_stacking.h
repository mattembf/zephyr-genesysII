/*
 * Copyright (C) 2024 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SOC_OPENHWGROUP_CVA6_SOC_ISR_STACKING_H_
#define SOC_OPENHWGROUP_CVA6_SOC_ISR_STACKING_H_

#include <zephyr/arch/riscv/irq.h>

#if !defined(_ASMLANGUAGE)

#include <zephyr/devicetree.h>

//TODO: To conditionally set maybe unused fields (soc_context, csf, )
#define SOC_ISR_STACKING_ESF_DECLARE                                                       \
	struct arch_esf {                                                                      \
		unsigned long s0;                                                                  \
		unsigned long mstatus;                                                             \
		unsigned long sp;                                                                  \
		unsigned long mepc;                                                                \
        _callee_saved_t *csf;                                                              \
		unsigned long padding;															   \
		unsigned long ra;                                                                  \
		unsigned long t0;                                                                  \
		unsigned long t1;                                                                  \
		unsigned long t2;                                                                  \
		unsigned long a0;																   \
		unsigned long a1;                                                                  \
		unsigned long a2;                                                                  \
		unsigned long a3;                                                                  \
		unsigned long a4;                                                                  \
		unsigned long a5;                                                                  \
		unsigned long a6;                                                                  \
		unsigned long a7;                                                                  \
		unsigned long t3;                                                                  \
		unsigned long t4;                                                                  \
		unsigned long t5;                                                                  \
		unsigned long t6;                                                                  \
	} __aligned(16);


#else /* _ASMLANGUAGE */

/*
 * Size of the HW managed part of the ESF:
 *    sizeof unsigned long * 16 saved registers
 */
#define ESF_HW_SIZEOF	  (__SIZEOF_LONG__ * 16)

/*
 * Size of the SW managed part of the ESF in case of exception
 */
#define ESF_SW_SIZEOF (__struct_arch_esf_SIZEOF - ESF_HW_SIZEOF)

/*
 * VPR needs aligned(8) SP when doing HW stacking, if this condition is not fulfilled it will move
 * SP by additional 4 bytes when HW stacking is done. This will be indicated by LSB bit in stacked
 * MEPC. This bit needs to be saved and then restored because zephyr is managing MEPC and doesn't
 * know anything about this additional offset.
 */
#define MEPC_SP_ALIGN_BIT_MASK (0x1UL)

#define STORE_SP_ALIGN_BIT_FROM_MEPC				\
	addi t1, sp, __struct_arch_esf_soc_context_OFFSET;	\
	lr t0, __struct_arch_esf_mepc_OFFSET(sp);		\
	andi t0, t0, MEPC_SP_ALIGN_BIT_MASK;			\
	sr t0, __soc_esf_t_sp_align_OFFSET(t1)

#define RESTORE_SP_ALIGN_BIT_TO_MEPC				\
	addi t1, sp, __struct_arch_esf_soc_context_OFFSET;	\
	lr t0, __soc_esf_t_sp_align_OFFSET(t1);			\
	lr t1, __struct_arch_esf_mepc_OFFSET(sp);		\
	or t2, t1, t0;						\
	sr t2, __struct_arch_esf_mepc_OFFSET(sp)

#define SOC_ISR_SW_STACKING			\
	addi sp, sp, -ESF_SW_SIZEOF;	


#define SOC_ISR_SW_UNSTACKING			\
	addi sp, sp, ESF_SW_SIZEOF;			\
	csrw 0x7C9, sp;		                \
	li t0, (0x1 << 26);                 \
	csrs 0x7C7, t0;                        

#endif /* _ASMLANGUAGE */

#endif /* SOC_OPENHWGROUP_CVA6_SOC_ISR_STACKING_H_ */
