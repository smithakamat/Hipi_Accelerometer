/*Description: The following is the driver code that is used to implement the I2C read and write 
from the gyroscope and accelerometer registers. The HiPi modules have been used to access kernel
drivers for I2C devices

Authors: Smitha Sunil Kamat, Jay Khandhar 
*/

/*Preprocessor directives*/
#include <stdio.h>
#include <stdlib.h>

/*This function is used to read a 1 byte value from the register whose address is passed as an argument*/
/*slv_addr is the address of the slave that is connected to the I2C bus 1 of the raspberry pi*/
unsigned char  read_reg(unsigned char slv_addr,unsigned char reg_addr)
{
	FILE *fp;
	char buff[4];
	unsigned char out, val;
	char test_buff[20];
	sprintf(test_buff, "hipi-i2c r 1 0x%02X 0x%02X 1", slv_addr, reg_addr);

	fp = popen(test_buff, "r");
	if(fp == NULL)
		printf("Unable to open the pipe\n");
	else
	out=fread(buff,sizeof(char),sizeof(buff)-1,fp);
	fclose(fp);
	val =  atoi(buff);
	return val;
}

/*This function is used to write a one byte value into the register whose value is passed as an argument*/
/*slv_addr is the address of the slave that is connected to the I2C bus 1 of the raspberry pi*/
void write_reg(unsigned char slv_addr,unsigned char reg_addr, unsigned char val)
{
        FILE *fp;
	char buff[4];
        unsigned char out, temp_val;
	char test_buff[20];
	sprintf(test_buff, "hipi-i2c w 1 0x%02X 0x%02X 0x%02X ",slv_addr,reg_addr,val);
	fp=popen(test_buff,"r");
	if(fp==NULL)
		printf("Unable to open the pipe\n");
        else
		//printf("Pipe Opened\n");
	
	out=fread(buff,sizeof(char),sizeof(buff)-1,fp);
	fclose(fp);
	temp_val=atoi(buff);
	return;
   
}



