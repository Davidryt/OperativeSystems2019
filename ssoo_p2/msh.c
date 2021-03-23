/*-
 * msh.c
 *
 * Minishell C source
 * Show how to use "obtain_order" input interface function
 *
 * THIS FILE IS TO BE MODIFIED
 */

#include <stddef.h>			/* NULL */
#include <stdio.h>			/* setbuf, printf */

#include <stdlib.h>

#include <sys/types.h>	//For open()
#include <fcntl.h>
#include <sys/stat.h>

#include <sys/wait.h>
#include <unistd.h>

#include <sys/time.h>
#include <string.h>

#include <linux/limits.h>
	

extern int obtain_order();		/* See parser.y for description */

int main(void)
{
	char ***argvv;
	int command_counter;
	int num_commands;
	char *filev[3];
	int bg;
	int ret;
	
	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);

	while (1) 
	{
		fprintf(stderr, "%s", "msh> ");	/* Prompt */
		ret = obtain_order(&argvv, filev, &bg);
		if (ret == 0) break;		/* EOF */
		if (ret == -1) continue;	/* Syntax error */
		num_commands = ret - 1;		/* Line */
		if (num_commands == 0) continue;	/* Empty line */

/*
 * THE PART THAT MUST BE REMOVED STARTS HERE
 * THE FOLLOWING LINES ONLY GIVE AN IDEA OF HOW TO USE THE STRUCTURES
 * argvv AND filev. THESE LINES MUST BE REMOVED.
 */	
 	
 	
 	//If the command is mytime
	if(strcmp(argvv[0][0],"mytime")==0){
		//If the commands is piped to another commands error
		if(num_commands!=1){
			char error [29]="Usage:mytime <command<args>>\n";
			if(write(2,error,sizeof(error))<0)perror("writing");
		}else{	
			//Struct to save the time
			struct timeval start_time,end_time;
			//Move the arguments one place to the left in the array so the exec
			//call can be done easier
			for(int i=0;argvv[0][i]!=NULL;i++){
				argvv[0][i]=argvv[0][i+1];

			}
			//Variable to store the fianl time
			double time;
			//Get the time before execution
			if(gettimeofday(&start_time, NULL )!=0) perror("get time");
			int pid = fork();
			if(pid==0){
				//Exec
				execvp(argvv[0][0],argvv[0]);
				perror("exec");
				exit(-1);		
			}else if(pid>0){
				//Wait for the child to finish, if this fails then
				//most likely command inputed is wrong, print the error
				//message
				if(waitpid(pid,&pid,0)<0){		
					char error [29]="Usage:mytime <command<args>>\n";
					if(write(2,error,sizeof(error))<0)perror("writing");
				}
				//Store time after execution
				if(gettimeofday(&end_time, NULL )!=0) perror("get time");
				//computation of final time
				time=(end_time.tv_sec*1000000+end_time.tv_usec)-(start_time.tv_sec*1000000+start_time.tv_usec);
				printf("Time spent: %f secs.\n",time/1000000);
			}else{
				perror("fork");
			}
		}
		//Internal command mypwd
	}else if(strcmp(argvv[0][0],"mypwd")==0){
		//In case of wrong input
		if(num_commands!=1||argvv[0][1]!=NULL){
			char error [12]="Mypwd error\n";
			if(write(2,error,sizeof(error))<0)perror("writing");
		}else{
			//Get the actual path and print it
			char dir [PATH_MAX];
			getcwd(dir,PATH_MAX);
			printf("Current dir: %s\n",dir);
		}	
	}else{


		//Process id array
		int pid[num_commands];
		
		//Pipes variables
		int num_pipes=num_commands-1;
		int pip [num_pipes][2];
					
		//Executes once for every command in the line			
		for (command_counter = 0; command_counter < num_commands; command_counter++) 
		{
			// If its not the last iteration create a pipe, does not execute in simple commands
			if(command_counter+1 != num_commands){
				pipe(pip[command_counter]);
			}
			pid[command_counter] = fork();
			if(pid[command_counter]<0) perror("fork");
			if(pid[command_counter]==0){
				//Simple command
				if(num_commands==1){
				}
				//First command of sequence
				else if(command_counter==0){
						if(dup2(pip[command_counter][1],1)<0) perror("dup2");
						if(close(pip[command_counter][0])<0) perror("close 00");
						if(close(pip[command_counter][1])<0) perror("close 01");
				//Last command of sequence		
				}else if(command_counter+1 == num_commands){
						if(dup2(pip[command_counter-1][0],0)<0) perror("dup2");
						if(close(pip[command_counter-1][0])<0) perror("close x0");
						if(close(pip[command_counter-1][1])<0) perror("close x1");
				//Middle commands		
				}else{
					if(dup2(pip[command_counter-1][0],0)<0) perror("dup2");
					if(dup2(pip[command_counter][1],1)<0) perror("dup2");
					
					if(close(pip[command_counter-1][0])<0) perror("close n-1 0");
					if(close(pip[command_counter-1][1])<0) perror("close n-1 1");
					if(close(pip[command_counter][0])<0) perror("close n 0");
					if(close(pip[command_counter][1])<0) perror("close n 1");
				}
				//Redirect the input in case is need only first command or simple command
				if(command_counter==0){
					if(filev[0]!=NULL){

						if(close(0)<0)perror("close");
						if(open(filev[0],O_RDONLY)<0) perror("open");
					}
				
				}
				//Redirect the output in case is need only last command or simple command
				if(command_counter+1 == num_commands){
					if(filev[1]!=NULL){

						if(close(1)<0)perror("close");
						if(open(filev[1],O_WRONLY|O_CREAT)<0) perror("open");
					}
					if(bg==1){
						printf("[%d]\n",getpid());
					}
				}
				//Redirect error to every command
				if(filev[2]!=NULL){
						if(close(2)<0)perror("close");
						if(open(filev[2],O_CREAT|O_WRONLY,0666)<0)perror("open");
				}


				//Execution
				execvp(argvv[command_counter][0],argvv[command_counter]);
				perror("execvp");
				exit(-1);
		
			}
			//Parent process

			//Close pipes only after first command, doesnt execute in simple commands
			if(command_counter!=0){
				if(close(pip[command_counter-1][0])<0) perror("close p0");
				if(close(pip[command_counter-1][1])<0) perror("close p1");
			}
			
		}
		//In case the commands execute in the foreground
		if(bg==0){
			//Wait for each child, has to wait for them all to continue the shell 
			for(int i = 0;i<num_commands;i++){
				if(waitpid(pid[i],&pid[i],0)<0){
					perror("wait");
				}
			}
			
		}
	}
	//See if any child has finished in the bg and wait for it
	//Removal of zombies
	pid_t w =waitpid(-1,NULL,WNOHANG);
	while(w>0){

		w = waitpid(-1,NULL,WNOHANG);
	}


/*
 * END OF THE PART TO BE REMOVED
 */
	
	} //fin while 

	return 0;

} //end main

