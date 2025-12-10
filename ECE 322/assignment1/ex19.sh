#!/bin/bash

# Filename: ex19.sh
#
# Problem: Use awk to print the last field of every even numbered line from 
# ex19.input. 
cat ex19.input | awk '{
{n=split($0, a, ",")};
if (a[1] ~ /^[0-9]+$/) {
	split($0, a, ",");
	print a[n];
	}
}'
