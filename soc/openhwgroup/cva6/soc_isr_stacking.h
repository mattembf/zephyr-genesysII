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

#ifdef CONFIG_SOC_SERIES_PROVIDE_HW_CONTEXT_STACK

	#define SOC_ISR_SW_STACKING			\
		addi sp, sp, -ESF_SW_SIZEOF;	

#else /*CONFIG_SOC_SERIES_PROVIDE_HW_CONTEXT_STACK*/

	#define SOC_ISR_SW_STACKING			                           \
		/* Save caller-saved registers on current thread stack. */ \
		addi sp, sp, -__struct_arch_esf_SIZEOF;					   \
		DO_CALLER_SAVED(sr);


#endif /*CONFIG_SOC_SERIES_PROVIDE_HW_CONTEXT_STACK*/

#ifdef CONFIG_SOC_SERIES_PROVIDE_HW_CONTEXT_UNSTACK

#define SOC_ISR_HW_UNSTACKING			\
	addi t1, sp, ESF_SW_SIZEOF;			\
	csrw 0x7C9, t1;		                \
	li t0, (0x1 << 26);                 \
	csrs 0x7C7, t0;

#define SOC_ISR_SW_UNSTACKING	\
	addi sp,sp, ESF_SW_SIZEOF;	    

#else /*CONFIG_SOC_SERIES_PROVIDE_HW_CONTEXT_UNSTACK*/
	#define SOC_ISR_HW_UNSTACKING
	/* Restore caller-saved registers from thread stack */
	#define SOC_ISR_SW_UNSTACKING			    \
		DO_CALLER_SAVED(lr);                    \
		/* remove esf from the stack */         \
		addi sp, sp, __struct_arch_esf_SIZEOF;
		
#endif /*CONFIG_SOC_SERIES_PROVIDE_HW_CONTEXT_UNSTACK*/



#endif /* _ASMLANGUAGE */

#endif /* SOC_OPENHWGROUP_CVA6_SOC_ISR_STACKING_H_ */
