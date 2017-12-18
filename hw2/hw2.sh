#!/bin/bash
file='results'
fields=( 'static-1' 'static-4096' 'dynamic-1' 'dynamic-4096' )
schedules=( static dynamic )
# threads=( 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16) 
threads=( 1 2 4 6 8 10 12 14 16 18 20 22 24 )
chunks=( 1 4096 )
echo $( hostname )
echo $( uptime )
echo >> "temp"
for f in ${fields[@]}; do
	echo $f >> "temp"
done
cat "temp" | tr '\n' '\t' >> $file
echo >> $file
rm -f "temp"
for t in ${threads[@]} 
do
	printf "$t\t" >> $file
	for s in ${schedules[@]}
	do
		echo $s
		for c in ${chunks[@]}
		do
			g++ -DFILENAME=$file -DNUMT=$t -DSCHEDULE=$s -DCHUNKSIZE=$c -lm -fopenmp hw2.cpp -o prog
			./prog
			rm -f prog
		done
	done
	echo >> $file
done
