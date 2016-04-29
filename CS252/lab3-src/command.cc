
/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <regex.h>
#include <pwd.h>

#include "command.h"
#include "y.tab.h"
#include "lex.yy.h"

char * shelldir;
int max_backgroundpid = 2;
int index_backgroundpid = 0;
int * backgroundpid = (int *) malloc (max_backgroundpid * sizeof(int));
int * backgroundcmd = (int *) malloc (max_backgroundpid * sizeof(int));
int code;
extern char ** history;
extern int history_length;

SimpleCommand::SimpleCommand()
{
	// Create available space for 5 arguments
	_numOfAvailableArguments = 5;
	_numOfArguments = 0;
	_arguments = (char **) malloc( _numOfAvailableArguments * sizeof( char * ) );
}

void
SimpleCommand::insertArgument( char * argument )
{
	if ( _numOfAvailableArguments == _numOfArguments  + 1 ) {
		// Double the available space
		_numOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments, _numOfAvailableArguments * sizeof( char * ) );
	}

	// check ${}, environment variable expansion
	const char * pattern = "\\${.*}";
	regex_t regex;
	regmatch_t match;
	if ( regcomp(&regex, pattern, 0) ) {
		perror("regex failed to compile");
		exit(1);
	}

	if ( !regexec(&regex, argument, 1, &match, 0) ) {
		char * expandedArg = (char*)malloc(1024 * sizeof(char));
		expandedArg[0] = '\0';

		int i = 0;
		int x = 0;
		while (argument[i] != 0 && i < 1024 ) {
			if ( argument[i] != '$' ) {
				// copy the char
				expandedArg[x] = argument[i];
				expandedArg[x + 1] = '\0';
				i++;
				x++;
			} else {
				// argument[i] == '$', expand the variable
				char * start = strchr((char*)(argument + i), '{');
				char * end = strchr((char*)(argument + i), '}');

				// take the env variable out
				char * var = (char*) calloc(0, (end - start) * sizeof(char));
				strncat(var, start + 1, end - start - 1);
				char * value = (char*) calloc(0, 1024 * sizeof(char));
				value[0] = '\0';

				// get the env, also expand special ones
				if (!strcmp(var, "$")) {
					sprintf(value, "%d", getpid());
				} else if (!strcmp(var, "?")) {
					sprintf(value, "%d", code);
				} else if (!strcmp(var, "!")) {
					if (index_backgroundpid > 0)
						sprintf(value, "%d", backgroundpid[index_backgroundpid - 1]);
				} else if (!strcmp(var, "_")) {
					if (history_length > 1)
						strcat(value, history[history_length - 2]);
				} else if (!strcmp(var, "SHELL")) {
					free(value);
					value = strdup(shelldir);
				} else {
					if (getenv(var) != NULL)
						sprintf(value, "%s", getenv(var));
				}

				// set the pointers to the new position
				strcat(expandedArg, value);
				i += strlen(var) + 3;
				x += strlen(value);

				free(var);
				free(value);
			}
		}
		argument = expandedArg;
	}

	regfree(&regex);

	if (argument[0] == '~') {
		char * user;
		if (strchr(argument, '/') == NULL) {
			user = strdup(++argument);
			argument--;
		} else {
			char * end = strchr(argument, '/');
			char * start = strchr(argument, '~');
			user = (char *) calloc(0, (end - start) * sizeof(char));
			strncat(user, start + 1, end - start - 1);
		}

		struct passwd *pw;
		if (!strcmp(user, "")) {
			pw = getpwuid(getuid());
		} else {
			pw = getpwnam(user);
		}

		const char *homedir = pw->pw_dir;

		char * expandedArg = (char *)malloc((strlen(homedir) + strlen(argument)) * sizeof(char));
		expandedArg[0] = '\0';
		strcat( expandedArg, homedir );
		strcat( expandedArg, "/" );
		strcat( expandedArg, (char *)(argument + 1 + strlen(user)) );

		argument = expandedArg;
	}

	_arguments[ _numOfArguments ] = argument;
	// Add NULL argument at the end
	_arguments[ _numOfArguments + 1] = NULL;

	_numOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
	                  malloc( _numOfSimpleCommands * sizeof( SimpleCommand * ) );

	_numOfSimpleCommands = 0;
	_outFile = 0;
	_append = 0;
	_inFile = 0;
	_errFile = 0;
	_background = 0;
}

