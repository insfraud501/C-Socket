
/*  The whole purpose of this program is..
		   Client->Server socket

	Client.exe: Connect o to remote server listening on port to send messages and data.

	** Note : this program is only useable in Windows as it uses the Windows Socket API.**

	Below are the steps that server.exe does in order to establish a connection with a client.

	1.  Create Socket.
	2.  Bind socket to server.The PORT and IP will be assigned here.
	3.  Listen for any possible connections In attempt to find the client wandering around looking for a signal
	4.  Accept the connection.
	5.  Receive messages entered in by the client.

	If the client enters in "quit" into their prompt, the connection will be brokenand the sockets will be closed.

	@return -1: An Error has occured in the program.A WSA Error code will be provided.
	@return 0: The socket has been closed and the program has been cleaned up.
	@return 3: The client disconnected from the server.The server will be shut down.
	@return -2: An Error has occured in the programm. A WSA Error Code may not be provided.

*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_LENGTH 2048 // The max amount of bytes to be allowed to send and receive.

#include <stdio.h>
#include <WinSock2.h>

//#pragma comment(lib, "ws2_32.lib")

static inline void rline() {
	/* Clear the Windows Termincal. */
	system("cls");
}

static char* ArgumentSearch(char* keyword, int argc, char* argv[]) {
	// Search for Port and Ip parameters in command line. Otherwise terminate program.
	for (int i = 1; i < argc; i++) {

		// Search for keyword argument
		if (strcmp(argv[i], keyword) == 0 && i + 1 < argc) {
			return argv[i + 1];
		}

	}
	return 0;

}

int main(int argc, char** argv) 
{
	
	// Search for command line arguments to set these variables to. Used when creating socket.
	const int   PORT              = atoi(ArgumentSearch("-p", argc, argv)); // The port to be used to communicate server - client and vice - versa.
	const char* CLIENT_MACHINE_IP = ArgumentSearch("-ip", argc, argv); // The IP of the machine which client.exe is running on.

	if (PORT == 0 || CLIENT_MACHINE_IP == 0) 
	{
		printf("Invalid Arugments Provided. Exiting.");
		printf("Use -p to define the port and -ip to define the ip");

		return -2;
	}

	WORD WSA = 2;
	WSADATA WSAData;
	 
	if (0 > WSAStartup(WSA, &WSAData)) 
	{
		printf("Error starting up WSA. Error Code: %d\n", WSAGetLastError());
		return -1;
	}

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

	char* message = (char*) malloc((MAX_LENGTH + 1) * sizeof(char));
	//char* message[MAX_LENGTH] = { 0 };

	while (sfd != INVALID_SOCKET) {
		rline();
		printf("Established Connection With SERVER.");
		printf("\n- SERVER IP Address: %s\n", inet_ntoa(server_info.sin_addr));

		printf("\nEnter data to send to client.\n");
		printf("Enter \"quit\" to break:\n");
		fgets(message, MAX_LENGTH, stdin); // Get message input to send to the client.

		// Check if message is equal to quit to terminate connection with the client
		if (strcmp(message, "quit\n") == 0)
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


	closesocket(sfd);
	WSACleanup();

	return 0;
}