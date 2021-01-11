

/*-----------------------------------------
by Kang Woo Jung

2015.11.25

-------------------------------------------*/

#include "MLX90614.h"
#include "main.h"




u16 MLX90614_Read(u8 Command)
{
    u8 Lo, Hi;
    u16 Ret;

 
    I2C_GenerateSTART(I2C1, ENABLE);    //Start condition transmitted
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1,  0x00, I2C_Direction_Transmitter); //Transmitter Mode
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, Command); // Command : Read TObj1 (0x07) from RAM
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 

            
    I2C_GenerateSTART(I2C1, ENABLE); //Start condition transmitted
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, 0x00, I2C_Direction_Receiver); // Receiver Mode
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));


    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    Lo = I2C_ReceiveData(I2C1); // Data received

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    Hi = I2C_ReceiveData(I2C1); // Data received

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    I2C_GetPEC(I2C1);


    I2C_GenerateSTOP(I2C1, ENABLE);    //Stop condition transmitted

    Ret = Hi<<8;
    Ret |= Lo;
    return (Ret);
}


void MLX90614_Write(u8 Command, u16 Data)
{

    /* Enable I2C1 PEC Transmission */
    I2C_CalculatePEC(I2C1, ENABLE);

    I2C_GenerateSTART(I2C1, ENABLE);    //Start condition transmitted
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1,  0x00, I2C_Direction_Transmitter); //Transmitter Mode
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, Command); // Command : Read TObj1 (0x07) from RAM
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 

    I2C_SendData(I2C1, Data & 0xff);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 

    I2C_SendData(I2C1, Data >> 8);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 

    I2C_TransmitPEC(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 

    I2C_GenerateSTOP(I2C1, ENABLE);    //Stop condition transmitted
}

