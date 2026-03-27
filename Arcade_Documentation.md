# Arcade — Documentation provisoire des interfaces

> Cette documentation décrit **l’état actuel** des interfaces `Arcade.hpp`, `IEntity.hpp`, `ISound.hpp`, `IDisplayModule.hpp` et `IGameModule.hpp`.
---

## 1. Objectif global

Dans le projet Arcade, le programme principal charge dynamiquement :

- des **bibliothèques graphiques** ;
- des **bibliothèques de jeu**.

Le sujet impose que ces bibliothèques soient chargées au runtime avec `dlopen`, `dlsym`, `dlclose` et `dlerror`, et précise que le core ne doit pas dépendre explicitement d’une bibliothèque graphique donnée. Les bibliothèques doivent être placées dans `./lib/`, et le système doit rester générique, afin de permettre l’échange d’interfaces et de modules avec d’autres groupes. Le sujet demande aussi une documentation expliquant comment développer de nouvelles bibliothèques graphiques et de nouveaux jeux compatibles. Enfin, il impose le partage d’interface avec au moins un autre groupe. 

---

## 2. Vue d’ensemble des fichiers

Les interfaces actuellement prévues sont :

- `Arcade.hpp` : types partagés de base ;
- `IEntity.hpp` : abstraction d’un élément affichable ;
- `ISound.hpp` : abstraction d’un son déclenché par le jeu ;
- `IDisplayModule.hpp` : contrat d’une bibliothèque graphique ;
- `IGameModule.hpp` : contrat d’une bibliothèque de jeu.

L’idée générale est la suivante :

1. le **core** récupère les événements utilisateur via un module graphique ;
2. il transmet ces événements au **module de jeu** ;
3. le jeu simule un tick et renvoie des **entités** et des **sons** ;
4. le module graphique dessine ces éléments avec les assets correspondants.

---

## 3. Documentation de `Arcade.hpp`

`Arcade.hpp` centralise plusieurs types communs utilisés par les autres interfaces.

### 3.1 `Vector2f`

```cpp
using Vector2f = std::pair<float, float>;
```

`Vector2f` représente un couple de coordonnées.

Usage attendu :

- position d’une entité ;
- taille d’une entité ;

### 3.2 `Assets`

```cpp
using Assets = std::pair<std::vector<std::string>,
    std::vector<std::string>>;
```

`Assets` représente l’ensemble des ressources nécessaires à un jeu.

Les deux listes correspondent à :

