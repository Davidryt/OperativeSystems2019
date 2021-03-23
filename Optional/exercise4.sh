#!/bin/bash

#This script will take a directory as argument and resize all jpg
#images as long as it's size is bigger than 1MiB

#We only need the directory as argument
if [ $# == 1 ];then
	#check that the arguments is a valid directory
	if [ -d $1 ] ;then
	#move script to the given directory
		cd $1
		#check each file as long as its extension is jpg
		IFS=$'\n'
		for x in $( ls | grep .jpg )
			do
			#Check if size is at least 1MiB
			size=$(stat -c%s $x )
			if [ $size -gt 1048576 ];then
				#print the modified image
			 	echo $1/$x
			 	#resize the image (ImageMagick conserves proportions)
			 	mogrify -resize 720x720 $x
			fi			
		done
	else
		echo "$1: No such directory"
	fi
else
	echo "Usage: ./execise4.sh dir "
fi

exit 0
