# Documentation technique SFML - Client R-Type

## Vue d'ensemble

SFML (Simple and Fast Multimedia Library) est utilisé dans le client R-Type pour gérer les aspects graphiques et d'entrées utilisateur. Cette documentation détaille l'implémentation et l'architecture des modules SFML dans le projet.

## Architecture

L'intégration SFML suit un pattern d'abstraction avec des interfaces permettant la flexibilité et la testabilité :

```
Graphics/
├── interfaces/
│   ├── IRenderer.hpp         # Interface de rendu
│   ├── ISprite.hpp          # Interface sprite
│   └── ITextureManager.hpp  # Interface gestion textures
└── SFML/
    ├── SFMLRenderer.hpp/.cpp        # Implémentation rendu SFML
    ├── SFMLSpriteManagement.hpp/.cpp # Gestion sprites SFML
    └── SFMLTextureManager.hpp/.cpp   # Gestion textures SFML

Input/
├── interfaces/
│   └── IInputHandler.hpp    # Interface gestion entrées
└── SFMLInputHandler.hpp/.cpp # Implémentation entrées SFML
```

## Module de Rendu (SFMLRenderer)

### Responsabilités
- Création et gestion de la fenêtre de jeu
- Rendu des sprites et éléments graphiques
- Gestion des événements de fenêtre
- Contrôle du cycle de rendu (clear/display)

### Interface publique

```cpp
class SFMLRenderer : public IRenderer {
public:
    void createWindow(unsigned int width, unsigned int height, const std::string &title);
    bool isOpen() const;
    void close();
    void clear();
    void display();
    bool pollEvent(sf::Event &event);
    bool isWindowCloseEvent(const sf::Event &event) const;
    void drawSprite(const sf::Sprite &sprite);
    void renderSprite(const ISprite &sprite);
    unsigned int getWindowWidth() const;
    unsigned int getWindowHeight() const;
};
```

### Fonctionnalités clés

#### Gestion de la fenêtre
- **Création** : `createWindow()` initialise une `sf::RenderWindow` avec les dimensions et titre spécifiés
- **État** : `isOpen()` vérifie si la fenêtre est active
- **Fermeture** : `close()` ferme proprement la fenêtre

#### Cycle de rendu
- **Effacement** : `clear()` nettoie le buffer de rendu
- **Affichage** : `display()` présente le frame rendu à l'écran
- **Dessin** : `drawSprite()` et `renderSprite()` pour afficher les éléments graphiques

#### Gestion des événements
- **Polling** : `pollEvent()` récupère les événements système
- **Détection fermeture** : `isWindowCloseEvent()` identifie les événements de fermeture

## Module de Gestion des Sprites (SFMLSprite)

### Responsabilités
- Encapsulation des sprites SFML
- Gestion des transformations (position, échelle)
- Configuration des rectangles de texture (pour les animations/atlas)

### Interface publique

```cpp
class SFMLSprite : public ISprite {
public:
    explicit SFMLSprite(sf::Sprite sprite);
    void setPosition(float x, float y);
    float getWidth() const;
    float getHeight() const;
    void setScale(float scaleX, float scaleY);
    void setTextureRect(int left, int top, int width, int height);
    const sf::Sprite &getSFMLSprite() const;
};
```

### Fonctionnalités clés

#### Transformations
- **Position** : `setPosition()` définit les coordonnées x,y du sprite
- **Échelle** : `setScale()` applique un facteur de zoom
- **Dimensions** : `getWidth()` et `getHeight()` retournent les dimensions

#### Gestion des textures
- **Rectangle de texture** : `setTextureRect()` définit la région de texture à afficher
- **Accès SFML** : `getSFMLSprite()` retourne le sprite SFML natif

## Module de Gestion des Textures (SFMLTextureManager)

### Responsabilités
- Chargement et mise en cache des textures
- Création de sprites à partir des textures
- Optimisation mémoire par réutilisation des textures

### Interface publique

```cpp
class SFMLTextureManager : public ITextureManager {
public:
    bool loadTexture(const std::string &filePath);
    std::unique_ptr<ISprite> createSprite(const std::string &texturePath);
private:
    std::unordered_map<std::string, sf::Texture> _textures;
};
```

### Fonctionnalités clés

#### Gestion des textures
- **Chargement** : `loadTexture()` charge une texture depuis un fichier
- **Cache** : Les textures sont stockées dans une `std::unordered_map` pour éviter les rechargements
- **Création sprites** : `createSprite()` génère des sprites à partir des textures chargées

#### Optimisation
- **Réutilisation** : Une texture chargée peut servir à créer plusieurs sprites
- **Gestion mémoire** : Les textures sont automatiquement détruites avec le manager

