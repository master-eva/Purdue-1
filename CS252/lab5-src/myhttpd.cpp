#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <dirent.h>

const char * usage =
    "usage:                                                         \n"
    "                                                               \n"
    "myhttpd [-f|-t|-p] [<port>]                                    \n"
    "                                                               \n"
    "-f :  Create a new process for each request					\n"
    "-t : Create a new thread for each request                      \n"
    "-p :  Pool of threads						                    \n"
    "                                                               \n"
    "default port: 9826                                             \n"
    "                                                               \n";

const char * badrequest =
    "HTTP/1.0 400 Bad Request\r\n"
    "Server: CS 252 lab5\r\n"
    "Content-type: text/html\r\n"
    "\r\n"
    "<h1>400 Bad Request</h1>";

const char * filenotfound =
    "HTTP/1.0 404 File Not Found\r\n"
    "Server: CS 252 lab5\r\n"
    "Content-type: text/html\r\n"
    "\r\n"
    "<h1>404 File Not Found</h1>";

int QueueLength = 5;
const int MAX_BUFFER = 1024;

pthread_mutex_t mutex;

void processRequest(int);
void processHandler(int);
void poolHandler(int);
void zombieHandler(int);

int main( int argc, char **argv) {
	struct sigaction sa_child;
	sa_child.sa_handler = zombieHandler;
	sa_child.sa_flags = SA_RESTART;
	sigemptyset(&sa_child.sa_mask);
	if (sigaction(SIGCHLD, &sa_child, NULL) == -1) {
		perror ("sigchild action");
		exit(1);
	}

	int port;
	char option;

	// Get the port and options from the arguments
	if (argc == 1) {
		port = 9826; //by default
	} else if (argc == 2) {
		if (argv[1][0] == '-') {
			port = 9826;
			option = argv[1][1];
		} else {
			port = atoi( argv[1] );
		}
	} else if (argc == 3) {
		option = argv[1][1];
		port = atoi( argv[2] );
	} else {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}

	if (port <= 1024 || port >= 65536) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}


	struct sockaddr_in serverIPAddress;
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);

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

	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	struct sockaddr_in clientIPAddress;
	int addressLength = sizeof( clientIPAddress );
	int clientSocket;

	if (option == 'p') {
		// -p:  Pool of threads
		pthread_t tid[5];
		pthread_attr_t attr;

		pthread_attr_init( &attr );
		pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

		pthread_mutex_init(&mutex, NULL);

		for (int i = 0; i < 5; i++) {
			pthread_create(&tid[i], &attr, (void *(*)(void *))poolHandler, (void *)masterSocket);
		}

		for (int i = 0; i < 5; i++) {
			pthread_join(tid[i], NULL);
		}
	} else {
		while ( 1 ) {
			// get client socket
			clientSocket = accept( masterSocket, (struct sockaddr *)&clientIPAddress, (socklen_t*)&addressLength);

			if (option == 'f') {
				// -f :  Create a new process for each request
				if (fork() == 0) { // child
					processRequest( clientSocket );
					close( clientSocket );
					exit(0);
				}
				close( clientSocket );
			} else if ( option == 't' ) {
				// -t : Create a new thread for each request
				pthread_t tid;
				pthread_attr_t attr;

				pthread_attr_init( &attr );
				pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

				pthread_create(&tid, &attr, (void * (*)(void *))processHandler, (void *)clientSocket);
			} else if (option == 'p') {
				// skip the option have been processed
			} else {
				// process request
				processRequest( clientSocket );
				// close socket
				close(clientSocket);
			}
		}
	}
}

void zombieHandler(int sig)
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

void processHandler(int socket) {
	//printf("%d\n", pthread_self());
	processRequest(socket);
	close(socket);
}

void poolHandler(int masterSocket) {
	while ( 1 ) {
		struct sockaddr_in clientIPAddress;
		int addressLength = sizeof( clientIPAddress );
		int clientSocket;

		pthread_mutex_lock(&mutex);
		clientSocket = accept( masterSocket, (struct sockaddr *)&clientIPAddress, (socklen_t*)&addressLength);
		pthread_mutex_unlock(&mutex);


		processRequest(clientSocket);
		close(clientSocket);
	}
}

