
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_LENGTH 2048 // The max amount of bytes to be allowed to send and receive.

#include <stdio.h>
#include <WinSock2.h>

static inline void rline() {
	/* Clear the Windows Termincal. */
	system("cls");
}

static char* ArgumentSearch(char* keyword, int argc, char* argv[]) {
	// Search for Port and Ip parameters in command line. Otherwise terminate program.
	for (int i = 1; i < argc; i++) {

		// Search for keyword argument
		if (strcmp(argv[i], keyword) == 0 && i + 1 < argc) {
			return argv[i + 1]; // return parameter after keyword
		}

	}
	return 0;

}

int main(int argc, char** argv) 
{
	
	// Search for command line arguments to set these variables to. Used when creating socket.
	const int   PORT              = atoi(ArgumentSearch("-p", argc, argv)); // The port to be used to communicate server - client and vice - versa.
	const char* CLIENT_MACHINE_IP = ArgumentSearch("-ip", argc, argv); // The IP of the machine which client.exe is running on.
	const char* MODE              = ArgumentSearch("-m", argc, argv);

	if (PORT == 0 || CLIENT_MACHINE_IP == 0 || MODE == 0) 
	{
		printf("Invalid Arugments Provided. Exiting.");
		return -2;
	}

	// Initialize Windows Socket API
	WORD WSA = MAKEWORD(2, 2);
	WSADATA WSAData;
	 
	if (0 > WSAStartup(WSA, &WSAData)) 
	{
		printf("Error starting up WSA. Error Code: %d\n", WSAGetLastError());
		
		return -1;
	}

	// Set socket options
	struct sockaddr_in server_info = { 0 };
	server_info.sin_family         = AF_INET;
	server_info.sin_addr.s_addr    = inet_addr(CLIENT_MACHINE_IP);
	server_info.sin_port           = htons(PORT);
	
	int server_info_len = sizeof(server_info);
	
	SOCKET sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == INVALID_SOCKET) 
	{
		printf("Error creating socket. Error Code: %d\n", WSAGetLastError());
		WSACleanup();
		
		return -1;
	}

	printf("Attempting to connect to server.\n");
	if (0 > connect(sfd, ( struct sockaddr*)&server_info, server_info_len))
	{
		printf("Error while connecting. Error Code: %d\n", WSAGetLastError());
		WSACleanup();
		
		return -1;
	}

	// If -m == send then we are only sending information to server.
	if (!strcmp(MODE, "send"))
	{
		
		char* message = malloc((MAX_LENGTH + 1) * sizeof(char));

		while (sfd != INVALID_SOCKET) {
			rline();
			printf("Established Connection With SERVER.");
			printf("\n- SERVER IP Address: %s\n", inet_ntoa(server_info.sin_addr));

			printf("\nEnter data to send to server.\n");
			printf("Enter \"quit\" to break:\n");
			fgets(message, MAX_LENGTH, stdin); // Get message input to send to the server.

			// Check if message is equal to quit to terminate connection with the server
			if (!strcmp(message, "quit\n"))
			{
				rline();
				printf("Disconnecting from server.");
				break;
			}

			// Send message to server
			size_t socketSend = send(sfd, message, strlen(message), 0);
			if (0 > socketSend)
			{
				printf("Send failed. Error Code: %d", WSAGetLastError());
				closesocket(sfd);
				WSACleanup();
				return -1;
			}


		}
	
	}
	
	// If the -m argument is recv than that means the client will be receivng information sent from the server.
	else if (!strcmp(MODE, "recv")) 
	{	

		// Tell the server we are receving information
		Sleep(1);

		if (0 > send(sfd, "recv", 5, 0))
		{
			printf("Send failed. Error Code: %d", WSAGetLastError());
			closesocket(sfd);
			WSACleanup();
			return -1;
		};

		char buffer[MAX_LENGTH];
		
		printf("Receiving information from server...\n");
		while (sfd != INVALID_SOCKET) 
		{

			int received = recv(sfd, buffer, MAX_LENGTH - 1, 0);

			// Check if the server stopped sending information or the server closed.
			if (received == 0)
			{
				printf("Server closed.");
				break;
			}
			
			if (received == SOCKET_ERROR) 
			{
				printf("Error while receiving information. Error Code: %d", WSAGetLastError());
				closesocket(sfd);
				WSACleanup();

				return -1;

			}

			buffer[received - 1] = '\0';
			printf("Server: %s\n", buffer);

		}
	}


	closesocket(sfd);
	WSACleanup();

	return 0;
}