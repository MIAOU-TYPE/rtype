# R-Type -- Game Design Document

## 1. Gameplay Global (Mécaniques de jeu)

-   Scrolling horizontal constant.
-   Le joueur déplace son vaisseau librement dans les quatre directions.
-   Le joueur peut tirer des projectiles.
-   Les ennemis apparaissent à droite et avancent vers la gauche.
-   Certains ennemis tirent des projectiles.
-   Gestion complète des collisions :
    -   Joueur / Ennemi
    -   Joueur / Projectile ennemi
    -   Projectile joueur / Ennemi
-   Lorsqu'un ennemi est détruit, il peut lâcher un bonus.
-   Le jeu continue tant que le joueur n'a pas perdu toutes ses vies.
-   Jusqu'à **4 joueurs** peuvent jouer simultanément dans une même
    partie.
-   Description purement gameplay, indépendante du code.

------------------------------------------------------------------------

## 2. Rôles des entités principales

### Joueur

-   Entité contrôlée par un humain.
-   Peut se déplacer en haut, bas, gauche, droite.
-   Peut tirer des projectiles.
-   Possède :
    -   position,
    -   vitesse,
    -   points de vie.
-   Visuellement identifiable (sprite, couleur).
-   Envoie ses inputs au serveur.

### Ennemi

-   Entité contrôlée par la logique du serveur.
-   Apparaît à droite, en haut ou en bas de l'écran.
-   Suit un pattern de mouvement simple ou avancé.
-   Peut tirer à intervalles réguliers.
-   Possède un certain nombre de points de vie.
-   Peut donner un bonus/power-up à sa destruction.

### Projectile joueur

-   Tiré par le joueur.
-   Se déplace vers la droite.
-   Inflige des dégâts aux ennemis.
-   Disparaît à l'impact.

### Projectile ennemi

-   Tiré par les ennemis.
-   Se déplace vers la gauche (angle 0° à -180°).
-   Tue ou blesse le joueur en cas d'impact.
-   Disparaît à la collision.

### Obstacles

-   Entités statiques.
-   Peuvent bloquer les entités mobiles.
-   Certains peuvent être destructibles, d'autres non.

------------------------------------------------------------------------

## 3. Résumé

**Mécaniques globales :**\
Shoot'em up horizontal basé sur l'esquive, le tir et le déplacement
constant dans un environnement défilant. Ennemis et projectiles se
dirigent depuis la droite vers les joueurs.

**Rôles des entités :**\
- *Joueur* : mobile, tire, dispose de points de vie.\
- *Ennemi* : autonome, suit un pattern, tire, peut mourir.\
- *Projectile joueur* : tir offensif vers la droite.\
- *Projectile ennemi* : tir vers la gauche, dangereux.\
- *Obstacles* : éléments statiques potentiellement destructibles.
