# MMORPG — Объектно-ориентированная игровая система (C++17)

Учебный проект по курсу **Объектно-ориентированного программирования (ООП)** на языке **C++17** с использованием системы сборки **CMake**.  
Проект представляет собой пошаговую тактическую MMORPG-систему с игровым полем, иерархией персонажей, игровой базой, ландшафтом с модификаторами характеристик, нейтральными объектами, системой команд с поддержкой отмены (`undo`), посредником взаимодействий и цепочкой обработчиков консольного ввода[cite: 19].

---

## Архитектура и реализованные паттерны проектирования

Проект построен по модульному принципу с разделением на заголовочные файлы (`include/`) и файлы реализации (`src/`)[cite: 19]:

### 1. Порождающие паттерны
* **Фабричный метод (Factory Method)** (`UnitFactory`, `CarryFactory`, `MidderFactory`, `SupportFactory`):
  * Персонажи сгруппированы по игровым ролям (`Carry`, `Midder`, `Support`), каждая роль имеет собственную фабрику, создающую конкретных героев[cite: 19].
  * Соблюдается принцип **OCP (Open/Closed Principle)**: добавление новой роли и новых персонажей не требует изменения кода игрового поля или посредника[cite: 19].
* **Прототип (Prototype)** (`Unit::clone()`):
  * Каждый конкретный юнит реализует виртуальный метод `clone()`, возвращающий `std::unique_ptr<Unit>`, что обеспечивает корректное глубокое копирование игрового поля `GameField`[cite: 19].

### 2. Структурные паттерны
* **Заместитель / Прокси (Proxy)** (`Terrain`, `TerrainProxy`, `TerrainMap`):
  * `TerrainProxy` оборачивает реальные клетки местности (`Plains`, `Forest`, `Swamp`, `Mountains`), контролирует доступ (блокирует вход на непроходимые горы `Mountains`), ведёт логирование перемещений и управляет применением/снятием эффектов ландшафта (`onEnter` / `onLeave`)[cite: 19].
* **Фасад (Facade)** (`GameFacade`):
  * Предоставляет единый высокоуровневый интерфейс для управления игрой (`createUnit`, `moveUnit`, `attackUnit`, `buildBuilding`, `research`, `undoLastCommand` и др.), скрывая сложную координацию команд, истории выполнения и игрового посредника[cite: 6].

### 3. Поведенческие паттерны
* **Наблюдатель (Observer)** (`IObserver`, `IObservable`, `UnitEvent`, `Base`):
  * Класс `Unit` наследует `IObservable` и рассылает события `UnitEvent` (`Created`, `Damaged`, `Destroyed`)[cite: 19].
  * Игровая база `Base` реализует `IObserver`: отслеживает состояние созданных бойцов и при их гибели автоматически освобождает слот активного юнита[cite: 19].
* **Стратегия (Strategy)** (`IInteractionStrategy`, `PowerBoostStrategy`, `TrapDebuffStrategy`, `FullHealStrategy`, `TeleportStrategy`):
  * Поведение нейтральных объектов (`NeutralObject`) инкапсулировано в сменные стратегии взаимодействия с юнитом, что позволяет гибко изменять логику объектов во время выполнения[cite: 19].
* **Посредник (Mediator)** (`GameMediator`):
  * Централизует взаимодействие между игровым полем (`GameField`), картой ландшафта (`TerrainMap`), игровой базой (`Base`), фабриками юнитов и нейтральными объектами, устраняя прямые циклические зависимости между подсистемами[cite: 16].
* **Команда (Command)** (`ICommand` и 10 конкретных команд):
  * Инкапсулирует каждое игровое действие (`CreateUnitCommand`, `MoveUnitCommand`, `AttackUnitCommand`, `CollectResourceCommand`, `HealCommand`, `BuildCommand`, `ResearchCommand`, `DistributeResourcesCommand`, `ProduceUnitCommand`, `StatsCommand`) в отдельный объект с методами `execute()`, `undo()` и `getName()`, что позволяет вести журнал действий и выполнять откат (`undo`)[cite: 6, 15].
* **Цепочка обязанностей (Chain of Responsibility)** (`Handler` и 10 обработчиков в `Handlers/`):
  * Разгружает `GameFacade` от разбора текстового ввода: запрос последовательно проходит по цепочке (`CreateHandler` → `MoveHandler` → `AttackHandler` → `CollectHandler` → `HealHandler` → `BuildHandler` → `ResearchHandler` → `ProduceHandler` → `StatsHandler` → `HelpHandler`), пока профильный обработчик не распознает команду и не передаст её в фасад.

---

## Игровые сущности и механики

### Атрибуты (`include/Attributes/`)
* **`Health`** — текущее и максимальное здоровье с валидацией урона и лечения (`decrease`, `increase`, `isZero`)[cite: 19].
* **`Armor`** — показатель брони, снижающий входящий урон (`reduceDamage`, `modifyValue`)[cite: 19].
* **`Attack`** — показатель атаки и расчёт итогового урона по цели (`calculateDamage`, `modifyDamage`)[cite: 19].

