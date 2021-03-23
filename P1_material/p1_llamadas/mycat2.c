#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <sys/types.h>          /* Cabeceras llamada al sistema open  */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>             /* Cabecera llamadas al sistema read, write y close  */


int main(int argc, char *argv[])
{
char c;
FILE *file;
if(argv[1]!=NULL){
	file = fopen(argv[1],"r+");		//"r+"
	if(file<0){		//No entiendo por que no va, si no recuerdo -1 era no existe, -2 era error al abrir, comprobar
		printf("%i\n", -1);
		//break;
	}else{
		while(1==1) {
			c = fgetc(file);
			if(feof(file))break ;
			printf("%c", c);
		}
		fclose(file);}
	}
else{printf("%i\n", -1);}
}