void
Command::insertSimpleCommand( SimpleCommand * simpleCommand )
{
	if ( _numOfAvailableSimpleCommands == _numOfSimpleCommands ) {
		_numOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **) realloc( _simpleCommands,
		                  _numOfAvailableSimpleCommands * sizeof( SimpleCommand * ) );
	}

	_simpleCommands[ _numOfSimpleCommands ] = simpleCommand;
	_numOfSimpleCommands++;
}

void
Command::clear()
{
	for ( int i = 0; i < _numOfSimpleCommands; i++ ) {
		for ( int j = 0; j < _simpleCommands[ i ]->_numOfArguments; j ++ ) {
			free ( _simpleCommands[ i ]->_arguments[ j ] );
		}

		free ( _simpleCommands[ i ]->_arguments );
		free ( _simpleCommands[ i ] );
	}

	if ( _inFile ) {
		free( _inFile );
	}


	if ( _outFile == _errFile) {
		free( _outFile );
	} else {
		if ( _outFile ) {
			free( _outFile );
		}

		if ( _errFile ) {
			free( _errFile );
		}
	}
	_numOfSimpleCommands = 0;
	_outFile = 0;
	_inFile = 0;
	_errFile = 0;
	_background = 0;
}

void
Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");

	for ( int i = 0; i < _numOfSimpleCommands; i++ ) {
		printf("  %-3d ", i );
		for ( int j = 0; j < _simpleCommands[i]->_numOfArguments; j++ ) {
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[ j ] );
		}
	}

	printf( "\n\n" );
	printf( "  Output       Input        Error        Background\n" );
	printf( "  ------------ ------------ ------------ ------------\n" );
	printf( "  %-12s %-12s %-12s %-12s\n", _outFile ? _outFile : "default",
	        _inFile ? _inFile : "default", _errFile ? _errFile : "default",
	        _background ? "YES" : "NO");
	printf( "\n\n" );

}

void
Command::execute()
{
	// Don't do anything if there are no simple commands
	if ( _numOfSimpleCommands == 0) {
		prompt();
		return;
	}

	// Print contents of Command data structure
	//print();

	// Add execution here
	// For every simple command fork a new process
	// Setup i/o redirection
	// and call exec

	// default in, out and error
	int defaultin = dup(0);
	int defaultout = dup(1);
	int defaulterror = dup(2);

	// set up files
	int fdin;
	int fdout;
	int fderr;
	int fdpipe[2];

	// setup the input file
	if (_inFile) {
		fdin = open(_inFile, O_RDONLY, 660);
	} else {
		fdin = dup(defaultin);
	}

	// setup the error file
	if (_errFile) {
		if (_append) {
			fderr = open(_errFile, O_WRONLY | O_APPEND | O_CREAT, 0660);
		} else {
			fderr = open(_errFile, O_WRONLY | O_TRUNC | O_CREAT, 0660);
		}
	} else {
		fderr = dup(defaulterror);
	}

	int ret;
	for ( int i = 0; i < _numOfSimpleCommands; i++) {
		// redirect and close useless files
		dup2(fdin, 0);
		dup2(fderr, 2);
		close(fdin);
		close(fderr);

		// setup output for this command
		if (i == _numOfSimpleCommands - 1) {
			// set output file if it's the last command
			if (_outFile) {
				if (_append) {
					fdout = open(_outFile, O_WRONLY | O_APPEND | O_CREAT, 0660);
				} else {
					fdout = open(_outFile, O_WRONLY | O_TRUNC | O_CREAT, 0660);
				}
			} else {
				fdout = dup(defaultout);
			}
		} else {
			// pipe if it's not the last command
			pipe(fdpipe);
			fdin = fdpipe[0];
			fdout = fdpipe[1];
		}
		// redirect and close the file
		dup2(fdout, 1);
		close(fdout);

		// check "exit" command
		if ( !strcmp( _simpleCommands[i]->_arguments[0], "exit" ) || !strcmp( _simpleCommands[i]->_arguments[0], "eixt" )) {
			exit(1);
		}
		// check "setenv" command
		if ( !strcmp(_simpleCommands[i]->_arguments[0], "setenv") ) {
			ret = setenv(_simpleCommands[i]->_arguments[1], _simpleCommands[i]->_arguments[2], 1);
			if (ret < 0)
				perror("setenv");
			clear();
			prompt();
			return;
		}
		// check "unsetenv"
		if ( !strcmp(_simpleCommands[i]->_arguments[0], "unsetenv") ) {
			ret = unsetenv(_simpleCommands[i]->_arguments[1]);
			if (ret < 0)
				perror("unsetenv");
			clear();
			prompt();
			return;
		}
		// check "cd"
		if ( !strcmp(_simpleCommands[i]->_arguments[0], "cd") ) {
			int ret;
			if ( _simpleCommands[i]->_numOfArguments > 1 ) {
				ret = chdir( _simpleCommands[i]->_arguments[1] );
			} else {
				ret = chdir( getenv("HOME") );
			}
			if (ret < 0) {
				perror("cd");
			}

			clear();
			prompt();
			return;
		}

		// check "source"
		if ( !strcmp(_simpleCommands[i]->_arguments[0], "source") ) {
			FILE * myin = fopen(_simpleCommands[i]->_arguments[1], "r");
			if (myin == NULL) {
				clear();
				prompt();
				return;
			}
			yyin = myin;
			clear();
			fflush(stdout);
			yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
			yyparse();
			yypop_buffer_state();
			yyparse();
			prompt();
			return;
		}


		// check "jobs"
		if ( !strcmp(_simpleCommands[i]->_arguments[0], "jobs") ) {
			int i;
			for (i = 0; i < index_backgroundpid; i++)
				dprintf(1, "[%d]%d\t%s\n", i + 1, backgroundpid[i], history[backgroundcmd[i]]);
			clear();
			prompt();
			return;
		}

		ret = fork();
		if (ret == 0) {
			close(defaultin);
			close(defaultout);
			close(defaulterror);
			close(fdin);
			close(fdout);
			close(fderr);
			// in child process, check "printenv"
			if (!strcmp( _simpleCommands[i]->_arguments[0], "printenv" ))
			{
				char **p = environ;
				while (*p != NULL) {
					dprintf(1, "%s\n", *p);
					p++;
				}
				exit(0);
			}

			// child process, excute
			execvp(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_arguments);
			perror("execvp");
			_exit(1);
		} else if (ret < 0) {
			perror("fork");
			return;
		} // if it's a parent process, continue
	}

	// check if the process is a background command
	if (!_background) {
		int status;
		waitpid(ret, &status, 0);
		if (WIFEXITED(status)) {
			code = WEXITSTATUS(status);
			if (code != 0) {
				char * value = getenv("ON_ERROR");
				if (!(value == NULL || !strcmp(value, "")))
					printf("%s\n", value);
			}
		}
	} else {
		if (index_backgroundpid == max_backgroundpid) {
			max_backgroundpid *= 2;
			backgroundpid = (int *) realloc (backgroundpid, max_backgroundpid * sizeof(int));
			backgroundcmd = (int *) realloc (backgroundpid, max_backgroundpid * sizeof(int));
		}
		backgroundpid[index_backgroundpid++] = ret;
		backgroundcmd[index_backgroundpid] = history_length - 1;
	}

// restore the default input, output and error
	dup2(defaultin, 0);
	dup2(defaultout, 1);
	dup2(defaulterror, 2);
	close(defaultin);
	close(defaultout);
	close(defaulterror);

// Clear to prepare for next command
	clear();

// Print new prompt
	prompt();

}

