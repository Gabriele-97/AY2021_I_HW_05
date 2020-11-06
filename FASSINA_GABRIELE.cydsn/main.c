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

#define STARTUP 0x0000

int main(void)
{
    uint8 sampling_freq;
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    EEPROM_Start();
    sampling_freq = EEPROM_ReadByte(STARTUP);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
