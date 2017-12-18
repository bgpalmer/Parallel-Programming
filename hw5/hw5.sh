#!/bin/bash
NONSIMD="RESULTS.txt"
if [[ -f $NONSIMD ]]; then
	rm -f $NONSIMD
fi

SIZE=$( seq 1000 1000000 32000000 )
printf "\tNONSIMD:NONREDUCTION\tNONSIMD:REDUCTION\tSIMD:NONREDUCTION\tSIMD:REDUCTION\n" >> $NONSIMD
for s in ${SIZE[@]}; do
	printf "$s\t" >> $NONSIMD
	g++ hw5.cpp simd.p5.cpp -lm -fopenmp -DARRAYSIZE=$s -o main
	./main $NONSIMD
	if [[ $? != 0 ]]; then
		exit 1
	fi
	rm -f main
	echo >> $NONSIMD
done
	
cat $NONSIMD
		
