![C](https://img.shields.io/badge/language-C-blue.svg) ![CLI](https://img.shields.io/badge/type-Console-lightgrey.svg)


Projet C – Supémon (CLI Pokémon Game)

Contexte

L’objectif était de développer une version console (CLI) du célèbre jeu Pokémon, rebaptisée Supémon, en respectant une architecture modulaire, des structures de données cohérentes et une logique de combat complète.

Le jeu a été entièrement développé en langage C, sans interface graphique.


Fonctionnalités principales

Lancement du programme
- Le joueur peut :
  - Créer un nouveau profil (nom + choix du premier Supémon)
  - Charger une sauvegarde existante à partir d’un fichier texte



Actions hors combat
Explorer la nature → déclenche un combat aléatoire contre un Supémon sauvage  
Boutique (Shop) → achat ou vente d’objets avec les Supcoins gagnés en combat  
- Centre Supémon → soigne gratuitement tous les Supémons du joueur  
- Quitter le jeu → avec ou sans sauvegarde  



Joueur
Le joueur possède :
- Un nom
- Une liste de Supémons
- Un Supémon actif pour le combat
- Des Supcoins pour acheter des objets
- Un inventaire d’objets

---

Supémons
Chaque Supémon a :
- Un nom, niveau, expérience
- Des statistiques : **HP**, **Attaque**, **Défense**, **Évasion**, **Précision**, **Vitesse**
- Une liste de mouvements
- Un système de progression (gain d’expérience et montée de niveau)




 Système de combat
Les combats se déroulent tour par tour :
Le Supémon le plus rapide commence.
Le joueur peut :
  - Attaquer (choisir une capacité)
  - Changer de Supémon
  - Utiliser un objet
  - Fuir le combat
  - Tenter une capture

Les attaques sont influencées par les statistiques (Attaque/Défense, Évasion/Précision).  
Le taux de réussite d’une attaque ou d’une fuite est calculé avec des formules de probabilité.

En cas de victoire :
- Le joueur gagne entre 100 et 500 Supcoins
- Le Supémon actif gagne de l’expérience proportionnelle au niveau de l’ennemi


Expérience et niveau
- Passage de niveau : +30 % sur toutes les statistiques  
- Niveau 1 → 2 : 500 XP  
- Chaque niveau suivant demande +1000 XP supplémentaires  



Sauvegarde
Le jeu crée un fichier de sauvegarde contenant :
- Le nom du joueur  
- La liste des Supémons (statistiques, niveau, expérience)  
- L’inventaire et le nombre de Supcoins  

Le joueur peut reprendre sa partie à tout moment depuis ce fichier.
