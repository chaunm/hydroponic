/*
 * protocol.c
 *
 *  Created on: Jul 15, 2017
 *      Author: chaunm
 */

#include "serialcommunication.h"
#include "serial-protocol.h"

void SerialProtocolByteHandler(PVOID pSerial, BYTE byData, PWORD recvIndex, PBYTE recvPackage)
{
	BYTE estDataLength;
	PSERIAL pSerialPort = (PSERIAL)pSerial;
	if (*recvIndex > 2)
	{
		estDataLength = (WORD)(*(PWORD)(recvPackage + 1));
	}
	else
		estDataLength = 0;
	if ((*recvIndex == 0) && (byData == 0xAA))
	{
		recvPackage[*recvIndex] = 0xAA;
		(*recvIndex)++;
	}
	else if (*recvIndex == estDataLength + 3)
	{
		if (recvPackage[*recvIndex] == 0x55)
		{
			recvPackage[*recvIndex] = byData;
			(*recvIndex)++;
			// push validated data to queue
			QueuePush((void *)recvPackage, *recvIndex, pSerialPort->pInputQueue);
			*recvIndex = 0;
		}
		else
		{
			*recvIndex = 0;
			printf("Invalid package received");
		}
	}
	else
	{
		recvPackage[*recvIndex] = byData;
		(*recvIndex)++;
	}
}

void SerialProtocolPackageHandler(PBYTE pData, WORD nLength)
{
	PPROTOCOL_PACKAGE protocolPackage;
	protocolPackage = (PPROTOCOL_PACKAGE)pData;
	switch (protocolPackage->command) {

	}
}

void SerialProtocolSendCommand(PSERIAL pSerial, BYTE command, BYTE src, BYTE dest, PBYTE pData, WORD dataLength)
{
	PBYTE outputBuffer;
	PPROTOCOL_PACKAGE outputPackage;
	WORD i;
	outputBuffer = malloc(sizeof(PPROTOCOL_PACKAGE) + dataLength + 1);
	outputPackage = (PPROTOCOL_PACKAGE)outputBuffer;
	outputPackage->start = 0xAA;
	outputPackage->length = dataLength + 3;
	outputPackage->srcAddr = src;
	outputPackage->destAddr = dest;
	for (i = 0; i < dataLength; i++)
	{
		outputPackage->data[i] = pData[i];
	}
	//stop byte
	outputPackage->data[dataLength] = 0x55;
	SerialOutput(pSerial, outputBuffer, dataLength + sizeof(PROTOCOL_PACKAGE) + 1);
}
