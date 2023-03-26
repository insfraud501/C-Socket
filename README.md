# C TCP Socket Program 

This is a simple client-server communication program written in C that allows two machines to send and receive data over a network using TCP/IP sockets.
## Requirements

- Windows operating system
- Windows Socket API (WinSock2)

## Notes
- This program is only usable in Windows as it uses the Windows Socket API.
- The maximum amount of bytes allowed to send and receive is 2048.

# Usage
## Client

To run the client program, use the following command-line arguments:

    client.exe -ip <ip_address> -p <port_number> -m <send_recv_mode>

- <ip_address>: The IP address of the machine running the server program.

- <port_number>: The port number used to communicate between the client and server.

- <send_recv_mode>: The mode to run the program in. Use "send" to send data to the server, and "recv" to receive data from the server.

## Server

To run the server program, open up a new command prompt and run these command-line arguments: 

    server.exe -ip <ip_address> -p <port_number>

- <ip_address>: The IP address of the machine running the server program.

- <port_number>: The port number used to communicate between the client and server.

# How it Works

The program uses TCP/IP sockets to establish a connection between the client and server machines. The server program creates a socket and binds it to a specified IP address and port number. It then listens for any incoming connections from a client. Once a connection is established, the server receives messages sent by the client and displays them on the console.

The client program connects to the server by specifying the server's IP address and port number. It then sends data to the server in "send" mode, or receives data from the server in "recv" mode.

If the client sends the message "quit", the connection with the server is closed and the sockets are cleaned up.
# Error Codes

The program returns the following error codes:

- -1: An error has occurred in the program. A WSA error code will be provided.
- -2: Invalid command-line arguments were provided.
- 0: The socket has been closed and the program has been cleaned up.
- 3: The client disconnected from the server. The server will be shut down.
