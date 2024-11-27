all : crible_simple 

k := 512
N := 100
t := 3

compilation : compilation_crible_simple

compilation_crible_simple : crible_simple.c ../utility/utility.c
	$(CC) -Wall -o crible_simple ../utility/utility.c crible_simple.c   -lgmp

crible_simple : compilation_crible_simple
	./crible_simple ${k} ${N} ${t}

	
clean :
	rm -f crible_simple