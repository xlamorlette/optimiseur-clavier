# optimiseur-clavier
Programme calculant la disposition optimale des touches sur un clavier.

Ce programme prend en entr�e un fichier contenant les fr�quences d'apparition des lettres et des bigrammes les plus fr�quents (fr�quences normalis�es et exprim�es en pour mille), et en d�duit la r�partition optimale des touches entre les deux mains en maximisant l'�quilibre et l'alternance entre les deux mains.

Les explications d�taill�es du contexte et quelqes notes sur l'algorithme sont sur la page suivante : http://www.xavierlamorlette.fr/clavier.

Ce programme est �crit en C++ :

* Il utilise certaines de nombreuses nouvelles fonctionnalit�s syntaxiques de C++11. Le d�veloppement a �t� fait avec gcc 4.8.2 (compilateur fournit avec Ubuntu 14.04).
* Il utilise la biblioth�que Boost. Le d�veloppement a �t� fait avec avec la version 1.54.0 (obtenue sous Ubuntu via `apt-get install libboost-all-dev`).

Pour compiler le programme, lancez simplement la commande `make`.

Note : si les lettres accentuees dans un fichier de statistiques ne sont pas reconnues correctement par le programme, il peut etre utile de le transformer en UTF8 comme suit :

```
iconv frequences_lettres.txt -f iso-8859-1 -t UTF-8 -o frequences_lettres.txt
```
