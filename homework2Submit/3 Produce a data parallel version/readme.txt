Student :	 	Daler Rahimiv 
Observation:	Observation.txt 
Compile : 		gcc data_par_word_count.c -lpthread (Note: gives warning in strtok_r but due to some bug in strtok_r "warning: assignment makes pointer from integer without a cast")
Run 	: 		./a.out 
Run 10 times: 	./run10.sh (complile first, chmod +x run10.sh). This gives total time for 10 runs 
Output 	:		output.txt
Noise	: 		romove.txt holds all the words that need to be removed before counting. This could be edited
Substitude:		sub.txt holds all substitution word format "word-wordToSubstitude". This file could be edited 