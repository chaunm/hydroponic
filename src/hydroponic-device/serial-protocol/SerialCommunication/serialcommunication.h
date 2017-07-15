/*
 * serialcommunication.h
 *
 *  Created on: Jan 14, 2016
 *      Author: ChauNM
 */

#ifndef SERIALCOMMUNICATION_H_
#define SERIALCOMMUNICATION_H_
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "typesdef.h"
#include "queue.h"

#define MAX_SERIAL_PACKAGE_SIZE		520
#define SERIAL_QUEUE_SIZE			32

typedef void (*BYTE_HANDLER_CALLBACK) (PVOID, BYTE, PWORD, PBYTE);
typedef void (*PACKAGE_HANDLER_CALLBACK) (PBYTE, WORD);
/* Serial management data struct */
#pragma pack(1)
typedef struct stSerial {
	unsigned int uiBaudrate;
	int	tty_fd;
	PQUEUECONTROL pInputQueue;
	PQUEUECONTROL pOutputQueue;
	BYTE_HANDLER_CALLBACK SerialByteHandler;
	PACKAGE_HANDLER_CALLBACK SerialPackageHandler;
} SERIAL, *PSERIAL;


/* Exported function */
PSERIAL SerialOpen (char* PortName, unsigned int uiBaudrate);
VOID SerialClose(PSERIAL pSerialPort);
VOID SerialProcessIncomingData(PSERIAL pSerialPort);
VOID SerialOutputDataProcess(PSERIAL pSerialPort);
VOID SerialInputDataProcess(PSERIAL pSerialPort);
BYTE SerialOutput(PSERIAL pSerialPort, PBYTE pData, BYTE nSize);
void SerialSetByteHandler(PSERIAL pSerialPort, BYTE_HANDLER_CALLBACK ByteHandler);
void SerialSetPackageHandler(PSERIAL pSerialPort, PACKAGE_HANDLER_CALLBACK PackageHandler);
#endif /* SERIALCOMMUNICATION_H_ */
