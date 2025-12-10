#!/bin/bash

mkdir -p results

for i in $(seq -w 01 16)
do 
	./sdriver.pl -t trace"$i".txt -s ./tsh -a "-p" >> results/tsh_output.txt

	./sdriver.pl -t trace"$i".txt -s ./tshref -a "-p" >> results/tshref_output.txt
done 

echo "TEST CASES COMPLETED"
