# Travail pratique 3 : Le jeu *Circles*

## Description

* Il s'agit d'un jeu simple créé en utilisant SDL2. Le jeu consiste à contrôler un triangle pour éviter des obstacles en tournant autour d'un cercle au centre de l'écran. L'objectif est de survivre le plus longtemps possible.

* Ceci est le troisieme projet dans le cadre du cours [inf3135](https://etudier.uqam.ca/cours?sigle=INF3135)
enseigné par Serge,Dogny à l'Université du Québec à Montréal (UQAM).

## Auteurs

- Yacine et Ferrat (FERY14099608)

## Dépendances

Ce projet nécessite les bibliothèques suivantes :

- SDL2 : https://www.libsdl.org/download-2.0.php
- SDL2_image : https://www.libsdl.org/projects/SDL_image/
- SDL2_gfx : https://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/index.html

Veuillez vous référer aux sites officiels pour les instructions d'installation.

## Fonctionnement

1. Compilez le projet en utilisant le Makefile fourni. `make`
2. Exécutez le binaire compilé pour démarrer le jeu. `./circles`
3. Utilisez les flèches gauche et droite pour vous déplacer autour du cercle et éviter les obstacles.
4. Survivez le plus longtemps possible pour obtenir un score élevé.


## Plateformes supportées

Indiquez toutes les plateformes sur lesquelles vous avez testé l'application,
avec la version.

## Références

Le jeu a été testé sur les plateformes suivantes :

- Linux (Garuda)

## Division des tâches

Donnez ici une liste des tâches de chacun des membres de l'équipe. Utilisez la
syntaxe suivante (les crochets vides indiquent que la tâche n'est pas
complétée, les crochets avec un `X` indique que la tâche est complétée):

* [x] Gestion du menu
* [x] Affichage de la scène
* [x] Affichage du chronomètre
* [x] Animation des murs
* [X] Animation du joueur
* [x] Détection des collisions
* [x] Affichage d'une partie terminée
* [ ] Gestion de la musique principale
* [ ] Gestion des sons lorsqu'on navigue dans le menu
* [ ] Gestion de la musique de fin de partie

## État du projet

Il manque la gestion de la difficulté.

