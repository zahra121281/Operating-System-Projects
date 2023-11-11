#include "shell.h"

/**
 * own_cd - changes the working dir of the current shell executon env
 * @args: target directory
 *
 * Return: -1 one success, 0 otherwise.
 */
int own_cd(char **args)
{
	char s[100]; 
    
    // using the command 
    int o = chdir(args[1]); 
	if( o == 0)
	{
		// printing current working directory 
		return -1  ; 
	}else	
		printf("There is no such a directory" ) ; 
		return (-1);
}
