/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "I2C_Interface.h"

#define LIS3DH_DEVICE_ADDRESS 0x18

#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

#define LIS3DH_STATUS_REG 0x27

#define LIS3DH_CTRL_REG1 0x20

#define LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1 0X07 //INACTIVE LPEN

#define LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4 0x08 //ACTIVE HR

    
#define STARTUP 0x0000

int main(void)
{
    uint8 sampling_freq; //VALUTA SE FARE QUESTA DEF IN GLOBALE
    char message[50] = {'\0'};

    CyGlobalIntEnable; 
    EEPROM_Start();
    UART_Start();
    EEPROM_UpdateTemperature();
    sampling_freq = EEPROM_ReadByte(STARTUP); //SE DEVO RIFARE PIù VOLTE UPDATETEMP E READ METTO IN UNA FUNZ
    
    
    /* Read WHO AM I REGISTER register */
    uint8_t who_am_i_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_WHO_AM_I_REG_ADDR, 
                                                  &who_am_i_reg);
    if (error == NO_ERROR)
    {
        sprintf(message, "WHO AM I REG: 0x%02X [Expected: 0x33]\r\n", who_am_i_reg);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm\r\n");   
    }
    
     /******************************************/
    /*            I2C Writing REGISTER 1       */
    /******************************************/
    
    uint8_t ctrl_reg1;    
    
    if (ctrl_reg1 != LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1 )
    {
        ctrl_reg1 = LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1;
            
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    /******************************************/
    /*            I2C Writing REGISTER 4      */
    /******************************************/
    
    uint8_t ctrl_reg4;    
    
    if (ctrl_reg4 != LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4 )
    {
        ctrl_reg4 = LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4;
            
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg4);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    
    /******************************************/
    /*            I2C Writing REGISTER 1 for ODR */
    /******************************************/
        uint8_t ctrl_reg1_ODR;
        ctrl_reg1_ODR = sampling_freq<<4;
            
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1_ODR);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    
    
    
    for(;;)
    {
        /* controllo che il pulsante sia premuto e incremento la sampling frequency come valore ODR 1-6)*/
            if(PushButton_Read()){
                sampling_freq++;
                if (sampling_freq == 6) sampling_freq = 1;
                while (PushButton_Read());
            }        
        
    /******************************************/
    /*   setting ODR                        */
    /******************************************/
        
        uint8_t ctrl_reg1_ODR;
        ctrl_reg1_ODR = sampling_freq<<4;
            
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1_ODR);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
}

/* [] END OF FILE */
