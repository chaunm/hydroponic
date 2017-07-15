/*
 * protocol.h
 *
 *  Created on: Jul 15, 2017
 *      Author: chaunm
 */

#ifndef SRC_PROTOCOL_PROTOCOL_H_
#define SRC_PROTOCOL_PROTOCOL_H_

#pragma pack (1)
typedef struct stProtocolPackage {
	BYTE start;
	WORD length;
	BYTE srcAddr;
	BYTE destAddr;
	BYTE command;
	BYTE data[];
} PROTOCOL_PACKAGE, *PPROTOCOL_PACKAGE;


void SerialProtocolByteHandler(PVOID pSerial, BYTE byData, PWORD recvIndex, PBYTE recvPackage);
void SerialProtocolPackageHandler(PBYTE pData, WORD nLength);

#endif /* SRC_PROTOCOL_PROTOCOL_H_ */
