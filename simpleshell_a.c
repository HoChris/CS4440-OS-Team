#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>	/* needed for wait() function */
#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
	char *args[MAX_LINE/2 + 1];         /* command line arguments */
	char userInput[MAX_LINE/2+1];
	char *split;
	char *history[MAX_LINE];
	int historyC=0;
	int entry = 0;
	int should_run = 1;                 /* flag to determine when to exit program */

	while (should_run){
		printf("osh>");
		fflush(stdout);
		
		if(!fgets(userInput,MAX_LINE,stdin))
			break;
		size_t length=strlen(userInput);
		if(userInput[length-1]=='\n')
			userInput[length-1]='\0';

		split=strtok(userInput," ");
		int argc=0;
		
        int goCmd = 0;              /* flag for last cmd */
        int goNumberCmd = 0;       /* flag for number cmd */
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
			if(split[0] == '!' && split[1] == '!'){                      /* Checks for "!!" to execute lastCmd */
			    if(history == NULL || history[0] == NULL){                  /* if no history */
			        puts("No commands in history.");
			    }
			    else{
			        goCmd = 1;
					history[historyC] = args[argc];						/*copys array and place into history*/
			    }
			}
			else if (split[0] == '!' && isdigit(split[1]) != 0){         /* grabs the cmd number to execute */
			     goNumberCmd = 1;
				 /*NOT DONE YET*/
			}
			if (split[0] == '-')
				history[--entry] = ' ' + strdup(split);					/* this is placing its own spot in history -l*/
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
		
		
		if(strcmp("exit",args[0])==0){
			should_run=0;
			exit(0);
		}
		else if(strcmp("history",args[0])==0){                   /* added history cmd to print table */
		    int i = 0;
			for(i=entry-1;i>=0;--i){									//needs to cut off to 10
				printf("%d - %s\n",i+1, history[i]);
			}
		}
		
		/* After reading user input, the steps are:
		* (1) fork a child process using fork()
		* (2) the child process will invoke execvp()
		* (3) if command included &, parent will invoke wait() */
		
		/* Did you guys see Professor Thomas' post in the forum?
		 * All the forking is crashing cs1. She said to do without
		 * when testing, but it's still supposed to be in the final
		 * code that we turn in? */
		else{
			pid_t pid, wpid;
			int status;
			pid = fork();
			/* added for possibility of failed fork */
			if (pid < 0) {
				perror("Fork failed.\n");
				exit(1);
			}
			else if (pid==0 && goCmd ==0){ 
				execvp(args[0],args);
			/*	if(userInput[length-1] == '&')
			*		wpid = wait(&status);		*/
			}
			else if (pid==0 && goCmd ==1){                         /* added lastCmd checks */
				printf("%s \n", history[historyC]);                     /* prints lastCmd */
				execvp(history[historyC],args);  
				goCmd = 0;  
			}
			else {
				wait(NULL);
				printf("Child Complete\n");
			}
		}
	}
	return 0;
}