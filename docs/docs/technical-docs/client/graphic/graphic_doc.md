# Overview

## **Architecture**

The SFML integration follows an abstraction pattern using interfaces to ensure flexibility, testability, and decoupling.

```
Graphics/
├── interfaces/
│   ├── IRenderer.hpp         # Rendering interface
│   ├── ISprite.hpp           # Sprite interface
│   └── ITextureManager.hpp   # Texture management interface
└── SFML/
    ├── SFMLRenderer.hpp/.cpp         # SFML rendering implementation
    ├── SFMLSpriteManagement.hpp/.cpp # Sprite wrappers using SFML
    └── SFMLTextureManager.hpp/.cpp   # SFML texture loader & cache

Input/
├── interfaces/
│   └── IInputHandler.hpp     # Input handling interface
└── SFMLInputHandler.hpp/.cpp # SFML input implementation
```

---

## **Rendering Module (SFMLRenderer)**

### **Responsibilities**

* Creation and management of the game window
* Rendering sprites and graphical elements
* Processing window events
* Managing the render cycle (clear/draw/display)

### **Public Interface**

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

### **Key Features**

#### **Window Management**

* **Creation** → `createWindow()` initializes an `sf::RenderWindow`
* **State** → `isOpen()` checks if the window is still active
* **Close** → `close()` cleanly closes the window

#### **Rendering Cycle**

* `clear()` clears the render buffer
* `display()` shows the rendered frame
* `drawSprite()` / `renderSprite()` draw graphical objects

#### **Event Handling**

* `pollEvent()` retrieves OS window events
* `isWindowCloseEvent()` detects quit requests

---

## **Sprite Management Module (SFMLSprite)**

### **Responsibilities**

* Provide a wrapper around SFML sprites
* Manage transformations (position, scale, etc.)
* Handle texture rectangle setup (for animation / atlases)

### **Public Interface**

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

### **Key Features**

#### **Transformations**

* Move sprite → `setPosition()`
* Scale sprite → `setScale()`
* Query dimensions → `getWidth()`, `getHeight()`

#### **Texture Handling**

* `setTextureRect()` selects a portion of a texture
* `getSFMLSprite()` exposes the underlying SFML sprite

---

## **Texture Management Module (SFMLTextureManager)**

### **Responsibilities**

* Load and cache textures
* Create sprites based on cached textures
* Reduce memory usage through texture reuse

### **Public Interface**

```cpp
class SFMLTextureManager : public ITextureManager {
public:
    bool loadTexture(const std::string &filePath);
    std::unique_ptr<ISprite> createSprite(const std::string &texturePath);
private:
    std::unordered_map<std::string, sf::Texture> _textures;
};
```

### **Key Features**

#### **Texture Handling**

* `loadTexture()` loads textures from disk
* Textures are stored in `unordered_map` for reuse
* `createSprite()` returns a sprite bound to an existing texture

#### **Optimizations**

* Avoid duplicate file loads
* Automatic cleanup when manager is destroyed

---

## **Input Handling Module (SFMLInputHandler)**

### **Responsibilities**

* Process keyboard/mouse events
* Map internal key enums to SFML key codes
* Provide a unified interface for game input

### **Public Interface**

```cpp
class SFMLInputHandler : public IInputHandler {
public:
    void handleEvent(const sf::Event &event);
    bool isKeyPressed(Key key) const;
};
```

### **Key Features**

#### **Event Processing**

* `handleEvent()` processes SFML input events
* `isKeyPressed()` checks current key state

#### **Key Mapping Example**

```cpp
case Key::A: sfmlKey = sf::Keyboard::A; break;
case Key::SPACE: sfmlKey = sf::Keyboard::Space; break;
```

---

## **Game Loop Integration**

### **Main Loop (GameClient::run)**

```cpp
void GameClient::run()
{
    sf::Clock clock;
    const float UPDATE_INTERVAL_MS = 16.67f; // ~60 FPS
    
    while (_renderer->isOpen()) {
        // 1. Event handling
        sf::Event event;
        while (_renderer->pollEvent(event)) {
            if (_renderer->isWindowCloseEvent(event)) {
                _renderer->close();
            }
            _inputHandler->handleEvent(event);
        }
        
        // 2. Game logic update
        // ...
        
        // 3. Rendering
        _renderer->clear();
        _gameScene->render(*_renderer);
        _renderer->display();
    }
}
```

### **Timing**

* `sf::Clock` used to measure elapsed time
* Loop targets ~60 FPS (16.67ms)

---

## **Resource Management**

### **Asset Structure**

```
client/assets/
├── sprites/
│   ├── player.png
│   ├── enemies/
│   └── ui/
├── sounds/
└── fonts/
```

### **Texture Loading Example**

```cpp
auto textureManager = std::make_unique<SFMLTextureManager>();

textureManager->loadTexture("assets/sprites/player.png");
textureManager->loadTexture("assets/sprites/enemy.png");

auto playerSprite = textureManager->createSprite("assets/sprites/player.png");
auto enemySprite = textureManager->createSprite("assets/sprites/enemy.png");
```

---

## **Error Handling**

### **Common Issues**

* Missing texture files
* Unsupported/invalid formats
* Insufficient VRAM for large textures
* Invalid graphics context

### **Recovery Strategies**

* Validate all SFML API calls
* Use fallback textures
* Log errors for debugging

---

## **Performance & Optimization**

### **Recommendations**

* **Batch rendering** to minimize draw calls
* **Texture atlases** to reduce texture switching
* **Culling** to skip off-screen objects
* **Object pooling** to avoid constant allocation

### **Profiling Tools**

* FPS counters
* Render time metrics
* Texture memory usage reports

---

## **Configuration & Settings**

### **Window Options**

* Resolution (configurable)
* Fullscreen toggle
* VSync option

### **Rendering Options**

* Anti-aliasing
* Texture filtering: Linear / Nearest
* RGBA 32-bit support

---

## **Debugging & Development Tools**

### **Debug Info**

* Render counters
* List of loaded textures
* Input event logging

### **Helpful Tools**

* SFML Inspector
* Performance overlay
* Texture viewer

---

This modular SFML architecture ensures a clean separation of responsibilities while delivering the flexibility and performance required for a fast-paced game like R-Type.
