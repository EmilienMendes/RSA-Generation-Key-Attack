all : spa 

fichier_gen_premier := ../generation-nombre-premier/generation_premier.c
k := 512
N := 54
t := 10
nb_exec := 1

ptrace := ptrace.txt
qtrace := qtrace.txt
parametres := parametres.txt


compilation_spa : main.c crible_simple.c  attaque_spa.c ${fichier_gen_premier}
	$(CC) -Wall -o main main.c ${fichier_gen_premier} crible_simple.c attaque_spa.c -lgmp

spa : compilation_spa 
	./main  ${k} ${N} ${t} ${nb_exec} ${ptrace} ${qtrace} ${parametres}


clean :
	rm -f main ${ptrace} ${qtrace} ${parametres}