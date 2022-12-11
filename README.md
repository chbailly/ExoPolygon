# ExoPolygon

J'ai passé environs 5 heures sur l'exo (mais je l'ai trouvé amusant !)  et je pense avoir une solution qui tourne, en tout cas ca fonctionne sur l'exemple donné.
Je pense qu'il y a pas mal de petites optimisations supplémentaires possibles mais c'est déjà un bon début.

Je ne me suis pas appliqué à faire du code propre, j'ai fait au plus vite.

J'ai joué le jeu, j'ai fait cet exercice tout seul, vous verrez cependant dans la description ci-dessous à quels moments je me suis aidé du net.

Concernant la logique:
* J'ai compris qu'on n'avait pas d'autres choix que de tester un peu tous les possibilités à cause de la contrainte qu'il faut que le segment reste à l'intérieur du polygone. En effet si on a un passage très étroit par exemple pour un chemin qui doit passer à cet endroit, pour le trouver on ne peut pas utiliser de méthode qui tend vers ce chemin 
* Il faut découper le polygone. Il est important de maitriser la complexité car La complexité est globalement n(n-1)/2 dans l'implem que j'ai faite. 
Donc un des inputs est le nombre de points testés et je divise le périmètre par le nombre de points pour obtenir un pas. .
* Une des questions que je me suis posée est comment savoir si un point/segment était à l'intérieur d'un polygone.
Je me suis servi de google,  il faut tester le nombre d'intersections avec le polygone qui doit être 2.
* Pour l'implem du test de l'intersection, je savais comment faire, absolument aucun soucis mais c'est le type d'algo où on fait vite une erreur 
(un test manquant par exemple..). Donc c'était l'occasion de tester openAI et j'ai été bluffé !!!! Il m'a donné l'algo direct implémenté en python.
Par contre il ne faut pas lui faire confiance aveuglément, il manquait un return dans son implem...
* Je crée une classe pour parcourir le Polygon avec le step calculé. Il sait aussi sur quel segment il se trouve et il a une fonction next pour la position suivante,
sachant su'il peut changer de segment.
https://github.com/chbailly/ExoPolygon/blob/71151b7d2134da66c1597b5de3aec2275f0dbcc1/ExoPolygon.cpp#L149

* L'algo principal est ici, c'est une double boucle imbriquée.
https://github.com/chbailly/ExoPolygon/blob/71151b7d2134da66c1597b5de3aec2275f0dbcc1/ExoPolygon.cpp#L219

Les quelques optims que j'ai pu mettre en place:
* Je calcule la distance en premier ca me permet d'éliminer tout de suite la possibilité si elle est inférieure à la distance maximale qu'on a calculé jusqu'ici. On économise le calcul des intersections dans ce cas.
* C'est tout bête mais inutile de calculer la distance je maximise la distance au carré, on économise un sqrt.
* Quand on part d'une position de départ, on peut partir pour la seconde position de fin directement du sommet suivant

Une des parties que je n'ai pas faite c'est ensuite d'juster précisément la solution finale, en faisant peut être un redécoupage autour des points de départ et d'arrivée (mais je pense qu'il y a peut être mieux à faire, pas sur que ce découpage soit nécessaire, je pense qu'on peut trouver directment la solution).