// Shell implementation

void
Command::prompt()
{
	if (isatty(0)) {
		char * value = getenv("PROMPT");
		if (value == NULL || !strcmp(value, ""))
			printf("myshell>");
		else
			printf("%s", value);
		fflush(stdout);

	}
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

void sigintHandler(int sig) {
	printf("\n");
	Command::_currentCommand.clear();
	Command::_currentCommand.prompt();
}

void zombieHandler(int sig)
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

main()
{
	// handle SIGINT, ctrl-c
	/*
	struct sigaction sa_int;
	sa_int.sa_handler = sigintHandler;
	sa_int.sa_flags = SA_RESTART; //restart any interrupted system calls
	sigemptyset(&sa_int.sa_mask);
	if (sigaction(SIGINT, &sa_int, NULL) == -1) {
		perror("sigint action");
		exit(1);
	}
	*/
	// handle SIGCHILD, zombie process
	struct sigaction sa_child;
	sa_child.sa_handler = zombieHandler;
	sa_child.sa_flags = SA_RESTART;
	sigemptyset(&sa_child.sa_mask);
	if (sigaction(SIGCHLD, &sa_child, NULL) == -1) {
		perror ("sigchild action");
		exit(1);
	}

	shelldir = (char *) malloc(1024);
	int len = readlink("/proc/self/exe", shelldir, 1024);
	if (len > 0) shelldir[len] = '\0';

	yyin = fopen(".shellrc", "r");
	if (yyin > 0) {
		yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
		yyparse();
		yyin = stdin;
		Command::_currentCommand.clear();
		Command::_currentCommand.prompt();
		yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
		yyparse();
	} else {
		yyin = NULL;
		Command::_currentCommand.prompt();
		yyparse();
	}
}

