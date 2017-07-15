/*
 ============================================================================
 Name        : ZigbeeHost.c
 Author      : ChauNM
 Version     :
 Copyright   :
 Description : C Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include "serialcommunication.h"
#include "universal.h"
#include "actor.h"

void PrintHelpMenu() {
	printf("program: hydroponic-master\n"
			"using ./hydroponic-master --host --port --id --token --serial\n"
			"--serial: Serial port used to communicate with ZNP device (ex.: ttyUSB0, ttyAMA0..)\n"
			"--id: guid of the znp actor\n"
			"--token: pasword to the broker of the znp actor, this option can be omitted\n"
			"--host: mqtt server address can be ommitted\n"
			"--port: mqtt port - can be omitted\n"
			"--update: time for updating online message to system");
}

int main(int argc, char* argv[])
{

	pthread_t SerialProcessThread;
	pthread_t SerialOutputThread;
	pthread_t SerialHandleThread;
	//pthread_t DemoActorThread;
	PSERIAL	pSerialPort;
	BOOL bResult = FALSE;
	/* get option */
	int opt= 0;
	char *token = NULL;
	char *guid = NULL;
	char *SerialPort = NULL;
	char *mqttHost = NULL;
	WORD mqttPort = 0;
	WORD ttl = 0;

	// specific the expected option
	static struct option long_options[] = {
			{"id",      required_argument, 0, 'i' },
			{"token", 	required_argument, 0, 't' },
			{"serial",    required_argument, 0, 's' },
			{"update", 	required_argument, 0, 'u' },
			{"host", required_argument, 0, 'H'},
			{"port", required_argument, 0, 'p'}
	};
	int long_index;
	/* Process option */
	while ((opt = getopt_long(argc, argv,":hi:t:s:u:H:p:",
			long_options, &long_index )) != -1) {
		switch (opt) {
		case 'h' :
			PrintHelpMenu();
			return EXIT_SUCCESS;
			break;
		case 's' :
			SerialPort = StrDup(optarg);
			break;
		case 'i':
			guid = StrDup(optarg);
			break;
		case 't':
			token = StrDup(optarg);
			break;
		case 'u':
			ttl = atoi(optarg);
			break;
		case 'H':
			mqttHost = StrDup(optarg);
			break;
		case 'p':
			mqttPort = atoi(optarg);
			break;
		case ':':
			if ((optopt == 'i') || optopt == 'p')
			{
				printf("invalid option(s), using -h for help\n");
				return EXIT_FAILURE;
			}
			break;
		default:
			break;
		}
	}
	if ((SerialPort == NULL) || (guid == NULL))
	{
		printf("invalid options, using -h for help\n");
		return EXIT_FAILURE;
	}
	ACTOROPTION option;
	option.guid = guid;
	option.psw = token;
	option.host = mqttHost;
	option.port = mqttPort;

	/* Init device list */
//	DeviceListInit();

	/* Start MQTT actor */
//	FLUENT_LOGGER_INFO("znp actor start");
//	ZnpActorStart(&option);
	printf("hydroponic actor start success\n");

	/* open serial port and init queue for serial communication */
	char* PortName = malloc(strlen("/dev/") + strlen(SerialPort) + 1);
	memset(PortName, 0, strlen("/dev/") + strlen(SerialPort) + 1);
	sprintf(PortName, "%s%s", "/dev/", SerialPort);
	printf("open port %s\n", PortName);
	pSerialPort = SerialOpen(PortName, B115200);
	if (pSerialPort == NULL)
	{
		printf("Can not open serial port %s, try another port\n", PortName);
		return EXIT_FAILURE;
	}
	free(PortName);
	// Initial Serial port handle process
	pthread_create(&SerialProcessThread, NULL, (void*)&SerialProcessIncomingData, (void*)pSerialPort);
	pthread_create(&SerialOutputThread, NULL, (void*)&SerialOutputDataProcess, (void*)pSerialPort);
	pthread_create(&SerialHandleThread, NULL, (void*)&SerialInputDataProcess, (void*)pSerialPort);

	while (1)
	{
		sleep(1);
	}
	SerialClose(pSerialPort);
	return EXIT_SUCCESS;
}
