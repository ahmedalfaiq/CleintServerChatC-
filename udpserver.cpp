//#pragma warning(disable : 4996)

//https://github.com/youFrivolous/sw-project/wiki/UDP-TCP-Example-in-C
#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ws2tcpip.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#define BUFLEN 512 //Max length of buffer
#define PORT 8888 //The port on which to listen for incoming data
int main()
{
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;
	slen = sizeof(si_other);
	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");
	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	//Bind
	if (bind(s, (struct sockaddr*) & server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");
	//keep listening for data
	char m[512];
	while (1) {

		cout << ("Waiting for data...") << endl;;
		fflush(stdout);
		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)
			& si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d\n",
				WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		//puts(buf);
		if (equal(buf, buf + 2, "by")) {
			cout << "Client has quit the session" << endl;
			break;
		}
		//print details of the client/peer and the data received
		cout << "Received " << buf << " packet from " <<
			inet_ntoa(si_other.sin_addr)<< ntohs(si_other.sin_port) << endl;
		//printf("Data: %s\n", buf);
		cout << "\nEnter your message to client: ";
		cin >> m;
		// Echo the buffer back to the sender
		char* message = m;
		if (equal(m, m + 2, "by")) {
			sendto(s, message, strlen(m), 0, (struct sockaddr*) & si_other,
				slen);
			cout << "Server has quit the session" << endl;
			break;
		}

		//now reply the client with the same data
		if (sendto(s, message, strlen(m), 0, (struct sockaddr*) & si_other,
			slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d",
				WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}




/////
#include<stdio.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#define BUFLEN 512 //Max length of buffer
#define PORT 8888 //The port on which to listen for incoming data
int main()
{
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;
	slen = sizeof(si_other);
	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");
	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");
	//keep listening for data
	while (1){

		if (s,buf,recv_len,(struct sockaddr*)+2, "by" )
		{
			printf ( "Client has quit the session: %d",
				WSAGetLastError());
			
		}
		printf("Waiting for data...");
		fflush(stdout);
		//clear the buffer by filling null, it might have previously received data
			memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)
			&si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d",
				WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n",
			inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n", buf);
		//now reply the client with the same data
		if (sendto(s, buf, recv_len, 0, (struct sockaddr*)&si_other,
			slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d",
				WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		
		
	closesocket(s);
	WSACleanup();
	return 0;
}

///
#include<stdio.h>
#include<winsock2.h>
#include "iostream"
using namespace std;
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#define SERVER "127.0.0.1" //ip address of udp server
#define BUFLEN 512 //Max length of buffer
#define PORT 8888 //The port on which to listen for incoming data
int main(void)
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;
	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");
	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
	//start communication
	while (1)
	{
		printf("Enter message : ");
		cin>>(message);
		//send the message
		if (sendto(s, message, strlen(message), 0, (struct sockaddr*)&si_other,
			slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		if(equal(message,message+2,"by")){
			printf("Client disconnected\n");
			break;	
		}

		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen) ==
			SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d",
				WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		//puts(buf);
		printf("Data: %s\n", buf);
		if(equal(buf,buf+2,"by")){
			printf("Server disconnected\n");
			break;
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;

}