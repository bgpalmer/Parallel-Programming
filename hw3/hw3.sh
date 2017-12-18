#!/bin/bash

file='results.txt'
fix=( 1 2 )
numt=( 1 2 4 )
pad=$( seq 0 1 15 )

if [[ -f $file ]]; then
	rm -f $file
fi

echo $( uptime )
echo $( uptime ) >> $file
echo $( hostname ) >> $file
echo >> $file
echo >> $file

printf "\t" >> $file
for t in ${numt[@]}; do
	for f in ${fix[@]}; do
		printf "FIX_$f - NUMT_$t\t" >> $file
	done
done
echo >> $file

for p in ${pad[@]}; do
	printf "$p\t" >> $file 
	for t in ${numt[@]}; do
		for f in ${fix[@]}; do
			/usr/local/common/gcc-5.4.0/bin/gcc hw3.c -std=c99 -DNUMT=$t -DNUMPAD=$p -DFIX=$f -o prog -lm -fopenmp
			./prog $file
			if [[ $? != 0 ]]; then
				echo "EXITING"
				rm -f $file 
				exit 1
			fi
			rm -f prog
		done
	done
	echo >> $file	
done

