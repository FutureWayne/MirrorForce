# Space Shot Symphony Tech Doc

## Architecture

### Tech Specs
- **Programming Language:** C++
- **Game Engine:** Unreal Engine 5
- **Framework:** Gameplay Ability System, Gameplay Tags, Enhanced Input System

## Class Diagram
![Class Diagram](https://github.com/FutureWayne/MirrorForce/assets/39150337/a9775347-18ca-411d-95d7-071a57f9bd6c)

## Gameplay Ability System 

The Gameplay Ability System is a framework designed for building attributes, abilities, and interactions for an Actor. Below is an overview of how “Space Shot Symphony” utilizes this system.

### Integrating Gameplay Ability System (GAS)
- **Ability System Component (ASC):** Manages abilities and their interactions.
- **Attribute Set:** Defines and manages character stats and attributes.
- **Placement:** ASC and Attribute Set are added to the Player State for closer alignment with player's persistent data.

### Binding Input to an Ability
- **Process:** Linking Input Actions to abilities using Unreal Engine settings and player controller functions.
- **Advantages:** Scalable, flexible, and decouples input handling from ability logic.

#### Process Breakdown
1. **Define Input Actions**
   - **Location:** Unreal Engine Editor under Project Settings.
   - **Purpose:** Map input events to named input actions.
2. **Define Ability Tags**
   - **File:** `MirrorForceGameplayAbility.cpp`
   - **Purpose:** Grant each ability a startup tag as an identifier.
3. **Setup Input Component in Player Controller**
   - **File:** `MirrorForcePlayerController.cpp`
   - **Purpose:** Set up input bindings.
4. **Bind Ability Actions**
   - **File:** `MirrorForcePlayerController.cpp`
   - **Purpose:** Bind ability input actions for activation.
5. **Handle Ability Input Tags**
   - **File:** `MirrorForcePlayerController.cpp`
   - **Purpose:** Detect and process ability-related input.
6. **Gameplay Ability Component Handling**
   - **File:** `MirrorAbilitySystemComponent.cpp`
   - **Purpose:** Check and activate corresponding abilities based on input tags.

### In-game Implementation
- **Gameplay Abilities:** Three abilities implemented - Shield, Flash, and Projectile Firing.
- **Attributes:** Health, MaxHealth, Mana, MaxMana.
- **Gameplay Effects:** Used for handling damage, ability cost, and cooldowns.

## Model-View-Controller (MVC) Pattern
Applied in “Space Shot Symphony” to enhance structure and maintainability.

### Model
- **Components:** AttributeSet and AbilitySystemComponent.
- **Role:** Represent core data and game logic.

### View
- **Components:** UMG widgets subclassed from MirrorForceUserWidget.
- **Role:** Present data to the user.

### Controller
- **Component:** MirrorForceWidgetController.
- **Role:** Intermediary between Model and View.

## Object Pooling

In "Space Shot Symphony", object pooling is utilized to efficiently reuse game objects.

### MirrorForceActorPool.cpp
**Functionality:** Manages a pool of actors.

#### Key Methods
- **Constructor:** Initializes the actor pool component.
- **BeginPlay:** 
  - Creates a specified number of pooled actors (`PoolSize`) and adds them to the pool.
  - Each actor is initially set to inactive.
- **SpawnPooledActor:** 
  - Searches for an inactive actor in the pool.
  - Activates it for use or reuses an actor by deactivating and reactivating it if all are active.
- **OnPooledActorDespawn:** 
  - Called when a pooled actor is despawned.
  - Marks it as inactive and available for reuse.

### MirrorForcePooledActor.cpp
**Functionality:** Represents an individual actor within the pool.

#### Key Methods
- **SetActive:** 
  - Activates or deactivates the actor.
  - When deactivated, the actor is hidden and set with a timer for automatic deactivation.
- **Deactivate:** 
  - Deactivates the actor.
  - Notifies the pool that the actor is available for reuse.
- **SetLifeSpan:** 
  - Sets the lifespan of the actor, after which it will automatically deactivate.
- **IsActive:** 
  - Returns whether the actor is currently active.

### MirrorForceBulletSpawner.cpp
**Functionality:** Responsible for spawning bullets using the actor pool.

#### Key Methods
- **SpawnCirclePattern, SpawnMultipleCircles, SpawnSpiralPattern, etc.:**
  - Demonstrates various patterns for spawning bullets.
  - Uses the BulletPool (instance of MirrorForceActorPool) to obtain bullets.
  - Sets properties like location and velocity before activating them.

