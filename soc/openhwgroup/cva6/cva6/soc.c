
#include <zephyr/irq.h>
#include "soc.h"

#if defined(CONFIG_RISCV_SOC_INTERRUPT_INIT)
void soc_interrupt_init(void)
{
	(void)arch_irq_lock();
    csr_write(mie, 0);
#ifdef CONFIG_SOC_SERIES_PROVIDE_HW_CONTEXT_STACK
	custom_csr_set(CSR_SHADOW_STATUS, (0x1ULL << 28));
#endif /*CONFIG_SOC_SERIES_PROVIDE_HW_CONTEXT_STACK*/
}
#endif
