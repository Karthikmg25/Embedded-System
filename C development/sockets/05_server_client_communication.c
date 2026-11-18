#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>
#include <ws2tcpip.h>

/* This program creates a server socket and communicates with a client till end of connection */

// declare helper functions for send/recv n bytes of data
int send_all(SOCKET sock, const char* buffer, size_t len);
int recv_all(SOCKET sock, char* buffer, size_t len);

// declare a recv function to receive data till a delimiter
const char delimiter = '#';
int recv_until_delimiter(SOCKET sock, char* buffer, size_t buf_capacity);



int main()
{

    // 1. initialize winsock
    
    WSADATA wsa;
    int result = WSAStartup(MAKEWORD(2,2), &wsa);

    if(result != 0)
    {
        fprintf(stderr, "Winsock init failed\nerror code: %d", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized successfully\n");

    // 2. create a socket //

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);// Using IPv4, TCP protocol socket
    if(server == INVALID_SOCKET){
        fprintf(stderr, "Socket creation failed\nerror code: %d", WSAGetLastError());
        return 1;}
    printf("Socket created for server!\n");

    // 3. bind socket to IP, port //

    struct sockaddr_in serv_addr={};

    serv_addr.sin_family = AF_INET;// IPv4: 32 bit address
    serv_addr.sin_port   = htons(1337);
    serv_addr.sin_addr.s_addr =  inet_addr("192.168.31.133");//INADDR_ANY;//

    result = bind(server, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(result != 0){
        fprintf(stderr, "failed to bind to the ip address\nerror code: %d", WSAGetLastError());
        return 1;
    }
    printf("Socket binded to IP/port\n");

    // 4. start listeing for client connections

    result = listen(server, 5);// keep max 5 client requests in wait queue
    if(result != 0){
        fprintf(stderr, "Listen failed\nerror code: %d", WSAGetLastError());
        return 1;
    }
    printf("Listening for connections...\n");


    // 5. accept connection from a client

    SOCKET client = accept(server, NULL, NULL);
    if(client == INVALID_SOCKET){
        fprintf(stderr,"Failed to connect to client\nerror code: %d", WSAGetLastError());
        return 1;
    }
    printf("Connected to client\n");


    // 6. send/recv data from client till end of connection
    while(true)
    {
        // send data to client
        char buffer[100];
        printf("\n\nSend data to client: ");
        fgets(buffer, 100, stdin);

        int n = send_all(client, buffer, strlen(buffer));

        if(n == -1){
            fprintf(stderr, "Connection closed");
              // close sockets and cleanup
            closesocket(server);
            closesocket(client);
            WSACleanup();
            return 1;
        }


        printf("\n\nWaiting for data from client....");


        // receive data from client
        n = recv_until_delimiter(client, buffer, 100);

        if(n == -1){
            fprintf(stderr, "\nConnection closed");
              // close sockets and cleanup
            closesocket(server);
            closesocket(client);
            WSACleanup();
            return 1;
        }
        buffer[n] = '\0';
        printf("\n\nData from client: %s", buffer);

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