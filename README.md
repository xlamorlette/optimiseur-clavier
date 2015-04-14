# optimiseur-clavier
Programme calculant la disposition optimale des touches sur un clavier.

Ce programme prend en entrée un fichier contenant les fréquences d'apparition des lettres et des bigrammes les plus fréquents (fréquences normalisées et exprimées en pour mille), et en déduit la répartition optimale des touches entre les deux mains en maximisant l'équilibre et l'alternance entre les deux mains.

Les explications détaillées du contexte et de l'algorithme sont sur la page suivante : http://www.xavierlamorlette.fr/clavier.

Ce programme est écrit en C++ :

* Il utilise certaines nouvelles fonctionnalités de C++11. Le développement a été fait avec gcc 4.8.2 (compilateur fournit avec Ubuntu 14.04).
* Il utilise la bibliothèque Boost. Le développement a été fait avec avec la version 1.54.0 (obtenue sous Ubuntu via `apt-get install libboost-all-dev`).

Pour compiler le programme, lancez simplement la commande `make`.
