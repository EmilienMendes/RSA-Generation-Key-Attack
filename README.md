# Attaques side-channel sur générations de premiers
Les attaques par analyse de canaux auxiliaires, dites attaques side-channel, utilisent des informations (ex. consommation de courant, temps d’exécution) 
non disponibles dans les modèles d’attaquant de la cryptanalyse classique. Dans ce projet vous étudierez deux techniques différentes
d’attaques side-channel s’appliquant à deux méthodes différentes de génération de premiers.

## SPA sur la génération de nombre premiers par la méthode du crible simple
Le principe de cet attaque repose sur l'obtention d'information lors de la génération des nombres premiers p et q.  <br>
Par la méthode du crible simple, on effectue des divisons successifs par les N plus petit nombres premiers [r<sub>1</sub>,r<sub>2</sub>,... , r<sub>N</sub> ]
A partir de ces divisions successifs lors des j-ième algorithmes on peut retrouver les relations suivantes :

v<sub>j1</sub> = a<sub>i1</sub> mod r<sub>i1</sub> <br>
v<sub>j2</sub> = a<sub>i2</sub> mod r<sub>i2</sub> <br>
$\vdots$ <br>
v<sub>jn</sub> = a<sub>in</sub> mod r<sub>in</sub> <br>

On trouve les relations  : <br>
p = v<sub>0</sub> + 2 x m  = v<sub>j1</sub> + 2 x ( m - j<sub>1</sub>) = 2 x ( m - j1 )  modulo r<sub>i1</sub> <br>
p = v<sub>0</sub> + 2 x m  = v<sub>j2</sub> + 2 x ( m - j<sub>2</sub>) = 2 x ( m - j2 )  modulo r<sub>i2</sub> <br>
$\vdots$ <br>
p = v<sub>0</sub> + 2 x m  = v<sub>jn</sub> + 2 x ( m - j<sub>n</sub>) = 2 x ( m - jn )  modulo r<sub>in</sub> <br>


En utilisant le theoreme des restes chinois on trouve

p = a<sub>p</sub> mod s<sub>p</sub> avec s<sub>p</sub> = $$\prod r_{r \in S_p}  $$ <br> 
q = a<sub>q</sub> mod s<sub>q</sub> avec s<sub>q</sub> = $$\prod r_{r \in S_q} $$ <br> 

On peut aussi retrouver a<sub>q</sub> et b<sub>p</sub> en utilisant l'inversion modulaire et en multipliant par n. <br>
Cela permet de retrouver les valeur finales avec le théorème des restes chinois 

p = c<sub>p</sub> mod s <br>
q = c<sub>q</sub> mod s <br>
s = lcm(s<sub>p</sub>,s<sub>q</sub>)

Pour retrouver p et q, il faudrait appliquer l'algorithme [LLL](https://fr.wikipedia.org/wiki/Algorithme_LLL), ce qui n'a pas été effectué dans le cadre de notre implémentation.





## Utilisation
### Prérequis
Il suffit d'installer make et gmp pour pouvoir compiler de manière automatique.

### Compilation
1. Cloner le repository:  
   ```bash
   git clone https://github.com/EmilienMendes/RSA-Generation-Key-Attack.git
2. Compiler toutes les règles: 
    ```bash 
    make compilation
###  Exécution du programme
#### SPA
Il y a deux étapes à effectuer pour faire l'attaque
- Créer les nombres premiers p et q et générer leur trace associé
- Attaquer les deux traces pour retrouver c<sub>p</sub> et c<sub>q</sub>

Un fichier main est disponible et effectue les deux tâches au même momment.
Il est possible de faire appel à chacune des fonctions indépendemment.

``` bash
make spa
```
#### CPA
On suit les mêmes étapes que pour la SPA
- Créer le nombres premiers p et générer sa trace 
- Attaquer cette trace

``` bash
make cpa
```

Mesure :
 
l(i,j) = a * poids_hamming(wi) + b  avec  a = 1 et b = gaussienne avec esperance = 0 et sigma $\in$ [0.5,3];

Faire avec différents niveaux de bruits

Faire les mesures avec plusieurs générations de premiers (proba de succès)
