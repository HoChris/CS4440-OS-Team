
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
char *args[MAX_LINE/2 + 1]; /* command line arguments */
char userInput[MAX_LINE/2+1];
char *split;
char *history[MAX_LINE];

int historyC=0;
int counter;
int should_run = 1; /* flag to determine when to exit program */
while (should_run) {
printf("osh>");
fflush(stdout);
/**
* After reading user input, the steps are:
* (1) fork a child process using fork()
* (2) the child process will invoke execvp()
* (3) if command included &, parent will invoke wait()
*/
if(!fgets(userInput,MAX_LINE,stdin))
break;
size_t length=strlen(userInput);
if(userInput[length-1]=='\n')
	userInput[length-1]='\0';


split=strtok(userInput," ");
int argc=0;

while (split != NULL)
  {
    args[argc]=strdup(split);   
/*    printf ("%s\n",splitArray); */
    split = strtok (NULL," ");
    argc++;
  }
args[argc]=NULL;

for(counter;counter<historyC;counter++)
	printf("%s\n",history[counter]);

pid_t pid;
pid = fork();
if (pid==0){
execvp(args[0],args);

}
else{
wait(NULL);
printf("Child Complete\n");
}

}
return 0;
}
