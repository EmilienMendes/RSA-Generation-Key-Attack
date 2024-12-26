all : compilation lancement_attaque 

generation_premier_source := ../generation-nombre-premier/generation_premier.c
k := 512
N := 52
t := 3

ptrace := ptrace.txt
parametres := parametres.txt

compilation : compilation_crible_optimise compilation_attaque_cpa

compilation_crible_optimise : crible_optimise.c ${generation_premier_source}
	$(CC) -Wall -o crible_optimise ${generation_premier_source} crible_optimise.c  -lgmp

compilation_attaque_cpa : attaque_cpa.c ${generation_premier_source}
	$(CC) -Wall -o attaque_cpa ${generation_premier_source} attaque_cpa.c gauss.c -lgmp -lm

generation_premier_optimise : compilation_crible_optimise
	./crible_optimise ${k} ${N} ${t} ${ptrace} ${parametres}

attaque_cpa : compilation_attaque_cpa 
	./attaque_cpa ${N} ${ptrace} ${parametres}

lancement_attaque : 
	./crible_optimise ${k} ${N} ${t} ${ptrace} ${parametres}
	./attaque_cpa ${N} ${ptrace} ${parametres}

clean :
	rm -f crible_optimise attaque_cpa ${ptrace} ${parametres}