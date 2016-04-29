#!/bin/bash

#DO NOT REMOVE THE FOLLOWING TWO LINES
git add $0 >> .local.git.out
git commit -a -m "Lab 2 commit" >> .local.git.out

#Your code here
#get the first line of given file, count the length.
pw=`head -1 $1`
length=${#pw}

#check the length of password, give the initial score
if [ $length -gt 32 ] 
then
	echo "Error: Password length invalid."
	exit 1
fi 

if [ $length -lt 6 ]
then
	echo "Error: Password length invalid."
	exit 1
fi

score=$length

#if there is a special character, +5 points.
test=`egrep [#$\+%@] <<< $pw` 
if [ ${#test} -gt 0 ]
then
	score=$((score + 5))
fi

#if there is a number, +5 points.
test=`egrep [0-9] <<< $pw`
if [ ${#test} -gt 0 ]
then
	score=$((score + 5))
fi

#if there is a alphabetical, +5 points.
test=`egrep [A-Za-z] <<< $pw`
if [ ${#test} -gt 0 ]
then
	score=$((score + 5))
fi

#if there is a repitatin of alphanumeric character, -10 points.
test=`egrep '([A-Za-z0-9])\1' <<< $pw`
if [ ${#test} -gt 0 ]
then
	score=$((score - 10))
fi

#if there are 3 consecutive lowercase, -3 points.
test=`egrep '[a-z][a-z][a-z]' <<< $pw`
if [ ${#test} -gt 0 ]
then
	score=$((score - 3))
fi

#if there are 3 consecutive uppercase, -3 points.
test=`egrep '[A-Z][A-Z][A-Z]' <<< $pw`
if [ ${#test} -gt 0 ]
then
	score=$((score - 3))
fi

#if there are 3 consecutive numbers, -3 points.
test=`egrep '[0-9][0-9][0-9]' <<< $pw`
if [ ${#test} -gt 0 ]
then
	score=$((score - 3))
fi

#return the final score
echo "Password Score: ${score}"
