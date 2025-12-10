#!/bin/bash

# Filename: ex21.sh
#
# Problem: Count the number of quotes each person had in the top 100 quote
# list. Then output the list of names in alphabetical order with the number of quotes each.
cat ex21.input | awk '{
		if (match($0,"-")){
			name=substr($0,0,match($0,"-")-1);
			counts[name]++
			if (!(name in names)){
			names[count++] = name #| tr -d '[:space:]'
		}
#			name = substr($0, 1, match($0, "-") - 1);
#			counts[name]++
	}
	}
	#BEGIN {count = 0}
	END{
		for (person in counts){
		print person " had " counts[person] " quotes.";
		}
		#sort -k1 -nr
}' | sort -k1,1 -k3,3n | sort -t ' ' -k1,1 # | head -n 15
