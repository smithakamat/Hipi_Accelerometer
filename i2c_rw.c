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
       //printf("The test_buff is %s\n", test_buff);
	fp = popen(test_buff, "r");
	if(fp == NULL)
		printf("Unable to open the pipe\n");
	else
	//	printf("Successfull in opening the pipe\n");
	out=fread(buff,sizeof(char),sizeof(buff)-1,fp);
	//printf("The value in buffer is %d\n", out);
	fclose(fp);
	val =  atoi(buff);
	//printf("The number of bytes read is %d and the value is %d\n", out, val);
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
       // printf("TEST BUFF is %s\n",test_buff);	
	fp=popen(test_buff,"r");
	if(fp==NULL)
		printf("Unable to open the pipe\n");
        else
		//printf("Pipe Opened\n");
	
	out=fread(buff,sizeof(char),sizeof(buff)-1,fp);
	fclose(fp);
	temp_val=atoi(buff);
	//printf("Val is %d\n",temp_val);
	return;
   
}

/*int main()
{
	unsigned char temp=0;

	temp=read_reg(0x1D, 0x0D);
	
	printf("The value read is %d\n",temp);
	
        temp=read_reg(0x1D,0x2A);
	printf("CTRL_REG %d \n",temp);
	temp=temp & 0x7E;
	write_reg(0x1D,0x2A,temp);
	//printf("temp %d\n",temp);
}
*/


