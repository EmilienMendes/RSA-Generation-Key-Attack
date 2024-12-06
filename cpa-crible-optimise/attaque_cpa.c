#include "attaque_cpa.h"


void attaque_cpa(int N,char *trace,char *parametres){

}




int main(int argc,char **argv){
    if(argc != 4 ){
        printf("Usage : %s <N> <fichier1> <fichier2> \nN: nombre de petits premiers utilise pour le crible optimise\n",argv[0]);
        printf("fichier1 : nom du fichier contenant la trace d'execution\nfichier2 : nom du fichier contenant les parametres RSA\n");
        return 1;
    }

    int N = atoi(argv[1]);
    char *trace = argv[2];
    char *parametres = argv[3];
    attaque_cpa(N,trace,parametres);

    return 0;
}