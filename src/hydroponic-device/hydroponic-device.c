/*
 * hydroponic-device.c
 *
 *  Created on: Jul 15, 2017
 *      Author: chaunm
 */
#include "hydroponic-device.h"
#include "serial-protocol.h"
#include "pthread.h"

static HYDROPONIC_DEVICE hydroponicDevice;
static pthread_t SerialProcessThread;
static pthread_t SerialOutputThread;
static pthread_t SerialHandleThread;

PSERIAL DeviceOpenSerial(char* SerialPort, BYTE_HANDLER_CALLBACK byteHandler, PACKAGE_HANDLER_CALLBACK packageHandler)
{
	PSERIAL	pSerialPort;
	char* PortName = malloc(strlen("/dev/") + strlen(SerialPort) + 1);
	memset(PortName, 0, strlen("/dev/") + strlen(SerialPort) + 1);
	sprintf(PortName, "%s%s", "/dev/", SerialPort);
	printf("open port %s\n", PortName);
	pSerialPort = SerialOpen(PortName, B115200);
	free(PortName);
	if (pSerialPort == NULL)
	{
		printf("Can not open serial port %s, try another port\n", PortName);
		return NULL;
	}
	SerialSetByteHandler(pSerialPort, byteHandler);
	SerialSetPackageHandler(pSerialPort, packageHandler);
	// Initial Serial port handle process
	pthread_create(&SerialProcessThread, NULL, (void*)&SerialProcessIncomingData, (void*)pSerialPort);
	pthread_create(&SerialOutputThread, NULL, (void*)&SerialOutputDataProcess, (void*)pSerialPort);
	pthread_create(&SerialHandleThread, NULL, (void*)&SerialInputDataProcess, (void*)pSerialPort);
	return pSerialPort;
}

VOID DeviceCloseSerial()
{
	if (hydroponicDevice.serialPort != NULL)
	{
		pthread_cancel(SerialProcessThread);
		pthread_cancel(SerialOutputThread);
		pthread_cancel(SerialHandleThread);
		SerialClose(hydroponicDevice.serialPort);
		hydroponicDevice.serialPort = NULL;
	}
}

PACTOR DeviceCreateActor(PACTOROPTION actorOption)
{
	PACTOR pDeviceActor = ActorCreate(actorOption->guid, actorOption->psw, actorOption->host, actorOption->port);
	//Register callback to handle request package
	if (pDeviceActor == NULL)
	{
		printf("Couldn't create actor\n");
		return NULL;
	}
	/* Listen on list of topic to get data */
//	char* actionTopic;
//	actionTopic = ActorMakeTopicName("action/", guid, "/add_devices");
//	ActorRegisterCallback(pZnpActor, actionTopic, ZnpActorOnRequestAddDevice, CALLBACK_RETAIN);
//	free(actionTopic);
//	actionTopic = ActorMakeTopicName("action/", guid, "/remove_device");
//	ActorRegisterCallback(pZnpActor, actionTopic, ZnpActorOnRequestRemoveDevice, CALLBACK_RETAIN);
//	free(actionTopic);
	return pDeviceActor;
}

BYTE DeviceStart(PACTOROPTION actorOption, char* SerialPort)
{
	hydroponicDevice.serialPort = DeviceOpenSerial(SerialPort, SerialProtocolByteHandler, SerialProtocolPackageHandler);
	if (hydroponicDevice.serialPort == NULL)
		return 1;
	mosquitto_lib_init();
	hydroponicDevice.actor = DeviceCreateActor(actorOption);
	if (hydroponicDevice.actor == NULL)
	{
		DeviceCloseSerial();
		mosquitto_lib_cleanup();
		return 2;
	}
	while(1)
	{
		ActorProcessEvent(hydroponicDevice.actor);
		mosquitto_loop(hydroponicDevice.actor->client, 0, 1);
		usleep(10000);
	}
	mosquitto_disconnect(hydroponicDevice.actor->client);
	mosquitto_destroy(hydroponicDevice.actor->client);
	mosquitto_lib_cleanup();
	return 0;
}