## Module de Gestion des Entrées (SFMLInputHandler)

### Responsabilités
- Traitement des événements clavier/souris
- Abstraction des codes de touches SFML
- Interface unifiée pour la gestion des entrées

### Interface publique

```cpp
class SFMLInputHandler : public IInputHandler {
public:
    void handleEvent(const sf::Event &event);
    bool isKeyPressed(Key key) const;
};
```

### Fonctionnalités clés

#### Gestion des événements
- **Traitement** : `handleEvent()` process les événements SFML
- **État clavier** : `isKeyPressed()` vérifie l'état d'une touche

#### Mapping des touches
Le système mappe les énumérations internes vers les codes SFML :

```cpp
// Exemple de mapping
case Key::A: sfmlKey = sf::Keyboard::A; break;
case Key::SPACE: sfmlKey = sf::Keyboard::Space; break;
// ... autres mappings
```

## Intégration dans la Boucle de Jeu

### Cycle principal (GameClient::run())

```cpp
void GameClient::run()
{
    sf::Clock clock;
    const float UPDATE_INTERVAL_MS = 16.67f; // ~60 FPS
    
    while (_renderer->isOpen()) {
        // 1. Gestion des événements
        sf::Event event;
        while (_renderer->pollEvent(event)) {
            if (_renderer->isWindowCloseEvent(event)) {
                _renderer->close();
            }
            _inputHandler->handleEvent(event);
        }
        
        // 2. Mise à jour logique du jeu
        // ...
        
        // 3. Rendu
        _renderer->clear();
        _gameScene->render(*_renderer);
        _renderer->display();
    }
}
```

### Synchronisation
- **Clock SFML** : `sf::Clock` pour mesurer le temps écoulé
- **Fréquence fixe** : Boucle ciblent ~60 FPS (16.67ms par frame)

## Gestion des Ressources

### Structure des Assets

```
client/assets/
├── sprites/          # Textures et images
│   ├── player.png
│   ├── enemies/
│   └── ui/
├── sounds/           # Fichiers audio
└── fonts/           # Polices de caractères
```

### Chargement des textures

```cpp
// Initialisation du gestionnaire
auto textureManager = std::make_unique<SFMLTextureManager>();

// Chargement des textures
textureManager->loadTexture("assets/sprites/player.png");
textureManager->loadTexture("assets/sprites/enemy.png");

// Création de sprites
auto playerSprite = textureManager->createSprite("assets/sprites/player.png");
auto enemySprite = textureManager->createSprite("assets/sprites/enemy.png");
```

## Gestion des Erreurs

### Types d'erreurs courantes
- **Fichiers manquants** : Textures introuvables
- **Format invalide** : Fichiers corrompus ou format non supporté
- **Mémoire insuffisante** : Textures trop volumineuses
- **Fenêtre invalide** : Problèmes de contexte graphique

### Stratégies de récupération
- **Validation** : Vérification systématique des retours de fonction SFML
- **Fallback** : Textures par défaut en cas d'échec de chargement
- **Logging** : Enregistrement détaillé des erreurs pour le débogage

## Performance et Optimisations

### Recommandations
- **Batch rendering** : Grouper les appels de rendu similaires
- **Texture atlasing** : Combiner plusieurs petites textures en une grande
- **Culling** : Ne pas rendre les éléments hors écran
- **Pool d'objets** : Réutiliser les sprites plutôt que les recréer

### Profilage
- **Compteurs FPS** : Surveillance des performances en temps réel
- **Temps de rendu** : Mesure du temps passé dans chaque phase
- **Utilisation mémoire** : Monitoring de l'usage des textures

## Configuration et Paramètres

### Paramètres de fenêtre
- **Résolution** : Configurable via les paramètres du client
- **Mode plein écran** : Support du basculement windowed/fullscreen
- **VSync** : Synchronisation verticale optionnelle

### Paramètres de rendu
- **Anti-aliasing** : Configuration du lissage
- **Filtrage des textures** : Linear/Nearest selon les besoins
- **Format de couleur** : Support RGBA 32-bits

## Debugging et Outils de Développement

### Informations de debug
- **Compteurs de rendu** : Nombre de sprites dessinés par frame
- **État des textures** : Liste des textures chargées en mémoire
- **Événements d'entrée** : Log des actions utilisateur

### Outils utiles
- **SFML Inspector** : Visualisation des ressources chargées
- **Performance overlay** : Affichage temps réel des métriques
- **Texture viewer** : Prévisualisation des assets chargés

Cette architecture SFML modulaire permet une séparation claire des responsabilités tout en conservant les performances et la flexibilité nécessaires pour un jeu comme R-Type.