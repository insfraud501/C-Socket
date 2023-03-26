#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_LENGTH 2048 // The max amount of bytes to be allowed to send and receive.

#include <stdio.h>
#include <winsock2.h>

static inline void rline() {
    /* Clear the Windows Termincal. */
    system("cls"); 
}

static inline int CloseAndCleanUpSocket(SOCKET socket)
{
    closesocket(socket);
    WSACleanup();

    return 0;
}

char* ArgumentSearch(char* keyword, int argc, char* argv[]) {
    // Search for Port and Ip parameters in command line. Otherwise terminate program.
    for (int i = 1; i < argc; i++) {

        // Search for keyword argument
        if (strcmp(argv[i], keyword) == 0 && i + 1 < argc) {
            return argv[i + 1];
        }

    }
    return 0;

}

int main(int argc, char* argv[])
{

    // Search for command line arguments to set these variables to. Used when creating socket.
    const int   PORT              = atoi(ArgumentSearch("-p", argc, argv)); // The port to be used to communicate server-client and vice-versa.
    const char* SERVER_MACHINE_IP = ArgumentSearch("-ip", argc, argv); // The IP of the machine which server.exe is running on.

    // If none of the parameters are found, throw an error. Prevents failing to create a socket to unknown ip and port.
    if (PORT == 0 || SERVER_MACHINE_IP == 0) 
    {
        printf("Invalid Arugments Provided. Exiting.");
        printf("Use -p to define the port and -ip to define the ip");

        return -2;
    }

    // Start up Winsdows Socket API.
    WSADATA WSA;
    WORD WSAVersion = 2;

    if (WSAStartup(WSAVersion, &WSA) > 0)
    {
        printf("\nError starting WSA. Error Code: %d", WSAGetLastError());

        return -1;
    }

    // Create the server socket.
    SOCKET sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == INVALID_SOCKET)
    {
        printf("\nError creating socket. Error Code: %d", WSAGetLastError());
        CloseAndCleanUpSocket(sfd);

        return -1;
    }

    printf("\nSocket created...");
    rline();

    // Socket Options
    struct sockaddr_in server_info = { 0 };
    server_info.sin_addr.s_addr    = inet_addr(SERVER_MACHINE_IP);
    server_info.sin_family         = AF_INET;
    server_info.sin_port           = htons(PORT);
    
    int server_info_len = sizeof(server_info);


    // Bind Socket
    int socketBind = bind(sfd, (struct sockaddr*)&server_info, server_info_len);
    if (socketBind < 0)
    {
        printf("Error binding. Error Code: %d", WSAGetLastError());
        CloseAndCleanUpSocket(sfd);


        return -1;
    }

    printf("\nBinded socket");

    // Listen on port.
    int socketListen = listen(sfd, 0);
    if (socketListen < 0)
    {
        printf("Error while attempting to listen. Error Code: %d", WSAGetLastError());
        CloseAndCleanUpSocket(sfd);

        return -1;

    }

    rline();
    printf("Listening on Port %u.", htons(server_info.sin_port));

    struct sockaddr client_info = { 0 };
    int client_info_len         = sizeof(client_info);

    // Accept client connection.
    SOCKET cfd = accept(sfd, &client_info, &client_info_len);
    if (cfd < 0) 
    {
        printf("Error while attempting to accept. Error Code: %d", WSAGetLastError());
        CloseAndCleanUpSocket(sfd);
        CloseAndCleanUpSocket(cfd);


        return -1;
    } 
    
    rline();
    printf("\nAccepted.\n");
    rline();

    char buffer[MAX_LENGTH];

    // receive information from client. Default setting is client.exe -m is send
    while (cfd != INVALID_SOCKET) 
    {

        // Receive information from the CLIENT socket until it is closed.
        int received = recv(cfd, buffer, MAX_LENGTH - 1, 0);
        
        // Fill buffer only until the end of the message sent so you dont print random characters aswell.
        buffer[received - 1] = '\0';

        // Connection broken
        if (received == 0)
        {
            printf("\nClient disconnected from server. Socket Closed");
            CloseAndCleanUpSocket(sfd);
            CloseAndCleanUpSocket(cfd);

            return 3;
        }

        // Socket error has occured.
        else if (received == SOCKET_ERROR)
        {   
            printf("\nError while receiving information. Socket Error. Error Code: %d", WSAGetLastError());
            CloseAndCleanUpSocket(sfd);
            CloseAndCleanUpSocket(cfd);

            return -1;
        }

        else if (!strcmp(buffer, "recv")) 
        {
            printf("Sending messages to client");
            break; // start sending messages
        
        }

        printf("Client: %s\n", buffer);
    } 

    while (cfd != INVALID_SOCKET) 
    {
        char* message = malloc((MAX_LENGTH + 1) * sizeof(char));

        rline();
        printf("Established Connection With CLIENT.");
        printf("\n- SERVER IP Address: %s\n", inet_ntoa(server_info.sin_addr));

        printf("\nEnter data to send to client.\n");
        printf("Enter \"quit\" to break:\n");
        fgets(message, MAX_LENGTH, stdin); // Get message input to send to the client.

        // Check if message is equal to quit to terminate connection with the client
        if (!strcmp(message, "quit\n"))
        {
            rline();
            printf("No longer transmitting messages to client.");
            break;
        }

        // Send message to client
        size_t socketSend = send(cfd, message, strlen(message), 0);
        if (0 > socketSend)
        {
            printf("Send failed. Error Code: %d", WSAGetLastError());
            CloseAndCleanUpSocket(sfd);
            CloseAndCleanUpSocket(cfd);

            return -1;
        }
    }

    CloseAndCleanUpSocket(sfd);
    CloseAndCleanUpSocket(cfd);

    rline();
    printf("\nSocket closed.");

    return 0;
}