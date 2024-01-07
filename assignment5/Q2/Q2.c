#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#define TOK_DELIM " \t\r\n\a\""


pid_t pid ; 
int IO_pipe[100][2] ;


char **split_line(char *line)
{
	int bufsize = 64;
	int i = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;
	if (!tokens)
	{
		fprintf(stderr, "allocation error in split_line: tokens\n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, TOK_DELIM);
	while (token != NULL)
	{
		/* handle comments */
		if (token[0] == '#')
		{
			break;
		}
		tokens[i] = token;
		i++;
		if (i >= bufsize)
		{
			bufsize += bufsize;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "reallocation error in split_line: tokens");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, TOK_DELIM);
	}
	tokens[i] = NULL;
	return (tokens);
}



char *read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;

	if (getline(&line, &bufsize, stdin) == -1) /* if getline fails */
	{
		if (feof(stdin)) /* test for the eof */
		{
			free(line); /* avoid memory leaks when ctrl + d */
			exit(EXIT_SUCCESS); /* we recieved an eof */
		}
		else
		{
			free(line); /* avoid memory leaks when getline fails */
			perror("error while reading the line from stdin");
			exit(EXIT_FAILURE);
		}
	}
	return (line);
}
int commands_len; 

void close_pipes() 
{
    int i = 0;
    for (i = 0; i <commands_len; i++) {
        close(IO_pipe[i][0]);
        close(IO_pipe[i][1]);
    }
}

int execute_args(char **commands)
{
	char** args ; 
	for(int i=0; i < commands_len ; i++)
	{
		pipe(IO_pipe[i]) ; 
	}

    pid_t pids[commands_len] ; 
	for(int j=0 ; j < commands_len ; j++ )
    {  
        args = split_line(commands[j]); 
        pid_t pid;
        int status;
        pid = fork();
        if (pid ==  0)
        {   
            if( j > 0 ){
                dup2(IO_pipe[j-1][0] , STDIN_FILENO); 
            }
            if( j < commands_len-1)
            {
                dup2(IO_pipe[j][1] , STDOUT_FILENO); 
            }

			close_pipes() ; 
            if (execvp(args[0], args) == -1)
            {
                perror("error in new_process: child process");
            }

            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("error in new_process: forking");
		}
        else
        {  
            if ( WIFEXITED(status) ) {
                const int es = WEXITSTATUS(status);
                if ( es != 0 )
                    return 1 ; 
            }
			pids[j] = pid ; 
        }
		
	}
	close_pipes();
	for(int u =0 ; u < commands_len ; u++)
		waitpid(pids[u]  , NULL, WUNTRACED) ; 

	return 0 ;
}

int main(int argc , char** argv )
{
    char** args ; 
    printf("prompt> " ) ; 
    char* line = read_line() ; 
    char *token = strtok(line , "|"); 
    int l = strlen(token)-1 ; 
    char* commands[l] ; 
    int i=0 ; 
    while (token != NULL)
    {
        commands[i] = token ; 
        token = strtok(NULL , "|");
        i ++ ; 
    }
	
    commands_len = i ;
    execute_args(commands) ; 
    return 0  ; 
}




