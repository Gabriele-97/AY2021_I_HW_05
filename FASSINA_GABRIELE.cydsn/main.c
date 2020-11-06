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

#define LIS3DH_DEVICE_ADDRESS 0x18

#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

#define LIS3DH_STATUS_REG 0x27

#define LIS3DH_CTRL_REG1 0x20
    
#define STARTUP 0x0000

int main(void)
{
    uint8 sampling_freq; //VALUTA SE FARE QUESTA DEF IN GLOBALE
    
    CyGlobalIntEnable; 
    EEPROM_Start();
    I2C_Start();
    UART_Start();
    EEPROM_UpdateTemperature();
    sampling_freq = EEPROM_ReadByte(STARTUP); //SE DEVO RIFARE PIù VOLTE UPDATETEMP E READ METTO IN UNA FUNZ
    

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
