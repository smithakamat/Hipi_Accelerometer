/*Description : This is the header file of the acc.c code. The addresses of the 
control registers, output registers of the accelerometer sensor used in the acc.c
file have been defined here, along with the functions used

Authors : Smitha Sunil Kamat, Jay Khandhar
*/

#ifndef _ACC_H_
#define _ACC_H_

#include "i2c_rw.h"

/*********************************************************************************
**********Addresses of the hardware registers of MMA8452Q accelerometer************
**********************************************************************************/

#define acc_slvAddr 0x1D /*I2C Slave address of the accelerometer*/
#define acc_devId 0x0D  /*Manufacturer ID of the accelerometer*/
#define GSCALE 2  /*Setting the g scale of the acceleromter to +- 2g*/

/*Addresses of the control registers and data configuration registers*/
#define A_CTRL_REG1 0x2A
#define XYZ_DATA_CFG 0x0E

/*Addresses of the output registers of the accelerometer*/
#define OUT_X_H 0x01
#define OUT_X_L 0x02
#define OUT_Y_H 0x03
#define OUT_Y_L 0x04
#define OUT_Z_H 0x05
#define OUT_Z_L 0x06

extern float AccG_X, AccG_Y, AccG_Z;
extern unsigned int conv_raw_xconcat,conv_raw_yconcat,conv_raw_zconcat;
void initAcc(void);
void readAcc_XYZ(void);

#endif //_ACC_H_
