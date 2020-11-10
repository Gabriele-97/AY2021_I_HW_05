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
#include "Routines.h"


#define PUSH_BUTTON_PRESSED 0

#define LIS3DH_DEVICE_ADDRESS 0x18

#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

#define LIS3DH_STATUS_REG 0x27

#define LIS3DH_CTRL_REG1 0x20

#define LIS3DH_CTRL_REG4 0x23

#define LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4 0x08 //ACTIVE HR

#define LIS3DH_XLOUT 0x28 //register of output x
    
#define STARTUP 0x00
    
#define STATUS_REG 0x27
    
#define M_CALIBRAZIONE 4/4096
#define Q_CALIBRAZIONE 2

int main(void)
{
    

    CyGlobalIntEnable; 
    I2C_Peripheral_Start();//start I2C communication protocol
    EEPROM_Start(); //start eeprom
    UART_Start(); //start UART
    
    CyDelay(5); //wait for the starting procedures completed
     
      
    uint8_t ctrl_reg1_ODR;
    uint8_t ctrl_reg4;
    
    uint8_t OutArray[8];
    OutArray[0] = 0xA0;
    OutArray[7] = 0xC0;
    
    //I read the value of the eeprom to set the ODR at the activation
    EEPROM_UpdateTemperature();
    sampling_freq = EEPROM_ReadByte(STARTUP); 
    
    
    /* Read WHO AM I REGISTER register */
    uint8_t who_am_i_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_WHO_AM_I_REG_ADDR, 
                                                  &who_am_i_reg);
    check_for_error(error, who_am_i_reg);
    
    //set HR mode   
      
        ctrl_reg4 = LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4;
            
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG4,
                                             ctrl_reg4);
        check_for_error(error, ctrl_reg4);
    
        ctrl_reg1_ODR = sampling_freq << 4;
        ctrl_reg1_ODR |= LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1;
            
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1_ODR);
    
        check_for_error(error, ctrl_reg1_ODR);
    
    
    for(;;)
    {   
        updatefreq(); //FA L'UPDATE DELLA FREQUENZA MA VALUTA SE SI PUò RISCRIVERE MEGLIO A VIDEO
        
        
        // leggo i 3 valori dell'accelerometro e li metto nel vettore che mi permette la comunicazione 
        CyDelay(10);
        
        int16 outaccx_tbt;
        int16 outaccy_tbt;
        int16 outaccz_tbt;
        uint8 sts_reg;
        uint8_t zda;
        uint8_t yda;
        uint8_t xda;
        uint8_t zyxda;
        
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, STATUS_REG, &sts_reg);
        zda = sts_reg & 0x04;
        yda = sts_reg & 0x02;
        xda = sts_reg & 0x01;
        zyxda = sts_reg & 0x08;
        
       
        outaccx_tbt = read_and_convert(xda, 0x28, 0x29);
        outaccy_tbt = read_and_convert(yda, 0x2A, 0x2B);
        outaccz_tbt = read_and_convert(zda, 0x2C, 0x2D);
               
        
        if(zyxda){
        OutArray[1] =  outaccx_tbt >> 8;
        OutArray[2] =  outaccx_tbt & 0xFF;
        OutArray[3] =  outaccy_tbt >> 8;
        OutArray[4] =  outaccy_tbt & 0xFF;
        OutArray[5] =  outaccz_tbt >>8;
        OutArray[6] =  outaccz_tbt &0xFF;
        
        UART_PutArray(OutArray,8);
        }            
              
    }
         
    
}


/* [] END OF FILE */
