# MMORPG — Объектно-ориентированная игровая система (C++17)

Учебный проект по курсу **Объектно-ориентированного программирования (ООП)** на языке **C++17** с использованием системы сборки **CMake**.  
Проект представляет собой пошаговую тактическую MMORPG-систему с игровым полем, иерархией персонажей, игровой базой, ландшафтом с модификаторами характеристик, нейтральными объектами и единым игровым контроллером[cite: 31, 35, 36].

---

## Архитектура и реализованные паттерны проектирования

Проект построен по модульному принципу с разделением на заголовочные файлы (`include/`) и файлы реализации (`src/`)[cite: 35]:

### 1. Порождающие паттерны
* **Фабричный метод (Factory Method)** (`UnitFactory`, `CarryFactory`, `MidderFactory`, `SupportFactory`):
  * Персонажи сгруппированы по игровым ролям (`Carry`, `Midder`, `Support`), каждая роль имеет собственную фабрику, создающую конкретных героев[cite: 31].
  * Соблюдается принцип **OCP (Open/Closed Principle)**: добавление новой роли и новых персонажей не требует изменения кода игрового поля или контроллера[cite: 31].
* **Прототип (Prototype)** (`Unit::clone()`):
  * Каждый конкретный юнит реализует виртуальный метод `clone()`, возвращающий `std::unique_ptr<Unit>`, что обеспечивает корректное глубокое копирование игрового поля `GameField`[cite: 31].

### 2. Структурные паттерны
* **Заместитель / Прокси (Proxy)** (`Terrain`, `TerrainProxy`, `TerrainMap`):
  * `TerrainProxy` оборачивает реальные клетки местности (`Plains`, `Forest`, `Swamp`, `Mountains`), контролирует доступ (блокирует вход на непроходимые горы `Mountains`), ведёт логирование перемещений и управляет применением/снятием эффектов ландшафта (`onEnter` / `onLeave`)[cite: 33].
* **Фасад / Контроллер (Facade / Controller)** (`GameController`):
  * Координирует работу игрового поля (`GameField`), карты ландшафта и объектов (`TerrainMap`), игровой базы (`Base`) и фабрик юнитов, предоставляя единый интерфейс для спавна, перемещения, боя и отрисовки мира.

### 3. Поведенческие паттерны
* **Наблюдатель (Observer)** (`IObserver`, `IObservable`, `UnitEvent`, `Base`):
  * Класс `Unit` наследует `IObservable` и рассылает события `UnitEvent` (`Created`, `Damaged`, `Destroyed`)[cite: 33, 35].
  * Игровая база `Base` реализует `IObserver`: отслеживает состояние созданных бойцов и при их гибели автоматически освобождает слот активного юнита[cite: 33, 35].
* **Стратегия (Strategy)** (`IInteractionStrategy`, `PowerBoostStrategy`, `TrapDebuffStrategy`, `FullHealStrategy`, `TeleportStrategy`):
  * Поведение нейтральных объектов (`NeutralObject`) инкапсулировано в сменные стратегии взаимодействия с юнитом, что позволяет гибко изменять логику объектов во время выполнения[cite: 36].

---

## Игровые сущности и механики

### Атрибуты (`include/Attributes/`)
* **`Health`** — текущее и максимальное здоровье с валидацией урона и лечения (`decrease`, `increase`, `isZero`)[cite: 31, 37].
* **`Armor`** — показатель брони, снижающий входящий урон (`reduceDamage`, `modifyValue`)[cite: 31, 33].
* **`Attack`** — показатель атаки и расчёт итогового урона по цели (`calculateDamage`, `modifyDamage`)[cite: 31, 33].

### Иерархия персонажей (`include/Units/`)
Трёхуровневая иерархия наследования (`Unit` → Роль → Конкретный герой)[cite: 31]:
* **Carry**: `PhantomAssassin` (`P`), `Lifestealer` (`L`)[cite: 31]
* **Midder**: `Invoker` (`I`), `ArcWarden` (`A`)[cite: 31]
* **Support**: `Dazzle` (`D`), `WitchDoctor` (`W`)[cite: 31]

### Ландшафт (`include/Terrain/`)
| Тип местности | Символ | Проходимость | Эффекты для ролей |
| :--- | :---: | :---: | :--- |
| **Равнина (`Plains`)** | `.` | Да | `Carry`: **+10** к атаке[cite: 33] |
| **Лес (`Forest`)** | `T` | Да | `Midder`: **+10** к атаке; `Support`: **-5** к броне[cite: 33] |
| **Болото (`Swamp`)** | `~` | Да | `Carry` и `Midder`: **-5** к броне; `Support`: **+8** к броне[cite: 33] |
| **Горы (`Mountains`)** | `^` | **Нет** | Непроходимы для всех юнитов[cite: 33] |

### Нейтральные объекты и перегрузка операторов (`include/NeutralObjects/`)
* **`PowerRune`** (`*`) — одноразовая руна силы (**+15%** к атаке)[cite: 36].
* **`Trap`** (`x`) — одноразовая ловушка (**-5%** к броне и **-5%** к атаке)[cite: 36].
* **`HealingFountain`** (`+`) — источник лечения (восстанавливает **100% HP**)[cite: 36].
* **`Teleport`** (`@`) — зеркально телепортирует юнита относительно центра карты с проверкой проходимости и занятости целевой клетки[cite: 36].
* **Перегруженные операторы**:
  * `unit + neutralObj` — активирует взаимодействие юнита с нейтральным объектом[cite: 36].
  * `!neutralObj` — проверяет, деактивирован (израсходован) ли объект[cite: 36].

### Контейнер поля и управление памятью (`include/Field/`)
* **`GameField`** — управляет юнитами через `std::unique_ptr<Unit>`, реализует **Rule of Five** (глубокое копирование через идиому *Copy-and-Swap* с `clone()` и `noexcept` семантику перемещения)[cite: 31, 34].
* **`GameFieldIterator`** — собственный STL-совместимый итератор с автоматическим пропуском пустых ячеек (`skipEmpty()`)[cite: 31].

---

## Структура проекта

```text
mmo-rpg/
├── CMakeLists.txt
├── README.md
├── architecture.md
├── include/
│   ├── Attributes/        # Health, Armor, Attack
│   ├── Base/              # Base (игровая база и наблюдатель)
│   ├── Controller/        # GameController (координатор подсистем)
│   ├── Factories/         # UnitFactory, CarryFactory, MidderFactory, SupportFactory
│   ├── Field/             # GameField, GameFieldIterator
│   ├── NeutralObjects/    # NeutralObject, PowerRune, Trap, HealingFountain, Teleport
│   ├── Observer/          # IObserver, IObservable, UnitEvent
│   ├── Strategies/        # IInteractionStrategy и 4 конкретные стратегии
│   ├── Terrain/           # Terrain, Plains, Forest, Swamp, Mountains, TerrainProxy, TerrainMap
│   └── Units/             # Unit, Carry, Midder, Support и 6 классов персонажей
└── src/
    ├── Attributes/
    ├── Base/
    ├── Controller/
    ├── Factories/
    ├── Field/
    ├── NeutralObjects/
    ├── Observer/
    ├── Strategies/
    ├── Terrain/
    ├── Units/
    └── main.cpp           # Демонстрация всех сценариев работы системы