Bataille Navale
============
### Build

`make BOARD=idosens_base MINITEL=1` permet de build le code pour le joueur 1 et  
`make BOARD=idosens_base MINITEL=2` permet de build le code pour le joueur 2

Il est possible de rajouter `-j 16` à la fin de la commande pour utiliser plus de ressource de votre pc et accélérer le build

### Guide de jeu

➢ Les touches utilisables au clavier du minitel sont les suivantes: 

&esmp;Z, Q, S, D : déplacement du curseur (position actuelle marquée par le symbole ‘#’)  
&esmp;H, V : indiquer lors du placement si ce dernier est vertical (V) ou horizontal (H). Le placement se fait toujours vers le bas et vers la droite respectivement.  
&esmp;ENTRER : valider le placement / tir  
&esmp;* : Changer la grille affichée (ne fonctionne que lorsque c’est notre tour de jouer)

Ces touches sont facilement modifiables dans le code dans le cas où l’on utiliserait un minitel pour lequel ces touches ne sont plus fonctionnelles.

➢ Les tours de jeu et les conditions de victoires sont définis comme suit:

J1 commence le placement de bateaux, et effectue le premier tir lorsque J2 a effectué ses placements.

La partie est terminée lorsque l’un des joueurs a perdu l’ensemble de ses vies (chaque joueur en possède 17 initialement).