void processRequest(int socket) {
	char buffer[MAX_BUFFER];
	// initialize buffer
	memset( (void*)buffer, (int)'\0', MAX_BUFFER );

	// find serving directory, map documents
	char * dir = (char *) calloc (1, MAX_BUFFER);
	getcwd(dir, MAX_BUFFER);
	strcat(dir, "/http-root-dir");

	char * rootdir = strdup(dir);

	// read the request
	int result = read(socket, buffer, MAX_BUFFER - 1 );

	// find the first word of request
	char * request = buffer;
	char * firstSpace = strchr(request, ' ');
	if (firstSpace == NULL) {
		write( socket, badrequest, strlen(badrequest) );
		return;
	}
	*firstSpace = '\0';

	// find the second word of request
	char * document = firstSpace + 1;
	char * secondSpace = strchr(document, ' ');
	if (secondSpace == NULL) {
		write( socket, badrequest, strlen(badrequest) );
		return;
	}
	*secondSpace = '\0';

	// move the pointer to the third word
	char * protocol = secondSpace + 1;
	char * thirdSpace = strchr(protocol, '\n');
	*thirdSpace = '\0';

	if (strncmp(request, "GET", 3)) {
		write( socket, badrequest, strlen(badrequest) );
		return;
	}

	if (strncmp(protocol, "HTTP/1.0", 8) && strncmp(protocol, "HTTP/1.1", 8)) {
		write( socket, badrequest, strlen(badrequest) );
		return;
	}

	// get document path, add to dir
	char * docdir = (char *) calloc (1 , MAX_BUFFER);
	if (!strncmp(document, "/icons", strlen("/icons")) || !strncmp(document, "/htdocs", strlen("/htdocs"))
	        || !strncmp(document, "/cgi-bin", strlen("/cgi-bin"))) {
		strcpy(docdir, document);
	} else {
		if (!strcmp(document, "/")) {
			strcpy(docdir, "/htdocs/index.html");
		} else {
			strcpy(docdir, "/htdocs");
			strcat(docdir, document);
		}
	}
	strcat(dir, docdir);


	// find real path
	if (strstr(dir, "..") > 0)  {
		char * tempdir = (char *) calloc (1, MAX_BUFFER);
		tempdir = realpath(dir, NULL);

		if (strlen(tempdir) > strlen(rootdir)) {
			strcpy(dir, tempdir);
		} else {
			write( socket, filenotfound, strlen(filenotfound) );
			return;
		}
		free(tempdir);
	}

	char * temp;
	if (!strncmp(document, "/cgi-bin", strlen("/cgi-bin"))) {
		char * cwd = strdup(dir);
		char * temp = strchr(cwd, '?');
		if (temp) {
			*temp = '\0';
		}

		if (open(cwd, O_RDONLY)) {
			char ** args = (char **)calloc(1, sizeof(char *) * strlen(document));
			args[0] = (char *)calloc(1, sizeof(char) * MAX_BUFFER);
			args[1] = (char *)calloc(1, sizeof(char) * MAX_BUFFER);

			strcpy(args[0], cwd);

			temp = strchr(dir, '?');

			if (temp) {
				temp ++;
				strcpy(args[1], temp);
			}

			// write header
			const char * header =
			    "HTTP/1.0 200 Document follows\r\n"
			    "Server: CS 252 lab5\r\n";
			write(socket, header, strlen(header));

			// change socket to default out and save default
			int defaultout = dup(1);
			dup2(socket, 1);

			int ret = fork();
			if ( ret == 0 ) {
				setenv("REQUEST_METHOD", "GET", 1);
				setenv("QUERY_STRING", args[0], 1);

				execvp(cwd, args);

				exit(0);
			}
			dup2(defaultout, 1);
			close(defaultout);

			free(args[0]);
			free(args[1]);
			free(args);
		} else {
			// not found
			write(socket, filenotfound, strlen(filenotfound));
			return;
		}
	} else {
		// find content-type
		char * contentType = (char *) calloc (1, MAX_BUFFER);
		if (strstr(dir, ".html") > 0) {
			strcpy(contentType, "text/html");
		} else if (strstr(dir, ".jpg") > 0) {
			strcpy(contentType, "image/jpeg");
		} else if (strstr(dir, ".gif") > 0) {
			strcpy(contentType, "image/gif");
		} else {
			strcpy(contentType, "text/plain");
		}

		// use stat to find if it's a file or a directory
		struct stat path_stat;
		if (!stat(dir, &path_stat)) {
			// check if the file exists
			if (S_ISREG(path_stat.st_mode)) {
				int file = open(dir, O_RDONLY);
				if (file > 0) {
					// write header
					char * header = (char *) calloc (1, MAX_BUFFER);
					sprintf(header,
					        "HTTP/1.0 200 Document follows\r\n"
					        "Server: CS 252 lab5\r\n"
					        "Content-type: %s\r\n\r\n", contentType);
					write(socket, header, strlen(header));

					//write file
					char c;
					int res;
					while ( (res = read(file, &c, 1)) > 0 ) {
						write (socket, &c, 1);
					}
					free(header);
				}
			} else if (S_ISDIR(path_stat.st_mode)) {
				//printf("%s\n", dir);
				if (dir[strlen(dir) - 1] != '/') strcat(dir, "/");
				if (docdir[strlen(dir) - 1] != '/') strcat(docdir, "/");


				//writer header
				const char * header =
				    "HTTP/1.0 200 Document follows\r\n"
				    "Server: CS 252 lab5\r\n"
				    "Content-type: text/html\r\n\r\n";
				write(socket, header, strlen(header));

				write(socket, "<head><title>Index of ", 22);
				write(socket, dir, strlen(dir));
				write(socket, "</title></head>", 15);

				write(socket, "<head><h1>Index of ", 19);
				write(socket, dir, strlen(dir));
				write(socket, "</h1></head>", 12);

				write(socket, "<table width=\"400\">", 19);
				write(socket, "<tbody>", 7);

				write(socket, "<tr  align=\"left\">", 18);
				const char * tablehead =
				    "<th>Name</th><th>Last Modified</th><th>Size</th>";
				write(socket, tablehead, strlen(tablehead));
				write(socket, "</tr>", 5);

				struct dirent ** dirent;
				int count = scandir(dir, &dirent, 0, alphasort);

				for (int i = 0; i < count; i++) {
					write(socket, "<tr>", 4);
					if (dirent[i]->d_name[0] == '.') continue;


					char * href = (char *) calloc (1 , strlen(docdir) + strlen(dirent[i]->d_name));
					sprintf(href, "%s%s", docdir, dirent[i]->d_name);

					char * name = (char *) calloc (1 , strlen(dirent[i]->d_name) + strlen(href) + 30);
					sprintf(name, "<td><a href = \"%s\">%s</a></td>", href, dirent[i]->d_name);
					write(socket, name, strlen(name));

					char * path = (char *) calloc (1 , strlen(dir) + strlen(dirent[i]->d_name));
					sprintf(path, "%s%s", dir, dirent[i]->d_name);

					struct stat attr;
					stat(path, &attr);

					char * modifyTime = (char *) calloc (1 , 100);
					sprintf(modifyTime , "%s", ctime(&attr.st_mtime));
					write(socket, "<td>", 4);
					write(socket, modifyTime, strlen(modifyTime));
					write(socket, "</td>", 5);


					char * size = (char *) calloc (1 , 100);
					sprintf(size , "%ld", attr.st_size);
					write(socket, "<td>", 4);
					write(socket, size, strlen(size));
					write(socket, "</td>", 5);

					write(socket, "</tr>", 5);
					free(name);
					free(path);
					free(modifyTime);
					free(size);
				}

				dirent = NULL;

				write(socket, "</tbody>", 8);
				write(socket, "</table>", 8);
			}
			free(contentType);
			free(dir);
			free(docdir);
		} else {
			// file not found
			write( socket, filenotfound, strlen(filenotfound) );
		}
	}
}