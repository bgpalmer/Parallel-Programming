#!/bin/bash


for t in 1 2 3 4 
do
	for s in 1000 2000 4000 8000 16000 32000 64000 128000 256000 512000
	do
		g++ -DSZ=$s -DNUMT=$t hw0.cpp -o prog -lm -fopenmp
		./prog
	done
done

