#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <winsock2.h> // winsock APIs
#include <ws2tcpip.h> // TCP/IP APIs

/* This program creates a TCP server and communicates with a client socket */

// declare funtions to send and recieve  n bytes of data
int send_all(SOCKET sock, const char* buffer, size_t len);
int recv_all(SOCKET sock, char* buffer, size_t len);

const char delimiter = '#';
// declare a function to receive data till delimiter character: returns index of delimiter
int recv_until_delimiter(SOCKET sock, char* buffer, size_t buf_capacity);
int main()
{

	// Winsock initialization
	/**************************************************************************************/
	WSADATA wsa;
	int result = WSAStartup(MAKEWORD(2,2), &wsa);
	if(result != 0){printf("WSAStartup failed");return 1;}
	printf("Winsock initialized successfully\n");

	// Create a socket
	/**************************************************************************************/

	SOCKET server = socket(AF_INET,     // Using address family - IPV4
		                   SOCK_STREAM, // TCP socket
						   0// default
						   );
	if(server == INVALID_SOCKET){
		printf("Socket creation failed \n error code: %d", WSAGetLastError()); return 1;}						   
	printf("Socket created successfully\n");

	// Bind socket to a port and IP address
	/**************************************************************************************/

	struct sockaddr_in serv_addr ={};

	serv_addr.sin_family = AF_INET;             // IPV4
	serv_addr.sin_port   = htons(1337);         // bind to port 1337
	serv_addr.sin_addr.s_addr = INADDR_ANY;// liten through all available IPV4 interfaces

	result = bind(server, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(result == SOCKET_ERROR){printf("Bind failed \n error code: %d", WSAGetLastError());return 1;}
	printf("Socket binded successfully\n");

	// Start listening
	/**************************************************************************************/

	result = listen(server, 5); // Upto 5 waiting clients: keep a maximum of upto 5 connection requests in the backlog queue.
	if (result == SOCKET_ERROR)
	{
    printf("Listen failed: %d\n", WSAGetLastError());
    closesocket(server);
    WSACleanup();
    return 1;
	}

	printf("Server started listening...\n");


	// Accept connection request from client sockets
	/**************************************************************************************/

	printf("Waiting for client connections...\n");

	SOCKET client = accept(server, NULL, NULL);// Use NULL if we dont need address info of connected client socket
		if(client == INVALID_SOCKET){printf("Connection failed\nerror code: %d", WSAGetLastError());
			WSACleanup();closesocket(server); return 1;}
	printf("Client connected\n");

	// Send, receive data
	/**************************************************************************************/

	const char buffer[] = "Hello from server with delimiter !#";// <------------ Sending 20 bytes, # as delimiter
	int status = send_all(client, buffer, strlen(buffer));
	if(status != 0){ printf("Failed to send the data");return 1;}
	printf("Data send successfully\n");

	char buf[100];
	printf("Waiting for data from client....\n");

	int bytes = recv_until_delimiter(client, buf, 100); 
	buf[bytes] = '\0'; // !! Provide NULL termination to string !! //

	if(bytes <= 0){printf("Error/connection lost\n");return 1;}
	printf("Data received: %s\n", buf);


	// Close sockets
	/**************************************************************************************/
	closesocket(server);
	closesocket(client);
	printf("Sockets closed!\n");

	// Winsock cleanup
	/**************************************************************************************/
	WSACleanup();

	return 0;
}

int send_all(SOCKET sock, const char* buffer, size_t len)
{
	int send_bytes=0;

	while(send_bytes < len)
	{
		int n = send(sock, buffer+send_bytes, (int)(len - send_bytes), 0);

		// Error check:
		if( n == SOCKET_ERROR || n == 0){
			return -1;
		}
		send_bytes += n;
	}
	
	return 0;

}
int recv_all(SOCKET sock, char* buffer, size_t len)
{
	size_t bytes_rcv = 0;

	while( bytes_rcv < len)
	{
		int n = recv(sock, buffer + bytes_rcv, len - bytes_rcv, 0);

		if(n <= 0){
			// error
			return 1;
		}
		bytes_rcv += n;
	}

	// success
	return 0;
}
int recv_until_delimiter(SOCKET sock, char* buffer, size_t buf_capacity)
{
	size_t recieved_bytes = 0;
	size_t chunk_size = 15;// recv max 15 bytes at a time
	while(true)
	{

		// check if buffer has capacity for further recieve
		size_t remaining = buf_capacity - recieved_bytes -1; // -1 for storing '\0'

		if(remaining < chunk_size) return -1;// error -> no delimiter found, but no space in buffer

		// reveive chunk of data
		int n = recv(sock, buffer+recieved_bytes, chunk_size, 0);

		if(n <= 0){return -1;}//error/connection lost

		// check delimiter char inside received buffer
		for(int i =0;i < n;i++)
		{
			if((buffer+recieved_bytes)[i] == delimiter)
			{
				return recieved_bytes+i; // return index of delimiter
			}
		}

		recieved_bytes += n;

	}
}