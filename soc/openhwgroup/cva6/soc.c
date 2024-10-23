#include <soc.h>

#define LOG_LEVEL CONFIG_SOC_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(soc);

#ifdef CONFIG_RISCV_SOC_CONTEXT_SAVE

void __soc_save_context(soc_esf_t *state){
    
    unsigned long clear_mask = (0xFFFFul << 16);
    clear_mask = csr_read_clear(0x7C3, clear_mask); //Set enable to Supervisor counter = 0
    unsigned long old_enable = (clear_mask & 0xFFFF0000) >> 16;
    LOG_INF("Saving thread context from custom function\n");
    csr_write(0x7C3, old_enable ); //Set sel to read the register previously  enabled
    state->cyc_cnt_1 = csr_read(0x7C4); //read and store it in thread esf

}
	  
void __soc_restore_context(soc_esf_t *state){
    //Select CYC_1
    unsigned long status = csr_read(0x7C3);
    status = (status & (0xFFFF << 16)) | (0x1);
    csr_write(0x7C3, status);
    //Write saved Cyc_1
    csr_write(0x7C4, state->cyc_cnt_1);
    //Enable Cyc_1 advancement
    status = (0x1 << 16);
    LOG_INF("Restoring thread context from custom function\n");
    csr_write(0x7C3, status);
    return;
}

#endif /* CONFIG_RISCV_SOC_CONTEXT_SAVE */