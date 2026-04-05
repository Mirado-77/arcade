# Arcade — Documentation des interfaces

> Cette documentation décrit les interfaces du projet Arcade et explique comment développer de nouvelles bibliothèques graphiques et de nouveaux jeux compatibles.

---

## 1. Objectif global

Le programme principal charge dynamiquement des bibliothèques graphiques et des bibliothèques de jeu via `dlopen`/`dlsym`/`dlclose`. Les bibliothèques compilées (`.so`) doivent être placées dans `./lib/`. Le système est générique : toute bibliothèque respectant les interfaces et les entrypoints décrits ici est compatible.

---

## 2. Entrypoints obligatoires

Chaque bibliothèque dynamique (graphique ou jeu) doit exporter deux fonctions en `extern "C"` :

```cpp
extern "C" {
    std::unique_ptr<arc::IDisplayModule> makeInstance(); // pour une lib graphique
    // OU
    std::unique_ptr<arc::IGameModule> makeInstance();    // pour une lib de jeu

    arc::LibType getLibType();
}
```

### `makeInstance()`

Retourne une instance unique du module. Le core appelle cette fonction après `dlopen` pour obtenir l'objet polymorphe. Aucun argument.

### `getLibType()`

Retourne le type de la bibliothèque :

```cpp
enum class LibType {
    None,
    Display,   // bibliothèque graphique
    Game,      // bibliothèque de jeu
};
```

Le core utilise `getLibType()` pour catégoriser automatiquement toute `.so` trouvée dans `./lib/`.

### Exemple minimal (bibliothèque graphique)

```cpp
#include "IDisplayModule.hpp"

class MyDisplay : public arc::IDisplayModule { /* ... */ };

extern "C" {
    std::unique_ptr<arc::IDisplayModule> makeInstance()
    {
        return std::make_unique<MyDisplay>();
    }
    arc::LibType getLibType()
    {
        return arc::LibType::Display;
    }
}
```

---

## 3. Types communs (`Arcade.hpp`)

### `Vector2f`

```cpp
using Vector2f = std::pair<float, float>;
```

Couple de coordonnées flottantes. Utilisé pour les positions et tailles des entités.

**Important** : les valeurs sont des **pourcentages** entre `0.0` et `1.0`, relatives à la surface de jeu. Le module graphique est responsable de la conversion vers ses propres coordonnées pixel ou caractère :

- `pixel_x = pos.x * largeur_fenetre`
- `pixel_y = pos.y * hauteur_zone_jeu`

### `Assets`

```cpp
using Assets = std::pair<std::vector<std::string>,
    std::vector<std::string>>;
```

- `first` : liste de chemins vers les **assets graphiques**, **sans extension** (ex. `"assets/snake/head"`). Le module graphique y ajoute `.png`, `.jpg`, etc. Pour ncurses, il cherche `<chemin>.txt` et lit la première ligne comme caractère d'affichage.
- `second` : liste de chemins vers les **assets sonores** (ex. `"assets/eat.wav"`).

Les index retournés par `IEntity::getIdx()` et `ISound::getIdx()` correspondent à ces listes.

### `Action`

Enumération de toutes les entrées possibles :

```cpp
enum class Action {
    None,
    A .. Z,
    Num0 .. Num9,
    Escape, Enter, Space, Backspace, Tab,
    Left, Right, Up, Down,
    F1 .. F15,
    LeftMouse, RightMouse, MiddleMouse,
    Close,
    // ...
};
```

`Action::None` : aucune action significative. `Action::Close` : fermeture de fenêtre.

### `Event`

```cpp
using Event = std::pair<Action, Vector2f>;
```

- `first` : l'`Action` détectée
- `second` : position associée (ex. coordonnées souris dans la fenêtre)

### `Signal`

```cpp
enum class Signal {
    LoadDisplay,   // changer de bibliothèque graphique
    LoadGame,      // changer de bibliothèque de jeu
    RestartGame,   // redémarrer le jeu courant
    BackToMenu,    // retourner au menu
    Quit,          // quitter l'application
};
```

### `Command`

```cpp
using Command = std::pair<Signal, std::vector<std::string>>;
```

Une commande émise par un jeu vers le core via `loadCommand()`. Le `Signal` indique l'action souhaitée, le `vector<string>` transporte les arguments (ex. le chemin de la lib à charger pour `LoadDisplay`/`LoadGame`).

---

## 4. Interface `IEntity`

```cpp
class IEntity {
    public:
        virtual ~IEntity() = default;

        [[nodiscard]] virtual int getIdx() noexcept = 0;
        [[nodiscard]] virtual Vector2f getPos() noexcept = 0;
        [[nodiscard]] virtual Vector2f getSize() noexcept = 0;
        [[nodiscard]] virtual float getRotation() noexcept = 0;
        [[nodiscard]] virtual RGBA getColor() noexcept = 0;
        [[nodiscard]] virtual std::optional<std::string> getStr() noexcept = 0;
};
```

### `getIdx()`

Index dans `Assets.first`. Inférieur à zéro si l'entité n'a pas d'asset graphique (entité logique invisible).

### `getPos()`

Position en **pourcentage** de la surface de jeu (valeurs entre `0.0` et `1.0`). `{0.0, 0.0}` = coin haut-gauche, `{1.0, 1.0}` = coin bas-droit.

### `getSize()`

Taille en **pourcentage** de la surface de jeu. Ex. `{0.025, 0.05}` pour une cellule dans une grille 40×20.

### `getRotation()`

Angle de rotation en degrés (sens horaire). La rotation s'applique autour du **centre** de l'entité.

### `getColor()`

Couleur RGBA : `std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>`. Utilisée comme couleur de remplacement si aucun asset graphique n'est disponible.

