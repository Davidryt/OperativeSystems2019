#!/bin/bash

#This script takes 2 numbers as arguments and prints all prime number
#in the range 

#checks that the arguments are OK
if [ $# == 2 ] && [ $1 -lt $2 ] && [ $1 -ge 0 ] && [ $2 -ge 0 ];then
	i=$1
	j=$2
	#for every number in the interval check if the factor of it it's the same
	#as the original number if so print it as it's prime
	while [ $i -le $j ];
	do
	if [ $i -gt 1 ]; then
		if [ $(factor $i | tr -d ' '| cut -d: -f2) = $i ]; then
			echo $i
		fi
	fi
	i=$(($i+1))
	done
else
	echo "Usage: ./exercise3.sh NUM NUM"
fi
exit 0
