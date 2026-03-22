# SAE-S2-02 - Temps de Route

## Navyra : Carte Interactive & Calcul d'Itinéraires

Navyra est une application de bureau développée en C++ avec le framework Qt. Elle permet de visualiser une carte interactive de la France, d'explorer les villes et leurs données administratives, de calculer le trajet le plus court entre deux points, et de générer des matrices personnalisées de temps de trajet.

### ✨ Fonctionnalités Principales

* **Carte Interactive Hautes Performances :**
  * Rendu vectoriel fluide avec `QGraphicsScene` / `QGraphicsView`.
  * Navigation intuitive (Zoom à la molette, déplacement par glisser-déposer).
  * Système de LOD (*Level of Detail*) dynamique : l'affichage des villes et de leurs noms s'adapte intelligemment au niveau de zoom et à la taille de la population.
* **Calcul du Plus Court Chemin :**
  * Implémentation de l'algorithme de **Floyd-Warshall** pour déterminer le trajet optimal entre n'importe quelles villes connectées.
  * Affichage visuel du trajet sur la carte avec mise en évidence des segments et de leurs poids (distances/temps).
* **Tableau de Bord & Outils :**
  * Barres de recherche intelligente avec auto-complétion pour trouver rapidement une ville.
  * Outil de génération de matrices (Tableau à double entrée) pour comparer les temps de trajet entre une sélection personnalisée de plusieurs villes.
  * Affichage des informations détaillées au clic sur une ville (Population intra-muros/agglomération, Région, Pays, Coordonnées).

### 🛠️ Technologies Utilisées

* **Langage :** C++
* **Interface Graphique :** Qt Framework (Core, Gui, Widgets)
* **Algorithmique :** Graphes pondérés, Algorithme de Floyd-Warshall.
* **Format de données :** CSV (Villes et Connexions), GeoJSON (Frontières et Régions).

### 🚀 Installation et Lancement

1. Clonez ce dépôt sur votre machine.
2. Ouvrez le projet dans **Qt Creator** (ouvrez le fichier `.pro`).
3. Assurez-vous d'avoir configuré un kit de compilation valide (ex: MinGW ou MSVC pour Windows, GCC pour Linux).
4. Compilez et lancez l'application (Bouton "Run" ou `Ctrl+R`).

*(Note : Les ressources GeoJSON et CSV doivent être correctement liées dans le fichier `.qrc` pour être chargées au démarrage).*

### 📂 Structure du Projet

* `src/InteractiveMap/` : Gestion du rendu graphique (Scène, Vue, Éléments de la carte, Panneau de contrôle).
* `src/Map/` : Cœur de la logique et algorithmique (Graphe, Villes, Administrations, Calcul des chemins).
* `src/utils/` : Outils de désérialisation (Parsers CSV et GeoJSON).
* `assets/datas/` : Jeux de données (Villes, Temps de parcours, Frontières).

### 👏 Crédits

* **Développement :** [GHYS Nathan]
* **Données Cartographiques (GeoJSON) :** Grégoire David - [Github france-geojson](https://github.com/gregoiredavid/france-geojson)