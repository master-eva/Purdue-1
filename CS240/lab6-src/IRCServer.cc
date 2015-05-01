
const char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                          \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
"                                                               \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "IRCServer.h"
#include "HashTableVoid.h"
#include "LinkedList.h"

#define PASSWORD_FILE "password.txt"
#define MAX_MSG 100

FILE *file;

int QueueLength = 5;

HashTableVoid passwords;
HashTableVoid rooms;
HashTableVoid users;
HashTableVoid messages;
LinkedList * alluser;
LinkedList * allroom;

int IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);
  
	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1; 
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
			     (char *) &optval, sizeof( int ) );
	
	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			  (struct sockaddr *)&serverIPAddress,
			  sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}
	
	// Put socket in listening mode and set the 
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

void IRCServer::runServer(int port) {
	int masterSocket = open_server_socket(port);

	initialize();
	
	while ( 1 ) {
		
		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
					  (struct sockaddr *)&clientIPAddress,
					  (socklen_t*)&alen);
		
		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}
		
		// Process request.
		processRequest( slaveSocket );		
	}
}

int main( int argc, char ** argv ) {
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}
	
	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;

	// It will never return
	ircServer.runServer(port);
	
}

//
// Commands:
//   Commands are started by the client.
//
//   Request: ADD-USER <USER> <PASSWD>\r\n
//   Answer: OK\r\n or DENIED\r\n
//
//   REQUEST: GET-ALL-USERS <USER> <PASSWD>\r\n
//   Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//
//   REQUEST: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LIST-ROOMS <USER> <PASSWD>\r\n
//   Answer: room1\r\n
//           room2\r\n
//           ...
//           \r\n
//
//   Request: ENTER-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LEAVE-ROOM <USER> <PASSWD>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
//   Answer: OK\n or DENIED\n
//
//   Request: GET-MESSAGES <USER> <PASSWD> <LAST-MESSAGE-NUM> <ROOM>\r\n
//   Answer: MSGNUM1 USER1 MESSAGE1\r\n
//           MSGNUM2 USER2 MESSAGE2\r\n
//           MSGNUM3 USER2 MESSAGE2\r\n
//           ...\r\n
//           \r\n
//
//    REQUEST: GET-USERS-IN-ROOM <USER> <PASSWD> <ROOM>\r\n
//    Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//

