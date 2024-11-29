all : generation_premier attaque_spa 

k := 512
N := 100
t := 3
ptrace := ptrace.txt
qtrace := qtrace.txt
parametres := parametres.txt

compilation : compilation_crible_simple compilation_attaque_spa

compilation_crible_simple : crible_simple.c ../utility/utility.c
	$(CC) -Wall -o crible_simple ../utility/utility.c crible_simple.c   -lgmp

compilation_attaque_spa : attaque_spa.c ../utility/utility.c
	$(CC) -Wall -o attaque_spa ../utility/utility.c attaque_spa.c   -lgmp

generation_premier : compilation_crible_simple
	./crible_simple ${k} ${N} ${t} ${ptrace} ${qtrace} ${parametres}

attaque_spa : compilation_attaque_spa 
	./attaque_spa ${N} ${ptrace} ${qtrace} ${parametres}

lancement_attaque : 
	./crible_simple ${k} ${N} ${t} ${ptrace} ${qtrace} ${parametres}
	./attaque_spa ${N} ${ptrace} ${qtrace} ${parametres}

clean :
	rm -f crible_simple attaque_spa ${ptrace} ${qtrace} ${parametres}