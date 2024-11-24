all : crible_optimise

k := 512
N := 100
t := 3

compilation : compilation_crible_simple compilation_crible_optimise

compilation_crible_simple : crible_simple.c 
	$(CC) -Wall -o crible_simple crible_simple.c  -lgmp

crible_simple : compilation_crible_simple
	./crible_simple ${k} ${N} ${t}

compilation_crible_optimise : crible_optimise.c
	$(CC) -Wall -o crible_optimise crible_optimise.c  -lgmp

crible_optimise : compilation_crible_optimise
	./crible_optimise ${k} ${N} ${t}
	
clean :
	rm -f crible_simple crible_optimise