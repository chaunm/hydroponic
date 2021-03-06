/*
 * queue.h
 *
 *  Created on: Jan 22, 2016
 *      Author: ChauNM
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "typesdef.h"

#define BUFFER_SIZE		520
#define MAX_QUEUE_SIZE	128

#define QUEUE_ACTIVE	0
#define QUEUE_WAIT		1

#pragma pack(1)
typedef struct tagQueueControl {
	BYTE nQueueSize;
	WORD nBufferSize;
	BYTE nState;
	BYTE nCurrentPointer;
	BYTE nFreePointer;
	PBYTE pContent;
	PBYTE pContentLength;
} QUEUECONTROL, *PQUEUECONTROL;

#pragma pack(1)
typedef struct tagQueueContent {
	WORD nSize;
	PBYTE pData;
} QUEUECONTENT, *PQUEUECONTENT;

PQUEUECONTROL QueueCreate(BYTE nQueueSize, WORD nBufferSize);
void QueueFreeMem(PQUEUECONTROL pQueue);
BYTE QueuePush(void* pSource, WORD nSize, PQUEUECONTROL pQueue);
QUEUECONTENT QueueGetContent(PQUEUECONTROL pQueue);
void QueueFinishProcBuffer(PQUEUECONTROL pQueue);
void QueueSetState(PQUEUECONTROL pQueue, BYTE nState);
BYTE QueueGetState(PQUEUECONTROL pQueue);
#endif /* QUEUE_H_ */
