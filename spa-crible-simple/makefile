all : spa 

fichier_gen_premier := ../generation-nombre-premier/generation_premier.c
k := 1024
N := 54
t := 10
ptrace := ptrace.txt
qtrace := qtrace.txt
parametres := parametres.txt


compilation : main.c crible_simple.c  attaque_spa.c ${fichier_gen_premier}
	$(CC) -Wall -o main main.c ${fichier_gen_premier} crible_simple.c attaque_spa.c -lgmp

spa : compilation 
	./main  ${k} ${N} ${t} ${ptrace} ${qtrace} ${parametres}

suppression_stat : 
	rm -f stats

clean :
	rm -f main ${ptrace} ${qtrace} ${parametres}