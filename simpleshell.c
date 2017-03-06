#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>	/* needed for wait() function */
#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
	char *args[MAX LINE/2 + 1];         /* command line arguments */
	char userInput[MAX_LINE/2+1];
	char *split;
	char *history[MAX_LINE];
	int historyC=0;
	int entry = 0;
	int should_run = 1;                 /* flag to determine when to exit program */

	while (should_run) {
		printf("osh>");
		fflush(stdout);
		
		if(!fgets(userInput,MAX_LINE,stdin))
			break;
		size_t length=strlen(userInput);
		if(userInput[length-1]=='\n')
			userInput[length-1]='\0';

		split=strtok(userInput," ");
		int argc=0;

		while (split != NULL)
		{
			/* The array to store the history of the commands was initially
			 * history[historyC][entry]. If I'm interpreting it correctly, 
			 * it's the command and corresponding #. For the latter, the 
			 * index of the array should suffice.
			 * The 'if' part is to account for commands such as 'ls -l'.
			 * The input is parsed by a space, but any word starting 
			 * with a dash needs to be put with the word before it.
			 */
			if (split[0] == '-')
				history[--entry] += ' ' + strdup(split);
			else
				history[entry] = strdup(split);
			entry++;
			
			args[argc]=strdup(split);   
		/*    printf ("%s\n",splitArray); */
			split = strtok (NULL," ");
			argc++;
			
		}
		args[argc]=NULL;

	/*	for(counter;counter<historyC;counter++)
	*		printf("%s\n",history[counter]);	*/
		
		
		if(strcmp("exit",args[0])==0)
			should_run=0;
		
		/* After reading user input, the steps are:
		* (1) fork a child process using fork()
		* (2) the child process will invoke execvp()
		* (3) if command included &, parent will invoke wait() */
		
		/* Did you guys see Professor Thomas' post in the forum?
		 * All the forking is crashing cs1. She said to do without
		 * when testing, but it's still supposed to be in the final
		 * code that we turn in? */
		pid_t pid, wpid;
		int status;
		pid = fork();
		/* added for possibility of failed fork */
		if (pid < 0) {
			perror("Fork failed.\n");
			exit(1);
		}
		else if (pid==0){
			execvp(args[0],args);
		/*	if(userInput[length-1] == '&')
		*		wpid = wait(&status);		*/
		}
		else {
			wait(NULL);
			printf("Child Complete\n");
		}
	}
	return 0;
}
