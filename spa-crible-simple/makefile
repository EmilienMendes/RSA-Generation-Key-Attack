all : spa 

fichier_gen_premier := ../generation-nombre-premier/generation_premier.c
k := 512
N := 100
t := 3
ptrace := ptrace.txt
qtrace := qtrace.txt
parametres := parametres.txt


compilation : main.c crible_simple.c  attaque_spa.c ${fichier_gen_premier}
	$(CC) -Wall -o main main.c ${fichier_gen_premier} crible_simple.c attaque_spa.c -lgmp

spa : compilation 
	./main  ${k} ${N} ${t} ${ptrace} ${qtrace} ${parametres}

clean :
	rm -f main ${ptrace} ${qtrace} ${parametres}