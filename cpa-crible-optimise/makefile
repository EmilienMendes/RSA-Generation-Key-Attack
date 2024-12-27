all : attaque_cpa

fichier_gen_premier := ../generation-nombre-premier/generation_premier.c
k := 512
N := 52
t := 3

ptrace := ptrace.txt
parametres := param.txt


compilation_cpa : main.c attaque_cpa.c crible_optimise.c gauss.c 
	$(CC) -Wall -o main main.c  ${fichier_gen_premier} crible_optimise.c attaque_cpa.c gauss.c hamming.c -lgmp -lm

attaque_cpa : compilation_cpa 
	./main ${k} ${N} ${t} ${ptrace} ${parametres}


clean :
	rm -f main ${ptrace} ${parametres}