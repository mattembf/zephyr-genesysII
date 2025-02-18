#define CSR_CNT_STATUS      0x7C3
#define CSR_CNT_DATA        0x7C4
#define CSR_CNT_DATA_H      0x7C5
#define CSR_LAST_SP         0x7C6
#define CSR_SHADOW_STATUS   0x7C7
#define CSR_SHADOW_REG      0x7C8
#define CSR_LOAD_ESF        0x7C9

#define custom_csr_set(reg,var)   csr_set(reg,var) 
#define custom_csr_read(reg)      csr_read(reg) 
#define custom_csr_write(reg,var) csr_write(reg,var)