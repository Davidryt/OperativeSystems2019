#!/bin/bash
#This script will print the name and command parameter of a line 
#given the arguments of the /etc/passwd file 

#While executes once for every argument
i=0
while [ $i -lt $# ];  
do		
		#check that the first parameter of the line is equal to the argument
		#so arguments that apperears in a line but are not the first agument
		#wont execute the command
		if [ "$(grep $1 /etc/passwd | cut -d: -f1)" = $1 ]; then
				grep $1 /etc/passwd | awk -F: '{printf $1}'
				printf ": "
				grep $1 /etc/passwd | awk -F: '{printf $7}'
				echo
		else
				echo "-ERROR-" $1 ": no such user"
		fi
		
	#shift the argurmentssa
	shift
done

exit 0 