- une liste pour les **assets graphiques** (sans l'extension de fichier) ;
- une liste pour les **assets sonores** (avec l'extension de fichier).

Chaque entrée correspond à un chemin / un nom d’asset.

### 3.3 `Action`

```cpp
enum class Action { ... };
```

`Action` est l’énumération de toutes les actions d’entrée connues par le système.

Elle contient notamment :

- les lettres `A` à `Z` ;
- les chiffres `Num0` à `Num9` ;
- les touches de contrôle (`Escape`, `Enter`, `Tab`, etc.) ;
- les flèches (`Left`, `Right`, `Up`, `Down`) ;
- les touches de fonction (`F1` à `F15`) ;
- les boutons de souris ;
- `Close` pour signaler une fermeture de fenêtre ;
- `None` pour une touche non gérée ou l’absence d’action significative.

Cette énumération sert de format d’entrée unifié entre bibliothèques graphiques et jeux.

### 3.4 `Event`

```cpp
using Event = std::pair<Action, Vector2f>;
```

`Event` regroupe :

- une `Action` ;
- un `Vector2f`.

Usage attendu :

- l’`Action` indique la touche ou l’événement principal ;
- le `Vector2f` transporte une information de position, par exemple pour la souris.

Cela permet à une lib graphique de remonter aussi bien des entrées clavier que des événements liés à une position à l’écran.

---

## 4. Documentation de `IEntity.hpp`

`IEntity` décrit une entité rendered par une bibliothèque graphique.

### 4.1 Alias `RGBA`

```cpp
using RGBA = std::tuple<unsigned char, unsigned char,
    unsigned char, unsigned char>;
```

`RGBA` représente une couleur composée de quatre canaux :

- rouge ;
- vert ;
- bleu ;
- alpha.

### 4.2 Interface `IEntity`

```cpp
class IEntity {
    public:
        virtual ~IEntity() = default;

        virtual int getIdx() = 0;
        virtual Vector2f getPos() = 0;
        virtual Vector2f getSize() = 0;
        virtual float getRotation() = 0;
        virtual RGBA getColor() = 0;
        virtual std::optional<std::string> getStr() = 0;
};
```

#### Rôle

Une `IEntity` fournit toutes les informations minimales nécessaires à l’affichage d’un objet.

#### Méthodes

##### `getIdx()`
Retourne un entier identifiant l’asset à utiliser.

##### `getPos()`
Retourne la position de l’entité.

##### `getSize()`
Retourne la taille de l’entité.

##### `getRotation()`
Retourne l’angle de rotation de l’entité.

##### `getColor()`
Retourne la couleur RGBA appliquée à l’entité.

##### `getStr()`
Retourne éventuellement une chaîne.

### 4.3 Alias `Entities`

```cpp
using Entities = std::vector<std::unique_ptr<arc::IEntity>>;
```

`Entities` représente l’ensemble des entités à afficher pour un frame donné.

Le choix de `std::unique_ptr<IEntity>` signifie que le propriétaire des entités est explicitement transféré ou déplacé, et que chaque entité est polymorphe.

---

## 5. Documentation de `ISound.hpp`

`ISound` représente une information sonore produite par le jeu.

### 5.1 Interface `ISound`

```cpp
class ISound {
    public:
        virtual ~ISound() = default;

        virtual int getIdx() = 0;
        virtual bool isLoop() = 0;
};
```

#### Rôle

Cette interface décrit un son à jouer ou à gérer côté affichage / moteur audio.

#### Méthodes

##### `getIdx()`
Retourne l’identifiant du son à utiliser.

Par analogie avec `IEntity::getIdx()`, s’agit d’un index dans la liste des assets sonores.

##### `isLoop()`
Indique si le son doit être joué en boucle.

### 5.2 Alias `Sounds`

```cpp
using Sounds = std::vector<std::unique_ptr<arc::ISound>>;
```

`Sounds` représente l’ensemble des événements sonores du tick courant.

---

## 6. Documentation de `IDisplayModule.hpp`

`IDisplayModule` définit le contrat que doit respecter toute bibliothèque graphique.

```cpp
class IDisplayModule {
    public:
        virtual ~IDisplayModule() = default;

        virtual Event getEvent() = 0;
        virtual int setAssets(Assets) = 0;
        virtual void drawGame(std::reference_wrapper<
            std::pair<Entities, Sounds>>) = 0;
};
```

### 6.1 Rôle général

Un module graphique doit :

- récupérer les entrées utilisateur ;
- charger ou préparer les assets transmis par le jeu ;
- dessiner les entités et gérer les sons associés.

### 6.2 Méthodes

#### `getEvent()`
Retourne le prochain événement utilisateur sous forme de `Event`.

Responsabilités :

- lire le clavier ;
- lire la souris ;
- détecter la fermeture de fenêtre ;
- convertir les événements natifs de la lib graphique vers le type commun `arc::Event`.

#### `setAssets(Assets)`
Permet au jeu de fournir les assets nécessaires au module graphique.

#### `drawGame(std::reference_wrapper<std::pair<Entities, Sounds>>)`
Dessine la frame courante du jeu à partir :

- des entités à afficher ;
- des sons à traiter.

Implication pratique :

- le renderer reçoit l’état visuel courant produit par le jeu ;
- l’interface actuelle laisse possible une modification de ces structures par le module graphique.

---

## 7. Documentation de `IGameModule.hpp`

`IGameModule` définit le contrat que doit respecter toute bibliothèque de jeu.

### 7.1 Alias auxiliaires

```cpp
using SelectLibs = std::pair<std::optional<std::string>,
    std::optional<std::string>>;
using Libs = std::pair<std::vector<std::string>, std::vector<std::string>>;
```

#### `Libs`
Représente la liste des bibliothèques disponibles.

#### `SelectLibs`
Représente une demande de changement de bibliothèque.

Chaque `optional<string>` peut contenir :

- le nom ou chemin d’une nouvelle bibliothèque graphique ;
- le nom ou chemin d’un nouveau jeu ;
- ou être vide si aucun changement n’est demandé.

### 7.2 Interface `IGameModule`

```cpp
class IGameModule {
    public:
        virtual ~IGameModule() = default;

        virtual void simulate(Event) = 0;
        virtual Assets getAssets() = 0;
        virtual std::pair<Entities, Sounds> getElements() = 0;
        virtual bool isOpen() = 0;
        virtual SelectLibs libChanges(Libs) = 0;
};
```

### 7.3 Méthodes

#### `simulate(Event)`
Fait avancer la logique du jeu d’un tick à partir d’un événement utilisateur.

Responsabilités :

- interpréter l’entrée joueur ;
- mettre à jour la simulation ;
- générer un nouvel état de jeu.

#### `getAssets()`
Retourne la liste des assets nécessaires au jeu.

Cette méthode permet au core ou au module graphique de connaître les ressources attendues.

#### `getElements()`
Retourne les entités et les sons à produire pour l’état courant.

C’est la sortie principale du module de jeu vers le système de rendu.

#### `isOpen()`
Indique si le jeu est encore en cours d’exécution ou si le module souhaite continuer à fonctionner.

#### `libChanges(Libs)`
Permet au jeu de signaler un changement de bibliothèque souhaité.
