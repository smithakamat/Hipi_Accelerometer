#ifndef _I2C_RW_H_
#define _I2C_RW_H_

unsigned char read_reg(unsigned char slv_addr, unsigned char reg_addr);
void write_reg(unsigned char slv_addr, unsigned char reg_addr, unsigned char val);

#endif //_I2C_RW_H_
