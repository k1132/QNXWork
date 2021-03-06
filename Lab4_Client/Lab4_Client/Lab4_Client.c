#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "mystruct.h"

int main(int argc, char* argv[]) //program name (0), pid (1), a (2), op (3), b (4)
{
	int coid, i;
	request myRequest;
	response myResponse;

	for (i = 0; i <=4; i++){
		if (argv[i] == NULL){
			printf("Error: Arg %d is NULL. Incorrect number of arguments \n", i);
			exit(EXIT_FAILURE);
		}
	}

	pid_t serverpid = atoi(argv[1]);
//	myRequest.a = atoi(argv[2]);
//	myRequest.op = argv[3][0];
//	myRequest.b = atoi(argv[4]);

	char *end;
	char *number = argv[2];
	long valueA = strtol(number, &end, 10);
	if (end == number || *end != '\0' || errno == ERANGE){
		printf("Error, number too large\n");
		exit(EXIT_FAILURE);
	}
	myRequest.a = valueA;

	myRequest.op = argv[3][0];

	number = argv[4];
	long valueB = strtol(number, &end, 10);
	if (end == number || *end != '\0' || errno == ERANGE){
		printf("Error, number too large\n");
		exit(EXIT_FAILURE);
	}
	myRequest.b = valueB;

	// establish a connection
	coid = ConnectAttach(ND_LOCAL_NODE, serverpid, 1, _NTO_SIDE_CHANNEL, 0); //connects to the server by PID
	if (coid == -1) {
		fprintf(stderr, "Couldn't ConnectAttach\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	// send the message
	if (MsgSend(coid, &myRequest, sizeof(myRequest) + 1, &myResponse,
			sizeof(myResponse)) == -1) { //sends msg to server and SEND blocks until it gets a reply
		fprintf(stderr, "Error during MsgSend\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	if (myResponse.status == 0) {
		printf("The server has calculated the result of %d %c %d as %f.\n",
				myRequest.a, myRequest.op, myRequest.b, myResponse.result);
	} else if (myResponse.status == -1) {
		printf("The server returned status %d with the error message: %s \n",
				myResponse.status, myResponse.err);
	}

	//Disconnect from the channel
	ConnectDetach(coid);

	return EXIT_SUCCESS;
}
