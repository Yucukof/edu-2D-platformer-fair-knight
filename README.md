Ref: 1819_IHDCB132_Bailly_Hadrien
# Progra2
Code pour le projet de programmation 2 (IHDCB132) - Étudiant: Hadrien BAILLY

Libraries externes utilisées:
* [GLUT](http://freeglut.sourceforge.net) (freeglut3-dev)
* [SOIL](https://www.lonesock.net/soil.html) (libsoil-dev)
* [freetype](https://www.freetype.org) (libfreetype6-dev)

Sprites by 0x72: 
* [16x16 Dungeon Tileset](https://0x72.itch.io/16x16-dungeon-tileset)
* [16x16 DungeonTileset II](https://0x72.itch.io/dungeontileset-ii)

## **Projet** : Jumping Bananas :monkey:
* Il s'agit de développer en C le jeu video _Jumping Bananas_:
  * Jeu de plateau
  * 1 joueur

On s'appuiera sur la bibliothèque [OpenGL](https://www.opengl.org) pour la gestion de l'interface graphique

### Objectifs
* Maitrise du langage de programmation C,
* Connaissance de l'environnement Linux, Windows, Mac,
* Compilateur gcc (`Makefile`),
* Utilisation d'une bibliothèque de fonctions externes.

### Requis
* Connaissances de base du langage C,
* Utilisation des types des données complexes :
> struct, array
* Gestion dynamique de la mémoire:
> malloc (), free ()

### Délivrable
1 Seul mode de jeu :
* Human Vs Machine.
> Dans un premier temps, savoir déplacer le personnage dans le monde (ou labyrinthe) avec les touches fléchées

* Implémentation d'une stratégie automatique de jeu.
> par exemple, le mouvement aléatoire des ennemis

## **Phases**
### Phase 1 : Analyse et spécification

Compréhension du problème et spécification d'une solution
* Choix des structures de données
* Types et constantes et leurs conventions de représentation
* Définition du/des écrans + enchaînement
* Description de la découpe en sous-problèmes
* Délivrable : rapport respectant la structure décrite dans le document “Comment rédiger un rapport associé à un exercice de programmation” via Webcampus

`Échéance : 28/02`

### Phase 2 : Écriture du programme
* Implémentation du programme tel que décrit lors de la
phase 1 en ANSI C 1999
```gcc -Wall -std=c99 *.c –o JumpingBananas```
* Utilisation autorisée de stdio, stdlib, string.
* Délivrable : code source via Webcampus

`Échéance : 01/04`

### Phase 3 : Amélioration de la stratégie automatique
* Carte blanche pour améliorer la stratégie de jeu
* Adaptation et enrichissement du rapport avec une description de la stratégie utilisée
* Délivrable : code source et rapport via Webcampus

`Échéance : 06/05`
