#!/bin/bash
#This script will print the name and command parameter of a line 
#given the arguments of the /etc/passwd file 



#While executes once for every argument
i=0
while [ $i -lt $# ];  
do		
		if [ "$(grep :$1: /etc/group | cut -d: -f3)" ]; then
				grep :$1: /etc/group | cut -d: -f4 
		else
				echo "-ERROR-" $1 ": no such GID"
		fi
	#shift the argurments
	shift
done

exit 0 

