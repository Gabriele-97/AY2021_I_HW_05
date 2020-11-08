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


#define PUSH_BUTTON_PRESSED 0

#define LIS3DH_DEVICE_ADDRESS 0x18

#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

#define LIS3DH_STATUS_REG 0x27

#define LIS3DH_CTRL_REG1 0x20

#define LIS3DH_CTRL_REG4 0x23

#define LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1 0X07 //INACTIVE LPEN

#define LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4 0x08 //ACTIVE HR

#define LIS3DH_XLOUT 0x28 //register of output x
    
#define STARTUP 0x00
    
#define STATUS_REG 0x27

int main(void)
{
    

    CyGlobalIntEnable; 
    I2C_Master_Start(); //controlla se va bene
    I2C_Peripheral_Start();//CONTROLLA POI SE è GIUSTO
    EEPROM_Start();
    UART_Start();
    
    CyDelay(5);
     
    uint8_t sampling_freq; //VALUTA SE FARE QUESTA DEF IN GLOBALE
    char message[50];
    
    uint8 flag_debug;
    uint8_t ctrl_reg1_ODR;
    
    uint8_t OutArray[4];
    OutArray[0] = 0xA0;
    OutArray[3] = 0xC0;
    
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
                                             LIS3DH_CTRL_REG4,
                                             ctrl_reg4);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 successfully written as: 0x%02X\r\n", ctrl_reg4);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    
    /******************************************/
    /*     I2C Writing REGISTER 1 for ODR    */
    /******************************************/
        ctrl_reg1_ODR = sampling_freq << 4;
        ctrl_reg1_ODR |= LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1;
            
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1_ODR);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1_ODR);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    
    
    
    for(;;)
    {   
        //read from button and update frequency
        if (PushButton_Read() == PUSH_BUTTON_PRESSED){
        while (PushButton_Read() == PUSH_BUTTON_PRESSED);
        sampling_freq ++;
                if (sampling_freq > 6) sampling_freq = 1;
                if (sampling_freq <1) sampling_freq =1;
        
                EEPROM_WriteByte(sampling_freq,STARTUP); //metto la nuova frequenza nella EEprom 
                ctrl_reg1_ODR = sampling_freq << 4 ;
                ctrl_reg1_ODR |= LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1;
                
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1_ODR);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1_ODR);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
        

        }
        
        // leggo i 3 valori dell'accelerometro e li metto nel vettore che mi permette la comunicazione 
        CyDelay(10);
        uint8_t acc[6];
        int16_t outaccx;
        int16_t outaccy;
        int16_t outaccz;
        uint8_t sts_reg;
        uint8_t zda;
        uint8_t yda;
        uint8_t xda;
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, STATUS_REG, &sts_reg);
        zda = sts_reg & 0x04;
        yda = sts_reg & 0x02;
        xda = sts_reg & 0x01;
        
        if(xda){        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 0x28, &acc[0]);
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 0x29, &acc[1]);
        outaccx = (int16_t)((acc[0] | (acc[1]<<8)>>4));
        }
        
        
        //fai qui elaborazioni dati 
        if(yda){
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 0x2A, &acc[2]);
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 0x2B, &acc[3]);
        outaccy = (int16_t)((acc[2] | (acc[3]<<8)>>4));
        }
        
        if(zda){
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 0x2C, &acc[4]);
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 0x2D, &acc[5]);
        outaccz = (int16_t)((acc[4] | (acc[5]<<8)>>4));
        }
        
        OutArray[1] = acc[0];
        OutArray[2] = acc[1];
        
        UART_PutArray(OutArray,4);
                                               
     
      
        
                    
              
    }
         
    
}


/* [] END OF FILE */
