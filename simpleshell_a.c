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
	int cNumber = 0;										/* the number for the number cmd */
	int entry = 0;
	int should_run = 1;                 /* flag to determine when to exit program */
	int goCmd = 0;              /* flag for last cmd movedflags */
    int goNumberCmd = 0;       /* flag for number cmd */
	int status = 0;

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
			        puts("No commands in history.\n");
			    }
			    else{
			        goCmd = 1;					//flag on
					history[historyC] = args[argc];						/*copys and place into history*/
			    }
			}
			else if (split[0] == '!' && split[1] != '!'){         
			//else if (split[0] == '!' && isdigit(split[1]) != 0){         /* grabs the cmd number to execute */
			     cNumber = atoi(&split[1]);
				 if(cNumber >= entry)
					 puts("No such command in history\n");
				 else{
				 goNumberCmd = 1;				//flag on
				 printf("--- %d --- \n", cNumber);
				 history[cNumber] = args[argc];	//changed from cNumber to argc
				 /*NOT DONE YET*/
				 }
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
		
		else{
			pid_t pid, wpid;
			pid = fork();
			/* added for possibility of failed fork */
			if (pid < 0) {
				perror("Fork failed.\n");
				exit(1);
			}
			else if (pid==0 && goCmd ==0){ 
				execvp(args[0],args);
			/*	if(userInput[length-1] == '&')
					wpid = wait(&status);		*/
			}
/*			else if ((pid = fork()) ==0){ 
				if(goCmd <= 0 || goNumberCmd <= 0)
					execvp(args[0],args);
				else if( goCmd > 0){
					printf("%s \n", history[historyC]);   				// prints lastCmd 
					goCmd = 0;  
					execvp(history[historyC],args);  
				}
				//<<<<<<<<<<<<<<<<<<<<started moving ifelses to more condence version play with wpid and pid	
				
			} */

			else if (pid==0 && goCmd ==1){                         /* added lastCmd checks */
				printf("%s \n", history[historyC]);   				/* prints lastCmd */
				goCmd = 0;  
				execvp(history[historyC],args);  
			}
			else if (pid==0 && goNumberCmd ==1){                         /* added Number Cmd checks */
				printf("%s \n", history[cNumber]);
				goNumberCmd = 0;  				/* prints Number Cmd  */
				execvp(history[cNumber],args);  
			}
			else {
				wait(NULL);
				printf("Child Complete\n");
			}
		}
	}
	
	return 0;
}