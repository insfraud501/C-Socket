# C-Socket
Client -> Server Socket made in C. Client sends information while the server receives information.

**This program is only useable in Windows as it relies on the Windows Socket API.**

# How to Use Server and Client.exe
After downloading, place Server.exe and Client.exe in a memorable path location

## Booting up Server.exe
Head to the command prompt by pressing Windows + R > cmd > Enter

In the command prompt, enter in this command.
    
    cd <the path you placed Server.exe in>

This will direct any input into the path you just entered in.
Next start up Server.exe with arguments in the command prompt. This part is important to get right.

    server.exe -ip <The ip of the machine that server.exe will be running on> -p <an open port on the current machine to listen on>
    
After this command, server.exe should be booted up and listening for any connections. If not, make sure the port and ip are entered correctly.

## Booting up Client.exe
Head to the command prompt again by pressing Windows + R > cmd > Enter

In the command prompt, enter in this command.
    
    cd <the path you placed Client.exe in>

Like before, this will direct any input into the path you just entered in.
Now start up Client.exe with arguments in the command prompt. This part is also important to get right.

    Client.exe -ip <The ip of the machine that Client.exe will be running on> -p <The port used passed through -p when booting up Server.exe>

After this, if everything is correct a connection should be established. 

Send messages to server by typing your messages and pressing enter.
Close the connection by typing in 'quit' in all lowercase and entering it in.

# The whole purpose of this program is..
	Client -> Server socket


Server.exe: open up a remote server and await a connection to establish with a client.

Client.exe: Search for remote server on a port. Find > Request connection > Connect.

In this program, the client connects to the remote server and once the connection is accepted BY the server, the client now has the ability to send messages to the server.


# Below are the steps used in the program

## On start Server.exe
-  Create Socket for client to connect to. 
-  Bind socket.
-  Listen for any possible connections in attempt to find the client looking for a signal.

### On start Client.exe
-  Create socket to connect to server.
-  Attempt to connect to server.

## Resume Server.exe
-  See attempt from client.exe to connect.
-  Accept the connection. 
-  Receive messages entered in by the client.

***If the client enters in "quit" into their prompt, the connection will be broken and the sockets will be closed.***

# Program Return Values
Return -1
- An Error has occured in the program. A WSA Error code will be provided.

Return 0
- The socket has been closed and the program has been cleaned up.

Return 3
- The client disconnected from the server. The server will be shut down.

Return -2
- An Error has occured in the programm. A WSA Error Code may not be provided.



