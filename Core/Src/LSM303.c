/*
 * LSM303.c
 *
 *  Created on: Jul 1, 2023
 *      Author: Oğuz Kaan
 */

#include "LSM303.h"


extern I2C_HandleTypeDef hi2c1;
float x,y,z;
int Shift;
float multipler;
void LSM303_Init(Resolution resolutionBits , Scale G_scale)
{
	// High res mode control reg1 a[3]= 0 and ctrl_reg4 a[4] =1

	/* 0x97 control reg 1
	 * z,x,y enable and hr normal low power mode
	 * 10010111
	 * 0x97
	 * 0x3F
	 * 0x57; // (0b01010111)
	 * */
	uint8_t wdata[1] = {0};

	switch(resolutionBits){
		case HighResolution: Shift = 4;

		if(G_scale == Scale_16g) multipler = 0.01172;
			else if (G_scale == Scale_8g) multipler = 0.0039;
			else if (G_scale == Scale_4g) multipler = 0.00195 ;
			else multipler =  0.00098;

			wdata[0] = 0x57;
			HAL_I2C_Mem_Write(&hi2c1, LSM303_Write_Address_Ac, CTRL_REG1_A, 1, wdata, 1, 1000);
			HAL_Delay(10);

			wdata[0] = 0x08 | (G_scale <<4);
			HAL_I2C_Mem_Write(&hi2c1, LSM303_Write_Address_Ac, CTRL_REG4_A, 1, wdata, 1, 1000);
			HAL_Delay(10);

		break;

		case NormalResolution: Shift = 2;

		if(G_scale == Scale_16g) multipler =0.0469;
			else if (G_scale == Scale_8g) multipler = 0.01563;
			else if (G_scale == Scale_4g) multipler = 0.00782 ;
			else multipler =   0.0039;

			wdata[0] = 0x57;
			HAL_I2C_Mem_Write(&hi2c1, LSM303_Write_Address_Ac, CTRL_REG1_A, 1, wdata, 1, 1000);
			HAL_Delay(10);

			wdata[0] =  G_scale << 4;
			HAL_I2C_Mem_Write(&hi2c1, LSM303_Write_Address_Ac, CTRL_REG4_A, 1, wdata, 1, 1000);
			HAL_Delay(10);
		break;

		case LowResolution: Shift =0;

		if(G_scale == Scale_16g) multipler =0.18758;
				else if (G_scale == Scale_8g) multipler = 0.06252;
				else if (G_scale == Scale_4g) multipler = 0.03126 ;
				else multipler = 0.01563;

			wdata[0] = 0x5F;
			HAL_I2C_Mem_Write(&hi2c1, LSM303_Write_Address_Ac, CTRL_REG1_A, 1, wdata, 1, 1000);
			HAL_Delay(10);

			wdata[0] =  G_scale << 4;
			HAL_I2C_Mem_Write(&hi2c1, LSM303_Write_Address_Ac, CTRL_REG4_A, 1, wdata, 1, 1000);
			HAL_Delay(10);
		break;

		}

	/*
	 * ctrl reg 4 a
	 *
	 * 00111000
	 * 0x38
	 * */

	/* CFG_REG_A_M
	 *
	 * */


}
// status rega okunması lazım

void LSM303_Read_Accel()
{


	//uint8_t rData[1] = {0};
	uint8_t acLData[1]= {0};
	uint8_t acHData[1]= {0};

	//HAL_I2C_Mem_Read(&hi2c1, LSM303_Read_Address_Ac, STATUS_REG_A, 1, rData, 1, 1000);

	//if(rData[0] & 0x3)
	//{

		HAL_I2C_Mem_Read(&hi2c1, LSM303_Read_Address_Ac, OUT_Z_L_A, 1, acLData, 1, 1000);
		HAL_Delay(20);
		HAL_I2C_Mem_Read(&hi2c1, LSM303_Read_Address_Ac, OUT_Z_H_A, 1, acHData, 1, 1000);

		// 4 bit kaydırdık çünkü high resolution da 12 bit data var
		z = ((int16_t)((acHData[0] << 8) | acLData[0]) ) >> Shift;
		z= (float)z*multipler;



		HAL_I2C_Mem_Read(&hi2c1, LSM303_Read_Address_Ac, OUT_X_L_A, 1, acLData, 1, 1000);
		HAL_Delay(20);
		HAL_I2C_Mem_Read(&hi2c1, LSM303_Read_Address_Ac, OUT_X_H_A, 1, acHData, 1, 1000);


		x =  ((int16_t)((acHData[0] << 8) | acLData[0]) ) >> Shift;

		x= (float)x*multipler;


		HAL_I2C_Mem_Read(&hi2c1, LSM303_Read_Address_Ac, OUT_Y_L_A, 1, acLData, 1, 1000);
		HAL_Delay(20);
		HAL_I2C_Mem_Read(&hi2c1, LSM303_Read_Address_Ac, OUT_Y_H_A, 1, acHData, 1, 1000);


		y =  ((int16_t)((acHData[0] << 8) | acLData[0]) ) >> Shift;
		y= (float)y*multipler;


	//}


}
