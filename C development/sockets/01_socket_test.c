#include <stdio.h>
#include <winsock2.h>    // Provides core windows socket APIs
#include <ws2tcpip.h>    // Provides TCP/IP related deffinitions and declarations

/* This program test use of winsock APIs for sockets programming */

int main()
{
	// Initialize winsock networking subsystem
	/*******************************************************************************************************/
	WSADATA wsa;
	int result = WSAStartup(MAKEWORD(2,2), &wsa);//  request winsock version 2.2

	/*
	WSADATA wsa;   — declares a structure that Winsock fills with information about the initialized implementation.
	MAKEWORD(2, 2) — constructs a value requesting Winsock version 2.2.
	*/

	if(result != 0){
		printf("WSAStartup failed!"); return 1;
	}
	printf("Initialized winsock successfully !! ");

	// cleanup
	/*******************************************************************************************************/
	WSACleanup();
	return 0;
}

