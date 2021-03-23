#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
char dir[PATH_MAX];

	if(getcwd(dir,sizeof(dir))==NULL){
		perror("-1");
	}else{
		if(argv[1]==NULL){
			//ACTUAL		
		}else{
			if((opendir(dir))==NULL){

			}		
		}
	}
}
