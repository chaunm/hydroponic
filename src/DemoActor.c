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
#include "actor.h"
#include "hydroponic-device.h"

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
	//pthread_t DemoActorThread;
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
			SerialPort = StrDup((const char*)optarg);
			break;
		case 'i':
			guid = StrDup((const char*)optarg);
			break;
		case 't':
			token = StrDup((const char*)optarg);
			break;
		case 'u':
			ttl = atoi(optarg);
			break;
		case 'H':
			mqttHost = StrDup((const char*)optarg);
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

	/* Start Device */
	if (DeviceStart(&option, SerialPort) != 0)
	{
		printf ("hydroponic device start fail\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
