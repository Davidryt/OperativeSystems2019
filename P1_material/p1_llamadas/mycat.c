#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <sys/types.h>          /* Cabeceras llamada al sistema open  */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>             /* Cabecera llamadas al sistema read, write y close  */


int main(int argc, char *argv[]){

int n;
char c[1024];
int file;
if(argv[1]!=NULL){
	if((file = open(argv[1],O_RDONLY,0666))<0){	
		perror("-1");			
	}else{
		if((n=read(file,c,1024))<0){
			perror("-1");		
		}else{
			int size=0;;
			int i=0;
			while(c[i]!='\0'){
				size++;
				i++;			
			}
			if((write(STDOUT_FILENO,c,size))<0){
				perror("-1");
			}
		}
	}
	close(file);
}else{
	perror("-1");		//PREGUNTAR
}
}

