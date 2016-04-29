#!/bin/bash

score=0
total=0

if [ -z "$1" ]; then
	passFile=tmpPass
else
	passFile=$1
fi

echo "Using $passFile as password file"

function test {
	name="$1"
	points="$2"
	output="$3"
	input="$4"
	
	let total=total+points
	
	echo "$input" > $passFile
	echo $output > tmpOracle
	./pwcheck.sh $passFile > tmpOutput
	
	printf "Test $name: "
	
	diff=$(diff -iw tmpOracle tmpOutput)
	if [ "$diff" != "" ]; then
		echo -e "\033[1;31mfail\033[0m"
		echo $diff
	else
		echo -e "\033[1;32mpass\033[0m"
		let score=score+points
	fi
}



test 1a 1 'Error: Password length invalid.' 'qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq'
test 1b 1 'Error: Password length invalid.' 'abcd5'

test 2a 2 'Password Score: 11' 'aBcDeF'
test 2b 2 'Password Score: 36' 'aBcDeFgHiJkLmNoPqRsTuVwXyZaBcDe'

test 3a 2 'Password Score: 16' 'aBc9F1'
test 3b 2 'Password Score: 41' 'aBcDeFg94JkLmNo8qRsTuVwXyZaBcD2'

test 4a 2 'Password Score: 17' 'aBcDeF@'
test 4b 2 'Password Score: 17' 'aBcDeF$'

test 5a 2 'Password Score: 3' 'AbCdEfGG'
test 5b 2 'Password Score: 3' 'aBcDeFgg'

test 6a 2 'Password Score: 17' 'aBcDeFgHiJkawfa'
test 6b 2 'Password Score: 14' 'abcdefghijkl'

test 7a 2 'Password Score: 17' 'AbCdEfGhIjKAWFA'
test 7b 2 'Password Score: 14' 'ABCDEFGHIJKL'

test 8a 2 'Password Score: 8' '123456'
test 8b 2 'Password Score: 32' 'ab12345f2456gD986f2e35a6f'

test 9a 5 'Password Score: 26' 'Exon#Mobi@Le21'
test 9b 5 'Password Score: 21' '123456789abcdef@gDWSS@Aw4'

echo "Score: $score/$total"

rm tmpPass tmpOracle tmpOutput
