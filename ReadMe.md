# **Projet IA – Sauvetage de Patients**

## Hana DELCOURT

## **Description**
Ce projet met en place une intelligence artificielle où un joueur et plusieurs IA doivent récupérer des patients de manière efficace, sans communication entre eux. Les IA suivent un algorithme basé sur Dijkstra pour se déplacer vers les patients tout en optimisant leurs choix.

## **Gameplay**
- **Joueur** :  
  - Se déplace avec les touches **ZQSD**.  
  - Ne peut tourner que lorsqu'il avance ou recule.  

- **IA** :  
  - Elles font l'hypothèse que **le personnage le plus proche d'un patient** ira vers celui-ci, et choisissent donc un autre patient à récupérer.  
  - Suivent un **chemin balisé** utilisé pour **Dijkstra**, permettant de calculer un trajet optimal.  
  - Se **réactualisent tous les 250 ticks** pour recalculer leurs déplacements.  

## **Technologies et Algorithmes Utilisés**
- **Pathfinding** : Algorithme de **Dijkstra** appliqué sur un chemin balisé.  
- **Système de décision** : Attribution des patients aux IA en fonction de la proximité.  
- **Mise à jour périodique** : Réévaluation des positions et décisions toutes les **250 ticks**.  

## **Objectifs du Projet**
- Expérimenter **l'optimisation des déplacements** sans communication entre entités.  
- Implémenter une **gestion efficace du pathfinding** via un graphe de déplacements.  
- Créer un gameplay équilibré entre **intervention humaine et IA**.  


## Lien

Voici, ici, le lien vers le build sur mon drive: https://drive.google.com/drive/folders/1Bqefc41s2TVsoz4FpyBvWW8NQXHVSHQG?usp=sharing