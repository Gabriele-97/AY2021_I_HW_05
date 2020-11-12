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
#include "Routines.h"
char message[50];

void check_for_error(ErrorCode error, uint8_t register_value){
    
    
    if (error == NO_ERROR)
    {
        sprintf(message, " REG: 0x%02X \r\n", register_value);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm\r\n");   
    }

}


void updatefreq(){
        uint8_t ODR;
        if (PushButton_Read() == PUSH_BUTTON_PRESSED){
        while (PushButton_Read() == PUSH_BUTTON_PRESSED);
        sampling_freq ++;
                if (sampling_freq > 0x06) sampling_freq = 0x01;
                if (sampling_freq <0x01) sampling_freq =0x01;
        
                EEPROM_WriteByte(sampling_freq,STARTUP); //metto la nuova frequenza nella EEprom 
                ODR = sampling_freq << 4 ;
                ODR |= LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1;
                
        ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ODR);
        if (error == NO_ERROR){        
        switch (sampling_freq){
                    case 1: sprintf(message, " sampling frequency set at %d Hz", 1);
                            break;
                    case 2: sprintf(message, " sampling frequency set at %d Hz", 10);
                            break;
                    case 3: sprintf(message, " sampling frequency set at %d Hz", 25);
                            break;
                    case 4: sprintf(message, " sampling frequency set at %d Hz", 50);
                            break;
                    case 5: sprintf(message, " sampling frequency set at %d Hz", 100);
                            break;
                    case 6: sprintf(message, " sampling frequency set at %d Hz", 200);
                            break;
                    default:
                        break;  
                }    
        UART_PutString(message);
        }
        else UART_PutString("Error occurred during I2C comm\r\n");   
}
}

int16 read_and_convert( uint8_t reg_low, uint8_t reg_high){
    uint8_t acc[2];
    int16 outacc;
    float outacc_conv;
    int16 outacc_tbt;
           
    I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, reg_low, &acc[0]);
    I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, reg_high, &acc[1]);
    outacc = (int16)((acc[0] | (acc[1]<<8)))>>4;
    if(outacc > 2048) outacc = 2048; 
    if(outacc < -2048) outacc = -2048; 
    outacc_conv = (float) (K * outacc * G);
    outacc_tbt = (int16) (TRICK * outacc_conv);
    
   return outacc_tbt;

}

void transmit(int16 accx, int16 accy, int16 accz){
        OutArray[1] =  (uint8_t) (accx & 0xFF);
        OutArray[2] =  (uint8_t) ((accx >> 8)&0xFF);
        OutArray[3] =  (uint8_t) (accy & 0xFF);
        OutArray[4] =  (uint8_t) ((accy >> 8)&0xFF);
        OutArray[5] =  (uint8_t) (accz & 0xFF);
        OutArray[6] =  (uint8_t) ((accz >> 8)&0xFF);
        
        UART_PutArray(OutArray,BYTE_TO_TRANSMIT);
}

void high_resolution_set(){
    ctrl_reg4 = LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4;
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG4,
                                             ctrl_reg4);
        check_for_error(error, ctrl_reg4);
    
        
        ctrl_reg1_ODR = (sampling_freq << 4) | LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1;
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1_ODR);
        check_for_error(error, ctrl_reg1_ODR);
    
}

/* [] END OF FILE */
/* [] END OF FILE */
