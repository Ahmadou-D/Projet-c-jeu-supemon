# 👾 Supémon - Console RPG Game (Langage C)

![C](https://img.shields.io/badge/C-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white) ![CLI](https://img.shields.io/badge/Interface-Terminal%20%2F%20CLI-black?style=for-the-badge) ![Game Dev](https://img.shields.io/badge/Game_Dev-RPG-purple?style=for-the-badge)

## 📝 Présentation du projet
**Supémon** est un jeu de rôle au tour par tour (fortement inspiré de la célèbre franchise Pokémon), développé intégralement en **Langage C** et jouable en ligne de commande (CLI). 

L'objectif de ce projet était de concevoir une architecture logicielle modulaire, en manipulant des structures de données complexes et des algorithmes de jeu (génération aléatoire, calcul de dégâts, persistance des données) sans recourir à une interface graphique.

## 🚀 Fonctionnalités Principales

### 👤 Gestion de Profil et Persistance
* **Création / Chargement** : Possibilité de démarrer une nouvelle aventure avec un Supémon de départ ou de charger une progression existante.
* **Sauvegarde (File I/O)** : Le jeu génère un fichier texte persistant contenant les données du joueur (nom, Supcoins), son inventaire, et l'état exact de son équipe de Supémons (niveaux, XP, statistiques actuelles).

### 🌍 Univers de Jeu (Hors Combat)
* **Explorer la nature** : Déclenche une rencontre aléatoire avec un Supémon sauvage.
* **Boutique (Shop)** : Système d'économie permettant l'achat et la vente d'objets via la monnaie du jeu (Supcoins).
* **Centre Supémon** : Zone de repos pour restaurer intégralement les points de vie (HP) de l'équipe.

### ⚔️ Système de Combat Avancé
Le moteur de combat au tour par tour repose sur des mécaniques statistiques poussées :
* **Initiative** : Le Supémon possédant la statistique de `Vitesse` la plus élevée frappe en premier.
* **Actions tactiques** : Le joueur peut Attaquer, Changer de Supémon, Utiliser un objet, Fuir ou Tenter une capture.
* **Algorithmique & Probabilités** : Le succès des attaques, des captures et des fuites est calculé dynamiquement grâce à des formules mathématiques croisant les statistiques d'Évasion et de Précision.

## 📈 Système de Progression (RPG)
* **Récompenses** : En cas de victoire, le joueur remporte entre 100 et 500 Supcoins, et le Supémon actif engrange de l'expérience (XP) proportionnelle au niveau de l'ennemi.
* **Leveling** : Un système d'expérience évolutif (Niveau 2 atteint à 500 XP, avec un palier incrémental de +1000 XP par niveau supplémentaire).
* **Scaling** : À chaque montée de niveau, l'ensemble des statistiques du Supémon (HP, Attaque, Défense, Vitesse, etc.) augmente de **30%**.

## 🛠️ Compétences Techniques Démontrées
* **Langage C** : Maîtrise de la gestion mémoire, des pointeurs et de la logique de programmation bas niveau.
* **Structures de Données** : Utilisation intensive des `struct` pour modéliser les entités du jeu (Joueur, Supémon, Inventaire, Compétences).
* **Gestion de Fichiers** : Lecture et écriture de fichiers (`fopen`, `fprintf`, `fscanf`) pour le système de sauvegarde.
* **Architecture Logicielle** : Implémentation d'une boucle de jeu robuste (Game Loop) et modularité du code.
