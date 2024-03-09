unsigned char Ad_Read(unsigned char addr);
void I2CSendByte(unsigned char byt);
void I2CStart(void);
void I2CStop(void);
unsigned char I2CReceiveByte(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(unsigned char ackbit);
void Da_Write(unsigned char dat);