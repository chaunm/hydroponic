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
	BYTE content[];
} PROTOCOL_PACKAGE, *PPROTOCOL_PACKAGE;

typedef struct stProtocolContent {
	BYTE srcAddr;
	BYTE destAddr;
	BYTE command;
	BYTE commandData[];
} PROTOCOL_CONTENT, *PPROTOCOL_CONTENT;

#endif /* SRC_PROTOCOL_PROTOCOL_H_ */
