#ifndef _ACC_H_
#define _ACC_H_

#include "i2c_rw.h"

#define acc_slvAddr 0x1D
#define acc_devId 0x0D
#define GSCALE 2

#define A_CTRL_REG1 0x2A
#define XYZ_DATA_CFG 0x0E

#define OUT_X_H 0x01
#define OUT_X_L 0x02
#define OUT_Y_H 0x03
#define OUT_Y_L 0x04
#define OUT_Z_H 0x05
#define OUT_Z_L 0x06

void initAcc(void);
void readAcc_XYZ(void);

#endif //_ACC_H_
