# Space Shot Symphony Tech Doc

## Architecture

### Tech Specs

-   Programming Language: C++
-   Game Engine: Unreal Engine 5
-   Framework: Gameplay Ability System, Gameplay Tags, Enhanced Input System

### Class Diagram
![Class Diagram](https://github.com/FutureWayne/MirrorForce/assets/39150337/3bbbbca3-e790-43a7-bbe6-cadb19b8c64f)

## Gameplay Ability System

The Gameplay Ability System is a framework for building attributes, abilities, and interactions that an Actor can own and trigger. Here is a breakdown how “Space Shot Symphony” utilizes this framework.

### Integrating Gameplay Ability System (GAS)

In GAS, the Ability System Component (ASC) is the component that manages the abilities and their interactions, while the Attribute Set defines and manages the character's stats and attributes that these abilities use or affect. In this game, The ASC and Attribute Set are added to the Player State rather than Player class, aligning the abilities and attributes more closely with the player's persistent data rather than their physical representation. This separation can lead to cleaner, more modular code.

### Binding input to an Ability

In “Space Shot Symphony”, the process of linking Input Actions to abilities is streamlined and efficient. Input actions are defined in the Unreal Engine settings and then bound to handling functions in the player controller. Each ability is associated with a unique Gameplay Tag, serving as an identifier. When an input action occurs, the Ability System Component (ASC) attached to the player character receives the corresponding tag and activates the matched ability.

This approach is highly scalable and flexible, allowing for easy addition of new abilities and input rebinding. It decouples input handling from ability logic, simplifying modifications and supporting complex input scenarios without overcomplicating the system. This method is ideal for dynamic gameplay and future game development scalability.


Here is a breakdown of how this linking process is done:

#### Define Input Actions

- **Where**: Defined in the Unreal Engine Editor under Project Settings.
- **Purpose**: Map input events (e.g., keyboard, mouse) to named input actions.

#### Define Ability Tags

- **File**: `MirrorForceGameplayAbility.cpp`
- **Function**: `AMirrorForceGameplayAbility::AddCharacterAbilities`
- **Purpose**: Grant each ability with a startup tag as an identifier.
- **Process**: Calls `AbilitySpec.DynamicAbilityTags.AddTag` to add a startup tag to the given ability.

#### Setup Input Component in Player Controller

- **File**: `MirrorForcePlayerController.cpp`
- **Function**: `AMirrorForcePlayerController::SetupInputComponent`
- **Purpose**: Sets up the input bindings for the player controller.
- **Process**:
  - Calls `Super::SetupInputComponent()` to inherit base class setup.
  - Binds specific actions to functions.
  - Uses `BindAbilityActions` to bind ability-related input actions.

#### Bind Ability Actions

- **File**: `MirrorForcePlayerController.cpp`
- **Function**: `BindAbilityActions`
- **Purpose**: Binds ability input actions to specific functions for handling ability activation.
- **Process**: Takes `InputConfig` and binds ability actions to `AbilityInputTagPressed`, `AbilityInputTagReleased`, and `AbilityInputTagHeld`.

#### Handle Ability Input Tags

- **File**: `MirrorForcePlayerController.cpp`
- **Functions**:
  - `AbilityInputTagPressed(FGameplayTag InputTag)`
  - `AbilityInputTagReleased(FGameplayTag InputTag)`
  - `AbilityInputTagHeld(FGameplayTag InputTag)`
- **Purpose**: These functions are called when an ability-related input is detected (pressed, released, or held).
- **Process**: They retrieve the `MirrorAbilitySystemComponent` and call corresponding methods on it with the gameplay tag.

#### Gameplay Ability Component Handling

- **File**: `MirrorAbilitySystemComponent.cpp`
- **Functions**:
  - `AbilityInputTagPressed(FGameplayTag InputTag)`
  - `AbilityInputTagReleased(FGameplayTag InputTag)`
  - `AbilityInputTagHeld(FGameplayTag InputTag)`
- **Purpose**: These methods check the registered abilities for a matching tag. If a match is found, the ASC attempts to activate the corresponding ability.
- **Process**: When an input action occurs (press, release, hold), the corresponding method (`AbilityInputTagPressed`, `AbilityInputTagReleased`, `AbilityInputTagHeld`) is called on the ASC with the gameplay tag.

### In-game Implementation

#### Gameplay Abilities

- **Description**: There are three abilities implemented in-game: Shield, Flash, and Projectile Firing. Each subclasses from `UMirrorForceGameplayAbility` and has their tasks written in the override function `ActivateAbility`.

#### Attributes

- **Class**: `UMirrorAttributeSet`
- **Description**: In the `UMirrorAttributeSet` class, the game defines four attributes:
  - **Health**: Indicates how many bullet hits the player can take before dying.
  - **MaxHealth**: The maximum health of the player.
  - **Mana**: Determines the energy needed to activate the shield.
  - **MaxMana**: The maximum mana of the player.

#### Gameplay Effects

- **Description**: In-game, gameplay effects are used for three main purposes:
  1. **Handling Damage**:
     - **Effect**: Defines a gameplay effect with an instant modifier to subtract the health attribute when the player gets hit.
  2. **Handling Ability Cost**:
     - **Effect**: Defines a gameplay effect with a duration modifier to drain mana attribute when the shield is activated.
  3. **Handling Ability Cooldown**:
     - **Effect**: Defines a gameplay effect with a duration modifier that grants the player a cooldown tag when in effect. The ability cannot be used again until this effect expires and removes the cooldown tag.

## Model-View-Controller (MVC) Pattern

The Model-View-Controller (MVC) pattern is a widely used software architectural pattern for implementing user interfaces. It offers significant benefits by establishing clear, one-way relationships between the components. This directed association enhances the overall structure and maintainability of the system. Here is a breakdown of how MVC is applied in the “Space Shot Symphony" project:

### Model

- **Components**: `AttributeSet` and `AbilitySystemComponent`.
- **Role**: Represents the core data and business logic of the game. `AttributeSet` holds gameplay-related data like health, mana, etc., while `AbilitySystemComponent` manages the abilities and their effects on these attributes.
- **Functionality**: Encapsulates the game's state and logic, such as calculating health after taking damage or managing ability cooldowns.

### View

- **Components**: UMG widgets that subclass from `MirrorForceUserWidget`.
- **Role**: Responsible for presenting the data to the user. These widgets form the user interface that players interact with.
- **Functionality**: Widgets register callbacks for events broadcasted by the Controller in `WigetBlueprint`, such as `OnHealthChange`. When these events are triggered, the widgets update themselves to reflect the new state of the Model (e.g., updating a health bar when the player's health changes).

### Controller

- **Component**: `MirrorForceWidgetController`.
- **Role**: Acts as an intermediary between the Model and the View. It observes changes in the Model and updates the View accordingly.
- **Functionality**: Observes the Model by registering events with `AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate`. This allows the controller to listen for changes in game attributes. Broadcasts changes to the View using functions like `OnHealthChange.Broadcast`. These broadcasts notify the View of changes in the Model.

## Object Pooling

“Space Shot Symphony” utilizes object pooling to reuse game objects efficiently.

### `MirrorForceActorPool.cpp`

- **Functionality**: Manages a pool of actors.
- **Key Methods**:
  - **Constructor**: Initializes the actor pool component.
  - **BeginPlay**: Creates a specified number of pooled actors (`PoolSize`) and adds them to the pool. Each actor is initially set to inactive.
  - **SpawnPooledActor**: Searches for an inactive actor in the pool and activates it for use. If all actors are active, it reuses an actor by deactivating and reactivating it.
  - **OnPooledActorDespawn**: Called when a pooled actor is despawned, marking it as inactive and available for reuse.

### `MirrorForcePooledActor.cpp`

- **Functionality**: Represents an individual actor within the pool.
- **Key Methods**:
  - **SetActive**: Activates or deactivates the actor. When deactivated, the actor is hidden and a timer is set for automatic deactivation.
  - **Deactivate**: Deactivates the actor and notifies the pool that the actor is available for reuse.
  - **SetLifeSpan**: Sets the lifespan of the actor, after which it will automatically deactivate.
  - **IsActive**: Returns whether the actor is currently active.

### `MirrorForceBulletSpawner.cpp`

- **Functionality**: Responsible for spawning bullets using the actor pool.
- **Key Methods**:
  - **SpawnCirclePattern, SpawnMultipleCircles, SpawnSpiralPattern**, etc.: These methods demonstrate various patterns for spawning bullets. They use the `BulletPool` (an instance of `MirrorForceActorPool`) to obtain bullets and set their properties (like location and velocity) before activating them.