void IRCServer::processRequest( int fd ) {
	// Buffer used to store the comand received from the client
	const int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;
	
	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;
	
	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&
		read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}
		
		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}
	
	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;

	printf("RECEIVED: %s\n", commandLine);

	// printf("The commandLine has the following format:\n");
	// printf("COMMAND <user> <password> <arguments>. See below.\n");
	// printf("You need to separate the commandLine into those components\n");
	// printf("For now, command, user, and password are hardwired.\n");

	char * c = strdup(commandLine);
	char * word = (char *)malloc(100 * sizeof(char));
	char * p = word;

	while (*c != ' ' && *c != '\0') {
		*p = *c;
		p++;
		c++;
	}
	while (*c == ' ') c++;
	*p = '\0';
	const char * command = strdup(word);

	p = word;
	while (*c != ' ' && *c != '\0') {
		*p = *c;
		p++;
		c++;
	}
	while (*c == ' ') c++;
	*p = '\0';
	const char * user = strdup(word);

	p = word;
	while (*c != ' ' && *c != '\0') {
		*p = *c;
		p++;
		c++;
	}
	while (*c == ' ') c++;
	*p = '\0';
	const char * password = strdup(word);

	const char * args = strdup(c);

	free(word);
	word = NULL;

	// const char * command = "ADD-USER";
	// const char * user = "peter";
	// const char * password = "spider";
	// const char * args = "";

	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "CREATE-ROOM")) {
		createRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LIST-ROOMS")) {
		listRooms(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else {
		const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	close(fd);	
}

void IRCServer::initialize() {
	file = fopen(PASSWORD_FILE, "w+");
	fclose(file);
	alluser = new LinkedList();
	allroom = new LinkedList();
}

bool IRCServer::checkPassword(int fd, const char * user, const char * password) {
	void * p;
	bool e;
	e = passwords.find(user,&p);
	if (!e) return e;

	char * c = (char *) p;
	return strcmp(password, c) == 0;
}

void IRCServer::addUser(int fd, const char * user, const char * password, const char * args) {
	void * p;
	if (passwords.find(user,&p) || strcmp(password,"") == 0) {
		const char * msg =  "DENIED\r\n";
		write(fd, msg, strlen(msg));
	} else {
		(*alluser).add(user);
		passwords.insertItem(user, (void *) password);

		HashTableVoidIterator iterator(&passwords);
		const char * user;
		void * p;

		file = fopen(PASSWORD_FILE, "w+");

		while (iterator.next(user, p)) {
			char * s = (char *) p;
			fprintf(file, "%s %s\r\n", user, s);
		}
		fclose(file);
		
		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::createRoom(int fd, const char * user, const char * password, const char * args) {
	void * p;
	char * room = strdup(args);
	if (!checkPassword(fd,user,password)) {
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}
	if (rooms.find(room,&p)) {
		const char * msg =  "ERROR (No room)\r\n";
		write(fd, msg, strlen(msg));
	} else {
		rooms.insertItem(room,(void *) 0);
		(*allroom).add(room);

		LinkedList * usersInRoom = new LinkedList();
		LinkedList * messagesInRoom = new LinkedList();
		
		users.insertItem(room, (void *) usersInRoom);
		messages.insertItem(room, (void *) messagesInRoom);
		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::listRooms(int fd, const char * user, const char * password, const char * args) {
	if (!checkPassword(fd,user,password)) {
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	} else {
		(*allroom).sort();
		ListNode * e = allroom -> head;
		while (e != NULL) {
			char * u = strdup(e -> value);
			strcat(u,"\r\n");
			write(fd, u, strlen(u));
			e = e -> next;
		}

		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::enterRoom(int fd, const char * user, const char * password, const char * args) {
	void * p;
	char * room = strdup(args);
	
	if (!checkPassword(fd,user,password)) {
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}

	if (!users.find(room,&p)) {
		const char * msg =  "ERROR (No room)\r\n";
		write(fd, msg, strlen(msg));
	} else {
		LinkedList * usersInRoom =  (LinkedList *)p;
		if ((*usersInRoom).exists(user)) {
			const char * msg =  "OK\r\n";
			write(fd, msg, strlen(msg));
			return;
		}
		(*usersInRoom).add(user);

		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::leaveRoom(int fd, const char * user, const char * password, const char * args) {
	void * p;
	char * room = strdup(args);
	if (!checkPassword(fd,user,password)) {
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}

	if (!users.find(room,&p)) {
		const char * msg =  "ERROR (No room)\r\n";
		write(fd, msg, strlen(msg));
		return;
	} else {
		LinkedList * usersInRoom =  (LinkedList *)p;
		if (!(*usersInRoom).exists(user)) {
			const char * msg =  "ERROR (No user in room)\r\n";
			write(fd, msg, strlen(msg));
			return;
		}
		
		(*usersInRoom).remove(user);

		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::sendMessage(int fd, const char * user, const char * password, const char * args) {
	void * p;

	if (!checkPassword(fd,user,password)) {
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}
	
	char * c = strdup(args);
	char * word = (char *)malloc(100 * sizeof(char));
	char * q = word;

	while (*c != ' ' && *c != '\0') {
		*q = *c;
		q++;
		c++;
	}
	while (*c == ' ') c++;
	*q = '\0';
	char * room = strdup(word);
	
	free(word);
	word = NULL;

	if (!users.find(room,&p)) {
		const char * msg =  "ERROR (No room)\r\n";
		write(fd, msg, strlen(msg));
	} else {
		LinkedList * usersInRoom =  (LinkedList *)p;
		if (!(*usersInRoom).exists(user)) {
			const char * msg =  "ERROR (user not in room)\r\n";
			write(fd, msg, strlen(msg));
			return;
		}
		messages.find(room,&p);
		LinkedList * messagesInRoom = (LinkedList *)p;

		rooms.find(room,&p);
		long n = (long) p + 1;
		if (n > MAX_MSG) (*messagesInRoom).removeHead();
		rooms.insertItem(room, (void *) n);

		char * temp = (char *)malloc(100 * sizeof(char));
		sprintf(temp,"%ld %s %s", n - 1, user, c);
		const char * msgToSend = strdup(temp);
		(*messagesInRoom).add(msgToSend);

		free(temp);
		temp = NULL;

		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::getMessages(int fd, const char * user, const char * password, const char * args) {
	void * p;

	if (!checkPassword(fd,user,password)) {
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}

	char * c = strdup(args);
	char * word = (char *)malloc(20 * sizeof(char));
	char * q = word;

	while (*c != ' ' && *c != '\0') {
		*q = *c;
		q++;
		c++;
	}
	while (*c == ' ') c++;
	*q = '\0';

	char * room = strdup(c);

	rooms.find(room,&p);
	int n = (long) p;
	int nums = atoi(word);

	free(word);
	word = NULL;
	
	if (n > MAX_MSG) nums = nums - (n - MAX_MSG);
	if (nums < 0) nums = 0;

	if (!users.find(room,&p)) {
		const char * msg =  "ERROR (No room)\r\n";
		write(fd, msg, strlen(msg));
	} else {
		LinkedList * usersInRoom =  (LinkedList *)p;
		if (!(*usersInRoom).exists(user)) {
			const char * msg =  "ERROR (User not in room)\r\n";
			write(fd, msg, strlen(msg));
			return;
		}

		if (nums >= n) {
			const char * msg =  "NO-NEW-MESSAGES\r\n";
			write(fd, msg, strlen(msg));
			return;
		}

		int i = 0;
		rooms.find(room,&p);
		long n = (long) p;

		messages.find(room,&p);
		LinkedList * messagesInRoom = (LinkedList *)p;
		ListNode * e = messagesInRoom -> head;
		while (e != NULL && i < nums) {
			e = e -> next;
			i++;
		}

		while (e != NULL) {
			char * m = strdup(e -> value);
			strcat(m,"\r\n");
			write(fd, m, strlen(m));
			e = e -> next;
		}
	}

	const char * msg =  "\r\n";
	write(fd, msg, strlen(msg));
}

void IRCServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args) {
	void * p;
	char * room = strdup(args);	

	if (!checkPassword(fd,user,password)) {
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}
	
	if (!users.find(room,&p)) {
		const char * msg =  "ERROR (No room)\r\n";
		write(fd, msg, strlen(msg));
	} else {
		LinkedList * usersInRoom =  (LinkedList *)p;
		(*usersInRoom).sort();
		ListNode * e = usersInRoom -> head;
		while (e != NULL) {
			char * u = strdup(e -> value);
			strcat(u,"\r\n");
			write(fd, u, strlen(u));
			e = e -> next;
		}

		const char * msg =  "\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::getAllUsers(int fd, const char * user, const char * password,const  char * args) {
	if (!checkPassword(fd,user,password)) {
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	} else {
		(*alluser).sort();
		ListNode * e = alluser -> head;
		while (e != NULL) {
			char * u = strdup(e -> value);
			strcat(u,"\r\n");
			write(fd, u, strlen(u));
			e = e -> next;
		}
	}
	
	const char * msg =  "\r\n";
	write(fd, msg, strlen(msg));

}

