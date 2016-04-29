
/*
 *
 * CS-252 Fall 2013
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "read-line.h"
#include "y.tab.h"

static void yyunput (int c,char *buf_ptr  );
extern char* shelldir;


void myunputc(int c) {
    unput(c);
}

int mygetc(FILE * f) {
  static char *p;
  char ch;

  if (!isatty(0)) {
    return getc(f);
  }

  if (p==NULL || *p == 0) {
    char * s = read_line();
    p = s;
  }

  ch = *p;
  p++;

  return ch;
}

#undef getc
#define getc(f) mygetc(f)
%}

%option header-file="lex.yy.h"

%%

\n 	{
		return NEWLINE;
	}

[ \t] 	{
		/* Discard spaces and tabs */
     	}	

">" 	{
		return GREAT;
	}
">>"	{
		return GREATGREAT;
	}
">&"	{
		return GREATAMPERSAND;
	}
">>&"	{
		return GREATGREATAMPERSAND;
	}
"<"	{
		return LESS;
	}
"|"	{
		return PIPE;
	}
"&" 	{
		return AMPERSAND;
	}
\`[^\n\`]*\` {
		// subshell
		char * command = strdup(yytext + 1); 	// drop the  `
		command[ strlen(command) - 1] = '\0';

		// store defaultin and out
		int defaultin = dup(0);
		int defaultout = dup(1);

		// creat pipes
		int childpipe[2];
		int parentpipe[2];
		pipe(childpipe);
		pipe(parentpipe);

		// buffer
		int max = 5;
		int index = 0;
		char * buffer = (char *) calloc(0, max * sizeof(char));
		char c;

		int ret = fork();
		if (ret == 0) {
			write(parentpipe[1], command, strlen(command));
			write(parentpipe[1], "\nexit\n", 6);

			close(parentpipe[1]);
			close(childpipe[0]);

			dup2(parentpipe[0], 0);
			dup2(childpipe[1], 1);

			execv(shelldir, NULL);
			perror("subshell");
			_exit(0);
		} else if (ret > 0) {
			//wait till child finish
			
			close(parentpipe[0]);
			close(parentpipe[1]);
			close(childpipe[1]);

			waitpid(ret, NULL, 0);

			while(read(childpipe[0], &c, 1) > 0) {
				buffer[index] = c;
				if(index + 1 > max) {
					max *= 2;
					buffer = (char*)realloc(buffer, (max * sizeof(char*)));
				}
				index++;
			}
			buffer[index] = '\0';
			
			close(childpipe[0]);
			dup2(defaultin, 0);
			dup2(defaultout, 1);
			close(defaultin);
			close(defaultout);

			int i;
			for (i = index; i > -1 ; i--) {
				if (buffer[i] == '\n') buffer[i] = ' ';
				unput(buffer[i]);
			}
		} else {
			perror("fork");
			_exit(1);
		}
	}
[^ \t\n&<>\|]*\\[^ \t\n]* {
		//escape
		char * newArg = (char*)malloc(strlen(yytext) + 1);
		int x = 0;
		for (int i = 0; i < strlen(yytext); i++) {
			if (yytext[i] == '\\') 
				i++;
			newArg[x] = yytext[i];
			x++;
		
		}
		newArg[x] = '\0'; 
		yylval.string_val = strdup(newArg);
    	return WORD;
}
\"[^\n\"]*\"  {
		//quote
    	yytext = (char *) yytext + 1; //drop the first quote
    	yytext[ strlen(yytext) - 1] = '\0'; // drop the last quote
    	yylval.string_val = strdup(yytext);
    	return WORD;
}
[^ \t\n|><&][^ \t\n|><&]* {
		yylval.string_val = strdup(yytext);
		return WORD;
}
