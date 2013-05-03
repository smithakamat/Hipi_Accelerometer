/*Description : This is the header file for the gyro.c code.
The addresses of the control registers, output registers of the gyroscope 
sensors used in the gyroc file have been defined here, alng with the functions used

Authors : Smitha Sunil Kamat, Jay Khandhar
*/

#ifndef _GYRO_H
#define _GYRO_H

/*Slave address of the gyroscope*/
#define gyro_slvAddr 0x6B

/*************************************************************************************************
****Definition of the addresses of the of the hardware registers in L3GD20**********************
**************************************************************************************************/

/*Defining the address of the WHO_AM_I device identification register*/
#define gyro_WHO_AM_I 0x0F

/*Defining the address of control registers*/
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

/*Defining the address of output registers for yaw, pitch and roll*/
#define OUT_X_Lo 0x28
#define OUT_X_Hi 0x29
#define OUT_Y_Lo 0x2A
#define OUT_Y_Hi 0x2B
#define OUT_Z_Lo 0x2C
#define OUT_Z_Hi 0x2D

/*Defining the address of the status register*/
#define STATUS_REG 0x27

/*Defining the address of the fifo control register*/
#define FIFO_CTRL_REG 0x2E

/*Defining the address of the Temperature register in the gyroscope*/
#define OUT_TEMP 0x26

extern unsigned int  raw_x,raw_y,raw_z;
extern struct timespec gyro_time;
extern float xtotal, ytotal, ztotal;
#endif //_GYRO_H_
