#!/bin/bash

#Check that 2 arguments are passed
if [ $# == 2 ];then
	tr -c [:alnum:] [\\n\*] < $1 | sort | uniq -c | sort -nr | head -$2
else
	echo -e "Usage: ./exercise2_b.sh" "\e[4mFILE\e[0m \e[4mNUM\e[0m"
fi 
exit 0 
