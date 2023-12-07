#ifndef _SYSTEM_SERIAL_H
#define _SYSTEM_SERIAL_H

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "System/base.h"
#include "System/Data/ring.h"
#include "System/hash.h"
#include "System/Serial/adapter.h"
#include "System/Serial/i2c.h"
#include "System/Serial/spi.h"
#include "System/Serial/uart.h"

#define SERIAL_TX_STREAM_SIZE 256
#define SERIAL_TX_MAX_LENGTH 128

#define SERIAL_RX_STREAM_SIZE 256
#define SERIAL_RX_MAX_LENGTH 128

class SerialDevice : public SerialAdapter
{
public:
	~SerialDevice(){}
	SerialDevice(UART_HandleTypeDef* uart);
	SerialDevice(SPI_HandleTypeDef* spi);
	SerialDevice(I2C_HandleTypeDef* i2c, uint8_t peripheralAddress);

	void 			Open();
	void 			Open(size_t rxsize, size_t txsize);
	void 			Open(void* rxbuffer, size_t rxsize, void* txbuffer, size_t txsize);

	uint8_t		 		Available();

	void    		Write(const void* data, size_t length);
	uint8_t		 		Read();
	uint8_t		 		ReadedByte();
	size_t  		Read(void *data, size_t length);

	virtual void 	Receive(void* data, size_t length, uint16_t remoteAddress = 0);
	virtual void 	Transmit(void* data, size_t length, uint16_t remoteAddress = 0);

	virtual	 uint32_t 	DeviceAddress();

	void 			ReceiptComplete(void);
	void 			TransmitComplete(void);

	void 			SetReceiptEvent(SerialEvent* event);
	void 			SetTransmitEvent(SerialEvent* event);

	SerialHandlerType HandlerType;

	int32_t				SerialCode;

private:

	Ring* 	receiptBuffer;
	Ring* 	transmitBuffer;

	SerialEvent* receiptEvent;
	SerialEvent* transmitEvent;

	uint32_t 		receiptLength = 0;
	uint32_t			transmitLength = 0;

	uint32_t 		lastReceiptPosition = 0;
	uint32_t 		maxReceiptLength;
	uint32_t 		receiptBufferSize;
	uint32_t 		maxTransmitLength;
	uint32_t 		transmitBufferSize;
};

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

class SerialDeviceCollection
{
public:
	SerialDeviceCollection()
	{
		for(int i = 0; i < 17; i++)
			Table[i] = NULL;
	}

	SerialDevice* 		Table[17];
	uint8_t			    	Count = 0;
	void			   	Add(SerialDevice *serialCom);
	SerialDevice*		Get(uint32_t key);
	uint8_t			Remove(uint32_t key);
};

extern SerialDeviceCollection *SerialDevices;

#endif /* _SYSTEM_SERIAL_H */
