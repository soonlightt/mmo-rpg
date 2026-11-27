# Архитектура проекта MMORPG

Данный документ описывает архитектуру пошаговой тактической MMORPG-системы, применённые порождающие, структурные и поведенческие паттерны проектирования, а также взаимодействие подсистем.

---

## 1. Общая диаграмма классов (Mermaid UML)

```mermaid
classDiagram
    %% Атрибуты
    class Health {
        -int value
        -int maxValue
        +decrease(int amount)
        +increase(int amount)
        +isZero() bool
    }
    class Armor {
        -int value
        +reduceDamage(int damage) int
    }
    class Attack {
        -int damage
        +calculateDamage(const Armor& targetArmor) int
    }

    %% Юниты и Наблюдатель
    class IObservable {
        +addObserver(IObserver* obs)
        +removeObserver(IObserver* obs)
        +notify(const UnitEvent& event)
    }
    class IObserver {
        <<interface>>
        +onUnitEvent(const UnitEvent& event)*
    }
    class Unit {
        <<abstract>>
        #int id
        #int x
        #int y
        #Health health
        #Armor armor
        #Attack attack
        +takeDamage(int dmg)
        +heal(int amount)
        +getType()* string
        +getSymbol()* char
        +clone()* unique_ptr~Unit~
    }

    IObservable <|-- Unit
    Unit *-- Health
    Unit *-- Armor
    Unit *-- Attack

    class Carry { <<abstract>> }
    class Midder { <<abstract>> }
    class Support { <<abstract>> }

    Unit <|-- Carry
    Unit <|-- Midder
    Unit <|-- Support

    Carry <|-- PhantomAssassin
    Carry <|-- Lifestealer
    Midder <|-- Invoker
    Midder <|-- ArcWarden
    Support <|-- Dazzle
    Support <|-- WitchDoctor

    %% Фабрики (Factory Method)
    class UnitFactory {
        <<interface>>
        +createUnit(int subType, int x, int y)* unique_ptr~Unit~
    }
    UnitFactory <|-- CarryFactory
    UnitFactory <|-- MidderFactory
    UnitFactory <|-- SupportFactory

    %% База и Постройки
    class Base {
        -int health
        -int resources
        -size_t maxUnits
        -vector~shared_ptr~Building~~ buildings
        -set~string~ researchedUpgrades
        +createUnit(...)
        +buildBuilding(...)
        +research(...)
        +distributeResources(...)
        +onUnitEvent(const UnitEvent& event)
    }
    IObserver <|.. Base
    Base *-- Building

    %% Поле и Итератор
    class GameField {
        -int width
        -int height
        -vector~unique_ptr~Unit~~ units
        +addUnit(unique_ptr~Unit~ unit) bool
        +removeUnit(int x, int y) bool
        +begin() GameFieldIterator
        +end() GameFieldIterator
    }
    GameField o-- Unit
    GameField ..> GameFieldIterator : creates

    %% Ландшафт (Proxy) и Нейтральные объекты (Strategy)
    class Terrain {
        <<interface>>
        +isPassable()* bool
        +onEnter(Unit& unit)*
        +onLeave(Unit& unit)*
    }
    Terrain <|-- Plains
    Terrain <|-- Forest
    Terrain <|-- Swamp
    Terrain <|-- Mountains
    Terrain <|-- TerrainProxy
    TerrainProxy o-- Terrain : wraps

    class IInteractionStrategy {
        <<interface>>
        +apply(Unit& unit)*
    }
    IInteractionStrategy <|-- PowerBoostStrategy
    IInteractionStrategy <|-- TrapDebuffStrategy
    IInteractionStrategy <|-- FullHealStrategy
    IInteractionStrategy <|-- TeleportStrategy

    class NeutralObject {
        <<abstract>>
        -unique_ptr~IInteractionStrategy~ strategy
        +interact(Unit& unit)
    }
    NeutralObject *-- IInteractionStrategy
    NeutralObject <|-- PowerRune
    NeutralObject <|-- Trap
    NeutralObject <|-- HealingFountain
    NeutralObject <|-- Teleport

    class TerrainMap {
        -vector~vector~shared_ptr~TerrainProxy~~~ grid
        -vector~unique_ptr~NeutralObject~~ neutralObjects
    }
    TerrainMap *-- TerrainProxy
    TerrainMap *-- NeutralObject

    %% Посредник (Mediator)
    class GameMediator {
        -shared_ptr~Base~ base
        -GameField field
        -TerrainMap terrainMap
        +createUnit(...) bool
        +moveUnit(...) bool
        +attackUnit(...) bool
        +collectResource(...) bool
        +healUnit(...) bool
        +buildBuilding(...) bool
        +research(...) bool
        +distributeResources(...) bool
        +produceUnit(...) bool
    }
    GameMediator *-- Base
    GameMediator *-- GameField
    GameMediator *-- TerrainMap
    GameMediator ..> UnitFactory : uses

    %% Команды (Command)
    class ICommand {
        <<interface>>
        +execute()* bool
        +undo()* bool
        +getName()* string
    }
    ICommand <|-- CreateUnitCommand
    ICommand <|-- MoveUnitCommand
    ICommand <|-- AttackUnitCommand
    ICommand <|-- CollectResourceCommand
    ICommand <|-- HealCommand
    ICommand <|-- BuildCommand
    ICommand <|-- ResearchCommand
    ICommand <|-- DistributeResourcesCommand
    ICommand <|-- ProduceUnitCommand
    ICommand <|-- StatsCommand
    ICommand ..> GameMediator : delegates

    %% Цепочка обязанностей (Chain of Responsibility) и Фасад (Facade)
    class Handler {
        #unique_ptr~Handler~ nextHandler
        +setNext(unique_ptr~Handler~ next) Handler*
        +handle(string input, GameFacade& facade) bool
    }
    Handler o-- Handler : nextHandler
    Handler <|-- CreateHandler
    Handler <|-- MoveHandler
    Handler <|-- AttackHandler
    Handler <|-- CollectHandler
    Handler <|-- HealHandler
    Handler <|-- BuildHandler
    Handler <|-- ResearchHandler
    Handler <|-- ProduceHandler
    Handler <|-- StatsHandler
    Handler <|-- HelpHandler

    class GameFacade {
        -GameMediator mediator
        -vector~unique_ptr~ICommand~~ history
        -unique_ptr~Handler~ handlerChain
        +executeCommand(string input) bool
        +undoLastCommand() bool
    }
    GameFacade *-- GameMediator
    GameFacade *-- ICommand : history
    GameFacade *-- Handler : handlerChain
    Handler ..> GameFacade : invokes