#include "shell.h"


int execute_args(char **args)
{
	pid_t pid;
	int status;
	char *builtin_func_list[] = {
		// you should add something here
		"cd" ,
		"exit",
	};

	int (*builtin_func[])(char **) = {
		// you should add something here
		&own_cd , 
		&own_exit,
	};


	long unsigned int i = 0;
	for (; i < sizeof(builtin_func_list) / sizeof(char *); i++)
	{
		// /* if there is a match execute the builtin command 
		if (strcmp(args[0], builtin_func_list[i]) == 0)
		{
			return ((*builtin_func[i])(args));
		}
	}


	// TODO

	int pid1 = fork();
	int status2;
	if (pid1 == 0) {
		//char *arguments[] = {"/bin/sh", "-c", (char *) command, NULL};
		status2 = execvp(args[0] , args );
		printf("Failed to invoke /bin/sh to execute the supplied command.");
		exit(0);
	}
	waitpid(pid1, &status2 , 0);
	return (-1); // this status code means everything is ok
}