### Иерархия персонажей (`include/Units/`)
Трёхуровневая иерархия наследования (`Unit` → Роль → Конкретный герой)[cite: 19]:
* **Carry**: `PhantomAssassin` (`P`), `Lifestealer` (`L`)[cite: 19]
* **Midder**: `Invoker` (`I`), `ArcWarden` (`A`)[cite: 19]
* **Support**: `Dazzle` (`D`), `WitchDoctor` (`W`)[cite: 19]

### Ландшафт (`include/Terrain/`)
| Тип местности | Символ | Проходимость | Эффекты для ролей |
| :--- | :---: | :---: | :--- |
| **Равнина (`Plains`)** | `.` | Да | `Carry`: **+10** к атаке[cite: 19] |
| **Лес (`Forest`)** | `T` | Да | `Midder`: **+10** к атаке; `Support`: **-5** к броне[cite: 19] |
| **Болото (`Swamp`)** | `~` | Да | `Carry` и `Midder`: **-5** к броне; `Support`: **+8** к броне[cite: 19] |
| **Горы (`Mountains`)** | `^` | **Нет** | Непроходимы для всех юнитов[cite: 19] |

### Нейтральные объекты и перегрузка операторов (`include/NeutralObjects/`)
* **`PowerRune`** (`*`) — одноразовая руна силы (**+15%** к атаке)[cite: 19].
* **`Trap`** (`x`) — одноразовая ловушка (**-5%** к броне и **-5%** к атаке)[cite: 19].
* **`HealingFountain`** (`+`) — источник лечения (восстанавливает **100% HP**)[cite: 19].
* **`Teleport`** (`@`) — зеркально телепортирует юнита относительно центра карты с проверкой проходимости и занятости целевой клетки[cite: 19].
* **Перегруженные операторы**:
  * `unit + neutralObj` — активирует взаимодействие юнита с нейтральным объектом[cite: 19].
  * `!neutralObj` — проверяет, деактивирован (израсходован) ли объект[cite: 19].

### Контейнер поля и управление памятью (`include/Field/`)
* **`GameField`** — управляет юнитами через `std::unique_ptr<Unit>`, реализует **Rule of Five** (глубокое копирование через идиому *Copy-and-Swap* с `clone()` и `noexcept` семантику перемещения)[cite: 19].
* **`GameFieldIterator`** — собственный STL-совместимый итератор с автоматическим пропуском пустых ячеек (`skipEmpty()`)[cite: 19].

---

## Консольные команды (Interactive Mode)

После демонстрационного сценария программа переходит в интерактивный режим, поддерживающий следующие команды:

* `create <unitType> at <x>,<y>` — нанять юнита (`PhantomAssassin`, `Lifestealer`, `Invoker`, `ArcWarden`, `Dazzle`, `WitchDoctor`)[cite: 2, 16]
* `move unit <id> to <x>,<y>` — переместить юнита по ID[cite: 2]
* `attack unit <attackerId> -> <targetId>` — атаковать вражеского юнита[cite: 2]
* `collect resource <resId> by unit <unitId>` — собрать ресурс в пользу базы[cite: 2]
* `heal unit <targetId> by <healerId>` — вылечить союзного юнита[cite: 2]
* `build <buildingType> at <x>,<y>` — построить здание (`barracks`, `tower` и др.)[cite: 2]
* `research <techId>` — изучить улучшение на базе (например, `attack_upgrade`)[cite: 1, 2]
* `distribute <amount> <resType> to <targetBuilding>` — распределить ресурсы со склада базы[cite: 2]
* `produce <unitType> in <buildingId>` — произвести юнита в здании[cite: 2]
* `stats [units|buildings|resources]` — вывести статистику мира[cite: 2]
* `undo` — отменить последнюю обратимую команду[cite: 2, 5]
* `history` — показать журнал выполненных команд[cite: 2, 5]
* `map` — отрисовать карту мира `15x15`[cite: 1, 2]
* `help` / `exit` — справка по командам и выход из программы[cite: 2]

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
│   ├── Buildings/         # Building (игровые постройки)
│   ├── Commands/          # ICommand и 10 конкретных классов команд
│   ├── Controller/        # GameController
│   ├── Facade/            # GameFacade (единая точка входа и история команд)
│   ├── Factories/         # UnitFactory, CarryFactory, MidderFactory, SupportFactory
│   ├── Field/             # GameField, GameFieldIterator
│   ├── Handlers/          # Handler и 10 обработчиков цепочки обязанностей
│   ├── Mediator/          # GameMediator (координатор подсистем)
│   ├── NeutralObjects/    # NeutralObject, PowerRune, Trap, HealingFountain, Teleport
│   ├── Observer/          # IObserver, IObservable, UnitEvent
│   ├── Strategies/        # IInteractionStrategy и 4 конкретные стратегии
│   ├── Terrain/           # Terrain, Plains, Forest, Swamp, Mountains, TerrainProxy, TerrainMap
│   └── Units/             # Unit, Carry, Midder, Support и 6 классов персонажей
└── src/
    ├── Attributes/
    ├── Base/
    ├── Buildings/
    ├── Commands/
    ├── Controller/
    ├── Facade/
    ├── Factories/
    ├── Field/
    ├── Handlers/
    ├── Mediator/
    ├── NeutralObjects/
    ├── Observer/
    ├── Strategies/
    ├── Terrain/
    ├── Units/
    └── main.cpp           # Демонстрация всех команд, undo и интерактивный режим