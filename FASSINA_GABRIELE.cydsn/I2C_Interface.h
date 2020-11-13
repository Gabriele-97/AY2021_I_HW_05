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
/** 
 * \file I2C_Interface.h

*/

#ifndef I2C_Interface_H
    #define I2C_Interface_H
    
    #include "cytypes.h"
    #include "errorcode.h"
    #include "stdio.h"
    
    /** \brief Start the I2C peripheral.
    *   
    *   This function starts the I2C peripheral so that it is ready to work.
    */
    ErrorCode I2C_Peripheral_Start(void);
    
    /** \brief Stop the I2C peripheral.
    *   
    *   This function stops the I2C peripheral from working.
    */
    ErrorCode I2C_Peripheral_Stop(void);
    
    /**
    *   \brief Read one byte over I2C.
    *   
    *   This function performs a complete reading operation over I2C from a single
    *   register.
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the register to be read.
    *   \param data Pointer to a variable where the byte will be saved.
    */
    ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                            uint8_t register_address,
                                            uint8_t* data);
    
    
    /** 
    *   \brief Write a byte over I2C.
    *   
    *   This function performs a complete writing operation over I2C to a single 
    *   register.
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the register to be written.
    *   \param data Data to be written
    */
    ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t data);
    
    
    /**
    *   \brief Check if device is connected over I2C.
    *
    *   This function checks if a device is connected over the I2C lines.
    *   \param device_address I2C address of the device to be checked.
    *   \retval Returns true (>0) if device is connected.
    */
    uint8_t I2C_Peripheral_IsDeviceConnected(uint8_t device_address);
    
#endif // I2C_Interface_H
/* [] END OF FILE */

/* [] END OF FILE */
