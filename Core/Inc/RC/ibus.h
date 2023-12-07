/*
 * ecliptar_rc_ibus.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef RC_IBUS_H
#define RC_IBUS_H

#include <stdio.h>
#include <inttypes.h>
#include <usart.h>
#include <stm32f4xx_hal.h>

#include "RC/base.h"
#include "RC/IBUS/sensors.h"
#include "Control/channel.h"
#include "system.h"

#define DEFAULT_MIN_PULSE_US    		600
#define DEFAULT_MID_PULSE_US    		1500
#define DEFAULT_MAX_PULSE_US        2400
#define DEFAULT_SIGNAL_LOST          0
#define PULSE_MIN_CHANGE_US       	1
#define PULSE_MAX_ACCEL_CUT        	0.5F
#define PROTOCOL_COMMAND40 			0x40        // Command to set servo or motor speed is always 0x40
#define PROTOCOL_COMMAND_DISCOVER 	0x80 // Command discover sensor (lowest 4 bits are sensor)
#define PROTOCOL_COMMAND_TYPE 		0x90     // Command discover sensor (lowest 4 bits are sensor)
#define PROTOCOL_COMMAND_VALUE 		0xA0    // Command send sensor data (lowest 4 bits are sensor)
#define SENSORS_MAX_LENGTH			10 // Max number of sensors
#define CHANNELS_MSG_LENGTH			32
#define SENSORS_MSG_LENGTH			4
#define REQUEST_BUFFER_SIZE			32
#define RESPONSE_BUFFER_SIZE			10



class RC_IBUS : public SerialEvent, public RemoteController // @suppress("Class has a virtual method and non-virtual destructor")
{
public:
	~RC_IBUS(){}
	RC_IBUS();

void            	Attach(uint8_t channelsCount, UART_HandleTypeDef* rc_uart);
void            	Attach(uint8_t channelsCount, UART_HandleTypeDef* rc_uart, uint16_t minPulseArray[], uint16_t midPulseArray[], uint16_t maxPulseArray[]);
void            	Attach(uint8_t channelsCount, UART_HandleTypeDef* rc_uart, uint16_t minPulseArray[], uint16_t midPulseArray[], uint16_t maxPulseArray[], int signalLostArray[]);
void            	Attach(uint8_t channelsCount, UART_HandleTypeDef* rc_uart, ControlChannel** channelArray);

virtual u8            	Attached();

virtual u8            	Available();

virtual ControlChannel**   	ReadChannels();

uint8_t         	readChannel(ControlChannel* channel);

uint8_t 			AddSensor(uint8_t type, uint8_t len);

void 				SetSensorMeasurement(uint8_t adr, int32_t value);

virtual void       PrintInfoReport(ControlChannel** channels);

uint8_t     		pulseDepreciation(ControlChannel* channel);

float             	pulseToPercents(ControlChannel* channel);

virtual void		ReceiptCompleteEvent(Ring *data, size_t size);

virtual void		TransmitCompleteEvent(Ring *data, size_t size);

void 				ibusReceiver(Ring *data, size_t size);

void 				ibusChannels(uint8_t* buffer);

void 				ibusSensors(uint8_t* buffer);

u8 				    ibusChecksum(uint8_t* buffer);

void 				ibusReset(void);

volatile uint8_t          	channelsCount :6;
SensorInfo** 		Sensors;
volatile uint8_t 			sensorsCount:6;
volatile uint8_t  	available :1;

SerialDevice*		Serial;

uint8_t* 			requestBuffer;
uint8_t* 			responseBuffer;
volatile uint8_t 			requestLength = 0;
volatile uint8_t 			responseLength = 0;
volatile uint8_t				frameLength= 0;
volatile uint32_t 			requestTickms = 0;
volatile uint8_t				frameStarted = 0;

typedef enum
{
	NONE,
	CHANNELS,
	SENSORS
} RequestType;

RequestType		requestType = NONE;

uint16_t* 			channelValues;
uint16_t* 			innerChannelValues;

};

extern RC_IBUS *RC;


#endif /* RC_IBUS_H */
