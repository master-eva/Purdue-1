
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%token <string_val> WORD

%token NOTOKEN GREAT NEWLINE GREATGREAT GREATAMPERSAND GREATGREATAMPERSAND PIPE AMPERSAND LESS

%union	{
	char   *string_val;
}

%{
//#define yylex yylex
#define MAXFILENAME 1024
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

extern FILE * yyin;
extern int yylex(void);
extern int yyparse(void);

void yyerror(const char * s);
int yylex();
void expandWildcards(char * prefix, char *suffix);
%}
%%

goal:
commands
;

commands:
command
| commands command
;

command: simple_command
;

simple_command:
pipe_list iomodifier_list background_optional NEWLINE {
	//printf("   Yacc: Execute command\n");
	Command::_currentCommand.execute();
}
| NEWLINE
| /* can be empty */
| error NEWLINE { yyerrok; }
;

command_and_args:
command_word argument_list {
	Command::_currentCommand.
	insertSimpleCommand( Command::_currentSimpleCommand );
}
;

argument_list:
argument_list argument
| /* can be empty */
;

argument:
WORD {
	if ( !strcmp($1, "${?}") || (strchr($1, '*') == NULL && strchr($1, '?') == NULL)) {
		Command::_currentSimpleCommand->insertArgument( $1 );
	}
	else {
		char * empty = (char *) malloc(1);
		*empty = '\0';
		expandWildcards(empty, $1);
	}
	//printf("   Yacc: insert argument \"%s\"\n", $1);
	//Command::_currentSimpleCommand->insertArgument( $1 );
}
;

command_word:
WORD {
	//printf("   Yacc: insert command \"%s\"\n", $1);

	Command::_currentSimpleCommand = new SimpleCommand();
	Command::_currentSimpleCommand->insertArgument( $1 );
}
;

pipe_list:
pipe_list PIPE command_and_args
| command_and_args
;

background_optional:
AMPERSAND {
	//printf("   Yacc: run in background \n");
	Command::_currentCommand._background = 1;
}
|/* can be empty */
;

iomodifier_list:
iomodifier_list iomodifier_opt
|/* can be empty */
;

iomodifier_opt:
GREAT WORD {
	if (Command::_currentCommand._outFile) {
		printf("Ambiguous output redirect\n");
		exit(1);
	}

	//printf("   Yacc: insert output \"%s\"\n", $2);
	Command::_currentCommand._outFile = $2;
}
| GREATGREAT WORD {
	if (Command::_currentCommand._outFile) {
		printf("Ambiguous output redirect\n");
		exit(1);
	}

	//printf("   Yacc: insert output \"%s\"\n", $2);
	Command::_currentCommand._append = 1;
	Command::_currentCommand._outFile = $2;
}
| GREATGREATAMPERSAND WORD {
	if (Command::_currentCommand._outFile) {
		printf("Ambiguous output redirect\n");
		exit(1);
	}
	if (Command::_currentCommand._errFile) {
		printf("Ambiguous error redirect\n");
		exit(1);
	}

	Command::_currentCommand._append = 1;
	//printf("   Yacc: insert output \"%s\"\n", $2);
	Command::_currentCommand._outFile = $2;
	//printf("   Yacc: insert error \"%s\"\n", $2);
	Command::_currentCommand._errFile = $2;

}
| GREATAMPERSAND WORD {
	if (Command::_currentCommand._outFile) {
		printf("Ambiguous output redirect\n");
		exit(1);
	}
	if (Command::_currentCommand._errFile) {
		printf("Ambiguous error redirect\n");
		exit(1);
	}

	//printf("   Yacc: insert output \"%s\"\n", $2);
	Command::_currentCommand._outFile = $2;
	//printf("   Yacc: insert error \"%s\"\n", $2);
	Command::_currentCommand._errFile = $2;
}
| LESS WORD {
	if (Command::_currentCommand._inFile) {
		printf("Ambiguous input redirect\n");
		exit(1);
	}

	//printf("   Yacc: insert input \"%s\"\n", $2);
	Command::_currentCommand._inFile = $2;
}


%%

void
yyerror(const char * s) {
	fprintf(stderr, "%s", s);
}


void
expandWildcards(char * prefix, char * suffix) {
	// we have done the expansion, add the arguement
	if (suffix[0] == 0) {
		//printf("%s\n", prefix);
		Command::_currentSimpleCommand->insertArgument(strdup(++prefix));
		return;
	}

	// get tilde
	char * s = strchr(suffix, '/');
	char * component = (char *) calloc (0, 1024 * sizeof(char));

	if ( s == NULL ) {
		// if there's no '/', we copy the whole suffix
		strcpy(component, suffix);
		suffix = suffix + strlen(suffix);
	} else {
		//copy the next component
		strncpy(component, suffix, s - suffix);
		suffix = s + 1;
	}

	// expand the component
	char * newPrefix = (char *) calloc (0, 1024 * sizeof(char));

	if (strchr(component, '*') == NULL && strchr(component, '?') == NULL) {
		//if the component doesn't include a wildcard.
		sprintf(newPrefix, "%s/%s", prefix, component);
		expandWildcards(newPrefix, suffix);
		return;
	}

	// convert wildcard to regex
	char * reg = (char *) calloc(0, 2 * strlen(component) + 20);
	char * a = component;
	char * r = reg;

	// start of regex
	*r = '^';
	r++;
	while (*a) {
		if (*a == '*') {
			// "*" -> ".*"
			*r = '.';
			r++;
			*r = '*';
			r++;
		} else if (*a == '?') {
			// "?" -> "."
			*r = '.';
			r++;
		} else if (*a == '.') {
			// "." -> "\."
			*r = '\\';
			r++;
			*r = '.';
			r++;
		} else {
			// else, copy the char
			*r = *a;
			r++;
		}
		a++;
	}
	// end of regex
	*r = '$';
	r++;
	*r = '\0';

	// printf("%s\n", reg);
	// compile the regex
	regex_t regex;
	if ( regcomp(&regex, reg, 0) ) {
		perror("regex failed to compile");
		exit(1);
	}

	// find the entries match the regex
	struct dirent ** dirent;
	int count;

	if (prefix[0] == 0) {
		// scan the current directory
		count = scandir(".", &dirent, 0, alphasort);
	} else {
		// scan the directory of prefix
		// printf("%s\n",prefix);
		count = scandir(prefix, &dirent, 0, alphasort);
	}

	if (count > 0) {
		// if it's not, loop over the directory to find the thing that match
		int i;
		for (i = 0; i < count; i++) {
			if (!regexec(&regex, dirent[i]->d_name, 0, NULL, 0)) {
				if (component[0] == '.') {
					// check if we can find hidden files
					sprintf(newPrefix, "%s/%s", prefix, dirent[i]->d_name);
					expandWildcards(newPrefix, suffix);
				} else if (dirent[i]->d_name[0] != '.') {
					sprintf(newPrefix, "%s/%s", prefix, dirent[i]->d_name);
					expandWildcards(newPrefix, suffix);
				}
			}
		}
	}
}
#if 0
main()
{
	yyparse();
}
#endif
