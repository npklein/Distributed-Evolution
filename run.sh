#!/bin/bash

mkdir "FletcherPowell"

make genospace

for i in {7..8}
do
	radius=$(echo "scale=2; 4 + 0.5 * $i" | bc)
	
	for j in {1..2}
	do
		./genospace 0 $radius
		mkdir "FletcherPowell/$radius"
		mv log.txt "FletcherPowell/$radius/log$j.txt"
	done
done
