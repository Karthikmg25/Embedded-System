#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>
#include <ws2tcpip.h>

/* This program creates a client socket and communicates with a server till end of connection */

// declare helper functions for send/recv n bytes of data
int send_all(SOCKET sock, const char* buffer, size_t len);
int recv_all(SOCKET sock, char* buffer, size_t len);

// declare a recv function to receive data till a delimiter
const char delimiter = '#';
int recv_until_delimiter(SOCKET sock, char* buffer, size_t buf_capacity);


int main()
{
    // 1. initialize winsock subsystem
	WSADATA wsa;
	int result = WSAStartup(MAKEWORD(2,2), &wsa);// requests version 2.2
	if( result != 0){
		fprintf(stderr, "Winsock startup failed\nerror code: %s", WSAGetLastError());
		return 1;
	}
	printf("Winsock initialized successfully\n");

	// 2. create a socket

	SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
	if(client == INVALID_SOCKET){
		fprintf(stderr, "Socket creation failed\nerror code: %d", WSAGetLastError());
		return 1;
	}
	printf("Socket created\n");

	// 2. connect to a server

	// specify the address info of server we need to connect  : connects now with the server pgm
    // with addr-> 192.168.31.172:1024   -> mobile server     : 
    // with addr-> 192.168.31.133:1337   -> PC server program : 
    
	struct sockaddr_in serv_addr = {};                                                              

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(1337);
	inet_pton(AF_INET, "192.168.31.133", &serv_addr.sin_addr.s_addr);

	printf("Connecting....\n");
	result = connect(client, (struct sockaddr*)&serv_addr, sizeof(serv_addr));// this blocks till server accepts connection
	if(result != 0){
		fprintf(stderr, "Connect failed\nerror code: %d", WSAGetLastError());
		return 1;
	}
	printf("Connected to server !\n");


	// 3. send/recv data till end of connection
    while(true)
    {
                                       // receive data from the server //

	    char rx_buffer[200]={};

	    printf("\nWaiting for data from server...\n\n");
	    int n = recv_until_delimiter(client, rx_buffer, 200);

	    if(n == -1){
		    fprintf(stderr, "Connection closed\nerror code: %d", WSAGetLastError());
        
	        // close socket
	        closesocket(client);
	        printf("Socket closed\n");

	        // cleanup winsock
	        WSACleanup();
		    return 1;
	    }
	    rx_buffer[n] = '\0';// provide null termination
	    printf("Data from the server: %s\n", rx_buffer);

	                                   // send some data to server //

        printf("\nSend Your reply to server: ");

	    char data[100];
        fgets(data, 100, stdin);

	    n = send_all(client, data, strlen(data));
	    if(n == -1){
		    fprintf(stderr, "Data send failed\nerror code: %d", WSAGetLastError());
            
	        // close socket
	        closesocket(client);
	        printf("Socket closed\n");

	        // cleanup winsock
	        WSACleanup();
		    return 1;
	    }
	    printf("Data send to server: %s\n",data );
    }



	return 0;
}

typedef size_t sz;
int send_all(SOCKET sock, const char* buffer, size_t len)
{
	sz bytes_send = 0;

	while(bytes_send < len)
	{
		int n = send(sock, buffer+bytes_send, len-bytes_send, 0);

		if(n <= 0){
			return -1;// error/connection closed
		}

		bytes_send += n;
	}

	return 0;// success
}
int recv_all(SOCKET sock, char* buffer, size_t len)
{
	sz bytes_recvd = 0;

	while(bytes_recvd < len)
	{
		int n = recv(sock, buffer+bytes_recvd, len-bytes_recvd, 0);
		if(n <= 0){
		return -1;// error/connection closed
		}
		bytes_recvd += n;
	}

	return 0; // success

}
int recv_until_delimiter(SOCKET sock, char* buffer, size_t buf_capacity)
{
	sz bytes_recvd = 0; 
	sz chunk_size  = 15;// recv a max of 15 bytes per each recv()

	while(true)
	{
		// first check if buf_capacity is sufficient to hold chunk_size of data
		sz remaining_buf_capacity = buf_capacity - bytes_recvd;
		if(remaining_buf_capacity < chunk_size -1){return -1;}// insufficient buffer space, -1 for holding '\0'

		int n = recv(sock, buffer+bytes_recvd, chunk_size, 0);

		if(n <= 0){
			return -1;// error recving data / peer has closed connection
		}

		// check if recved chunk has the delimiter
		for( int i =0; i< n; i++)
		{
			if(buffer[bytes_recvd + i] == delimiter)
            {
                return bytes_recvd+i;// returns index of delimiter when found in the chunk
            }
		
		}
        
       
		bytes_recvd += n;

	}
}