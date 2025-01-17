all : cpa

fichier_gen_premier := ../generation-nombre-premier/generation_premier.c
k := 512
N := 52
t := 3

ptrace := ptrace.txt
parametres := param.txt


compilation_cpa : main.c attaque_cpa.c crible_optimise.c gauss.c 
	$(CC) -Wall -o main main.c  ${fichier_gen_premier} crible_optimise.c attaque_cpa.c gauss.c hamming.c -lgmp -lm

cpa : compilation_cpa 
	./main ${k} ${N} ${t} ${ptrace} ${parametres}


compil_test_gauss : test_gauss.c gauss.c
	$(CC) -Wall -o test_gauss test_gauss.c  gauss.c -lm

test_gauss : compil_test_gauss
	./test_gauss

suppression_stat : 
	rm -f stats stats2

clean :
	rm -f main ${ptrace} ${parametres} log 