### `getStr()`

Chaîne optionnelle à afficher. Priorité d'affichage pour ncurses :
1. contenu du fichier `.txt` associé à l'asset (première ligne)
2. valeur de `getStr()` si présente
3. caractère ASCII de fallback : `(char)(getIdx() + 33)`

---

## 5. Interface `ISound`

```cpp
class ISound {
    public:
        virtual ~ISound() = default;

        [[nodiscard]] virtual int getIdx() noexcept = 0;
        [[nodiscard]] virtual bool isLoop() noexcept = 0;
};
```

### `getIdx()`

Index dans `Assets.second`. Le module graphique joue le son correspondant.

### `isLoop()`

Si `true`, le son doit être joué en boucle tant qu'il apparaît dans la liste. S'il disparaît d'une frame à l'autre, le module graphique doit l'arrêter.

---

## 6. Interface `IDisplayModule`

```cpp
class IDisplayModule {
    public:
        virtual ~IDisplayModule() = default;

        [[nodiscard]] virtual Event getEvent() noexcept = 0;
        [[nodiscard]] virtual int setAssets(const Assets) noexcept = 0;
        virtual void drawGame(const std::pair<Entities, Sounds>) noexcept = 0;
};
```

### `getEvent()`

Retourne le prochain événement utilisateur. Doit être non-bloquant (retourne `{Action::None, {0,0}}` si aucun événement).

### `setAssets(const Assets)`

Appelé par le core à chaque changement de jeu. Le module graphique doit charger les textures, sons et assets textuels correspondants. Retourne `0` en cas de succès, `-1` en cas d'erreur.

### `drawGame(const std::pair<Entities, Sounds>)`

Dessine une frame. Appelé une fois par tick après `simulate()`. L'implémentation doit :
1. effacer l'écran
2. pour chaque entité : calculer `pos_pixel = pos_pct * taille_fenetre`, `size_pixel = size_pct * taille_fenetre`, afficher l'asset ou un fallback coloré
3. gérer les sons (lancer, boucler, arrêter selon la liste `Sounds`)
4. afficher le résultat

---

## 7. Interface `IGameModule`

```cpp
class IGameModule {
    public:
        virtual ~IGameModule() = default;

        virtual void simulate(const Event) noexcept = 0;
        [[nodiscard]] virtual Assets getAssets() noexcept = 0;
        [[nodiscard]] virtual std::pair<Entities, Sounds> getElements() noexcept = 0;
        [[nodiscard]] virtual bool isOpen() noexcept = 0;
        [[nodiscard]] virtual std::vector<Command> loadCommand() noexcept = 0;
};
```

### `simulate(const Event)`

Avance la logique du jeu d'un tick. Reçoit l'événement utilisateur courant.

### `getAssets()`

Retourne les assets nécessaires. Appelé par le core après le chargement du jeu pour transmettre les chemins au module graphique via `setAssets()`.

### `getElements()`

Retourne l'état visuel et sonore courant. Appelé chaque tick après `simulate()`.

### `isOpen()`

Retourne `false` quand le jeu est terminé. Le core appelle alors `restartGame()`.

### `loadCommand()`

Retourne la liste des commandes à exécuter par le core ce tick. Exemples :

```cpp
// Changer de lib graphique
return {{{arc::Signal::LoadDisplay, {"./lib/arcade_sfml.so"}}}};

// Quitter
return {{{arc::Signal::Quit, {}}}};

// Aucune commande (cas normal)
return {};
```

---

## 8. Créer une nouvelle bibliothèque graphique

1. Créer un répertoire `plugins/MonDisplay/`
2. Implémenter `MonDisplay.hpp` et `MonDisplay.cpp` avec la classe héritant de `arc::IDisplayModule`
3. Exporter les entrypoints `makeInstance()` et `getLibType()` (retourne `arc::LibType::Display`)
4. Ajouter à `CMakeLists.txt` :

```cmake
add_library(arcade_mondisplay SHARED
    plugins/MonDisplay/MonDisplay.cpp
)
target_include_directories(arcade_mondisplay PRIVATE include)
target_link_libraries(arcade_mondisplay PRIVATE <vos_libs>)
set_target_properties(arcade_mondisplay PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    PREFIX "")
```

5. Placer le `.so` dans `./lib/` — le core le détecte automatiquement

**Règles de rendu :**
- `pos` et `size` sont en pourcentage → multiplier par les dimensions de la fenêtre
- La rotation est en degrés, autour du centre de l'entité
- Fallback si pas d'asset : rectangle de couleur `getColor()` avec texte `getStr()` ou caractère ASCII

---

## 9. Créer un nouveau jeu

1. Créer un répertoire `plugins/MonJeu/`
2. Implémenter la classe héritant de `arc::IGameModule`
3. Exporter les entrypoints `makeInstance()` et `getLibType()` (retourne `arc::LibType::Game`)
4. Ajouter à `CMakeLists.txt` (même structure que ci-dessus)

**Règles des entités :**
- `getPos()` retourne des fractions de la surface de jeu (ex. cellule `(x, y)` dans une grille `W×H` → `{(float)x/W, (float)y/H}`)
- `getSize()` retourne la taille d'une cellule en pourcentage (ex. `{1.0f/W, 1.0f/H}`)
- `getIdx()` < 0 pour une entité invisible (non affichée)
- `loadCommand()` retourne `{}` si aucune commande ce tick

**Assets textuels (ncurses) :**
Pour que ncurses affiche un caractère spécifique pour un asset, créer un fichier `<chemin_asset>.txt` contenant le caractère ou la chaîne sur la première ligne. Ex. pour `"assets/snake/head"`, créer `assets/snake/head.txt` avec `S`.

