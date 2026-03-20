# MobileShooter 🔫

> **Unreal Engine 5.7** · Mobile-first · Real-time Multiplayer

Step into the battlefield and prove your skills in this fast-paced mobile shooter!

Lock and load as you dive into intense action where every second counts. Fight against players from around the world or take on challenging missions to sharpen your aim and strategy. With smooth controls, realistic weapons, and dynamic maps, every match feels unique and adrenaline-filled.

Customize your loadout, unlock powerful gear, and upgrade your arsenal to dominate the competition. Whether you prefer close-range combat or long-distance sniping, there's a playstyle for you.

---

## 🔥 Features

| Feature | Details |
|---|---|
| **Real-time multiplayer** | Authoritative server, full UE5 replication, up to 10 players per match |
| **Weapon variety** | Assault Rifle · Sniper Rifle · Shotgun · SMG · Pistol · Rocket Launcher |
| **Weapon upgrades** | Damage, range, fire-rate, and cosmetic upgrades via progression system |
| **Dual weapon slots** | Primary + Secondary weapon slots with instant hot-swap |
| **Multiple game modes** | Team Deathmatch · Free-for-All · Capture The Flag · Domination · Battle Royale |
| **Dynamic maps** | Desert · Urban · Jungle (placeholder maps; swap your own UE5 levels) |
| **Mobile controls** | Virtual joysticks, touch-to-fire button layout, tilt-to-look support |
| **ADS (Aim Down Sights)** | Smooth FOV-interpolated zoom with reduced movement speed while aiming |
| **Crouch** | Full crouch support with reduced speed and collision height |
| **Graphics** | Nanite, Lumen, TSR (Temporal Super Resolution), mobile scalability settings |
| **Sound** | Immersive weapon SFX, positional audio, dynamic music via Audio Modulation |
| **Progression** | XP / levelling system, loadout saves, unlockable weapons, persistent save data |
| **Kill streaks** | Unbroken kill streaks rewarded with score bonuses (×3, ×5, ×7, ×10+) |
| **Assists** | Damage contributors who don't get the kill receive an assist and bonus score |
| **Shield regeneration** | Shields auto-regenerate after a delay (configurable rate, separate from health) |
| **AI Bots** | Behavior-Tree driven bot opponents fill lobbies when human players are short |

---

## 🗂️ Project Structure

```
MobileShooter/
├── MobileShooter.uproject          # UE5.7 project descriptor
├── Config/
│   ├── DefaultEngine.ini           # Renderer, network, mobile settings
│   ├── DefaultGame.ini             # Game maps, modes, packaging settings
│   ├── DefaultInput.ini            # Key/touch bindings
│   └── DefaultGameplayTags.ini     # Gameplay tag definitions
└── Source/
    └── MobileShooter/
        ├── MobileShooter.Build.cs  # Module dependencies
        ├── Types/
        │   └── MSTypes.h           # Shared enums & structs
        ├── Character/
        │   ├── MobileShooterCharacter.h/.cpp   # Player pawn (GAS, health, weapons)
        ├── Weapon/
        │   ├── WeaponBase.h/.cpp               # Abstract weapon (fire, reload, replication)
        │   ├── AssaultRifle.h/.cpp             # Full-auto AR
        │   ├── SniperRifle.h/.cpp              # Bolt-action sniper w/ scope
        │   ├── Shotgun.h/.cpp                  # Multi-pellet shotgun
        │   ├── SMG.h/.cpp                      # Fast-fire submachine gun
        │   └── Pistol.h/.cpp                   # Semi-auto sidearm
        ├── GameMode/
        │   ├── MobileShooterGameMode.h/.cpp    # Base game mode (lifecycle, respawn)
        │   ├── TeamDeathmatchGameMode.h/.cpp   # TDM variant
        │   └── DeathmatchGameMode.h/.cpp       # FFA variant
        ├── PlayerState/
        │   └── MobileShooterPlayerState.h/.cpp # Kills/Deaths/Assists/Team (replicated)
        ├── PlayerController/
        │   └── MobileShooterPlayerController.h/.cpp  # Mobile input, HUD management
        ├── UI/
        │   └── MobileShooterHUD.h/.cpp         # Widget management (HUD, respawn, scoreboard)
        ├── GameInstance/
        │   └── MobileShooterGameInstance.h/.cpp # Persistence, XP, loadout saving
        ├── Components/
        │   └── MSHealthComponent.h/.cpp         # Health + shield + regen
        └── AI/
            └── MSBotController.h/.cpp           # BT-based bot AI
```

---

## 🚀 Getting Started

### Prerequisites

- **Unreal Engine 5.7** installed via the Epic Games Launcher
- Visual Studio 2022 (Windows) or Xcode 15+ (macOS)
- Android SDK / iOS SDK for mobile builds

### Build & Run

1. Right-click **MobileShooter.uproject** → *Generate Visual Studio project files*
2. Open `MobileShooter.sln` in Visual Studio 2022
3. Set solution configuration to **Development Editor** and platform to **Win64**
4. Build and run — the editor will open automatically

### Mobile Deployment

**Android**
```
# From Project Settings > Android, configure SDK paths, then:
File > Package Project > Android (ETC2 / ASTC)
```

**iOS**
```
# From Project Settings > iOS, set your provisioning profile, then:
File > Package Project > iOS
```

---

## 🕹️ Controls

| Action | Mobile | Keyboard/Mouse | Gamepad |
|---|---|---|---|
| Move | Left joystick | WASD | Left stick |
| Look | Right joystick | Mouse | Right stick |
| Fire | Fire button | LMB | RT |
| Aim Down Sights | ADS button | RMB | LT |
| Reload | Reload button | R | □ |
| Jump | Jump button | Space | A / Cross |
| Crouch | Crouch button | L-Ctrl | B / Circle |
| Sprint | Sprint button | L-Shift | L3 |
| Swap Weapon | Swap button | Q | RB |
| Scoreboard | — | Tab | Menu |

---

## 🎮 Game Modes

| Mode | Description | Players | Time |
|---|---|---|---|
| **Team Deathmatch** | Two teams race to 75 kills | 2–10 | 10 min |
| **Free-for-All** | Every player for themselves; first to 30 kills wins | 2–12 | 8 min |
| **Capture The Flag** | Steal the enemy flag and return it to base | 4–10 | 12 min |
| **Domination** | Hold the most objectives to accumulate points | 4–10 | 10 min |
| **Battle Royale** | Last player / team standing wins | 2–10 | 15 min |

---

## ⚙️ Architecture Overview

- **Authoritative Server Model** — all gameplay logic (damage, kills, respawn) runs on the server; results are replicated to clients.
- **Gameplay Ability System (GAS)** — characters use `UAbilitySystemComponent` for abilities and attribute management, extensible without code changes.
- **Health Component** (`UMSHealthComponent`) — shield-then-health damage model with configurable auto-regeneration.
- **Replicated Weapon System** — `AWeaponBase` uses `Server_Fire` RPCs + `NetMulticast` for visual effects so all clients see the same muzzle flashes and impacts.
- **Behavior Tree AI** — `AMSBotController` drives bot navigation and combat via standard UE5 BT/Blackboard assets.

---

## 📄 License

Copyright 2024 MobileShooter Studio. All Rights Reserved.