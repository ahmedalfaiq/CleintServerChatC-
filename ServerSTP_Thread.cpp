
#define UNICODE 
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma warning(disable : 4996).
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
int clientnum = 0;
SOCKET  ClientSocket;
using namespace std;
struct CLIENT_INFO
{
	SOCKET ClientSocket;
	struct sockaddr_in clientAddr;
};
// Thread for each client
BOOL  WINAPI ProcessClient(LPVOID lpParam) {
//	SOCKET ClientSocket = (SOCKET)lpParam;
	CLIENT_INFO* pClientInfo = (CLIENT_INFO*)lpParam;

	// Send and receive data.
	char szBuffer[512];
	int len, iSendResult;
	while (1)
	{
		len = recv(pClientInfo->ClientSocket, szBuffer, sizeof(szBuffer), 0);
		cout << "\nRecv From"<< inet_ntoa(pClientInfo->clientAddr.sin_addr) << endl;
		for (int i = 0; i < len; i++) {
			cout << szBuffer[i];
		}
		cout << endl;
		if (equal(szBuffer, szBuffer + 2, "by")) {
			printf("\nClient Disconnected!\n");
			clientnum--;
			closesocket(pClientInfo->ClientSocket);
			return TRUE;
		}

		cout << "Enter message to client : " << endl;
		cin >> szBuffer;
		char *message = szBuffer;
		if (equal(szBuffer, szBuffer + 2, "by")) {
			send(pClientInfo->ClientSocket, message, strlen(szBuffer), 0);
			printf("\nServer  Disconnected!");
			clientnum--;
			closesocket(pClientInfo->ClientSocket);
			return TRUE;
		}
		iSendResult = send(pClientInfo->ClientSocket, message, strlen(szBuffer), 0);
		cout << "\nwaiting client response..." << endl;
	}
	return TRUE;
}

int __cdecl main(void)
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, 3);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("Now Listening...\n");
	do {
		ClientSocket = INVALID_SOCKET;
		struct sockaddr_in clientAddr;
		int nSize = sizeof(clientAddr);

		// Accept a client socket
		do {
			ClientSocket = accept(ListenSocket, (struct sockaddr*) & clientAddr, (int*) &nSize);
		} while (ClientSocket == SOCKET_ERROR);
		cout << "\nClient Connected: " <<ClientSocket<<" : "<<inet_ntoa(clientAddr.sin_addr) <<":"<< ntohs(clientAddr.sin_port) << endl;
		//new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port))
		struct CLIENT_INFO* clientInfo = (struct CLIENT_INFO*) malloc(sizeof(struct CLIENT_INFO));
		clientInfo->clientAddr = clientAddr;
		clientInfo->ClientSocket = ClientSocket;

		HANDLE hClientThread;
		DWORD dwThreadId;
		hClientThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ProcessClient, (LPVOID) & (*clientInfo), 0, &dwThreadId);
		if (hClientThread == NULL) {
			cout << "Unable to create client thread" << endl;
		}
		else
		{
			CloseHandle(hClientThread);
		}		
		clientnum++;
	} while (true);

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}
