#!/bin/bash


for t in 1 2 4 8  
do
	echo "Thread count: $t"
	for n in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
	do
		g++ -DNUMT=$t -DNUMNODES=$(( 2 ** $n )) hw1.cpp -o prog -lm -fopenmp
		if [ $(( $n % 5 )) -eq 0 ]; then
			echo "Subdivision: $n"
		fi
		./prog
	done
done

echo "Average Performance Results:"
cat avgPerf.txt
echo "
Peak Performance Results:"
cat peakPerf.txt
