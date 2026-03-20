// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MSTypes.generated.h"

// ─── Enumerations ────────────────────────────────────────────────────────────

/** All available game modes */
UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	TeamDeathmatch      UMETA(DisplayName = "Team Deathmatch"),
	Deathmatch          UMETA(DisplayName = "Free-for-All"),
	CaptureTheFlag      UMETA(DisplayName = "Capture The Flag"),
	Domination          UMETA(DisplayName = "Domination"),
	BattleRoyale        UMETA(DisplayName = "Battle Royale"),
	Sniper              UMETA(DisplayName = "Sniper Only"),
	GunGame             UMETA(DisplayName = "Gun Game"),
};

/** Weapon category */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AssaultRifle        UMETA(DisplayName = "Assault Rifle"),
	SniperRifle         UMETA(DisplayName = "Sniper Rifle"),
	Shotgun             UMETA(DisplayName = "Shotgun"),
	SMG                 UMETA(DisplayName = "SMG"),
	Pistol              UMETA(DisplayName = "Pistol"),
	RocketLauncher      UMETA(DisplayName = "Rocket Launcher"),
	Grenade             UMETA(DisplayName = "Grenade"),
	Melee               UMETA(DisplayName = "Melee"),
};

/** Fire mode */
UENUM(BlueprintType)
enum class EFireMode : uint8
{
	Single              UMETA(DisplayName = "Single"),
	Burst               UMETA(DisplayName = "Burst"),
	Automatic           UMETA(DisplayName = "Automatic"),
};

/** Player movement state */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle                UMETA(DisplayName = "Idle"),
	Walking             UMETA(DisplayName = "Walking"),
	Running             UMETA(DisplayName = "Running"),
	Crouching           UMETA(DisplayName = "Crouching"),
	Prone               UMETA(DisplayName = "Prone"),
	Jumping             UMETA(DisplayName = "Jumping"),
	Falling             UMETA(DisplayName = "Falling"),
};

/** Team identifier */
UENUM(BlueprintType)
enum class ETeam : uint8
{
	NoTeam              UMETA(DisplayName = "No Team"),
	TeamAlpha           UMETA(DisplayName = "Team Alpha"),
	TeamBravo           UMETA(DisplayName = "Team Bravo"),
};

// ─── Structs ─────────────────────────────────────────────────────────────────

/** Weapon statistics */
USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()

	/** Base damage per bullet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float BaseDamage = 25.f;

	/** Headshot damage multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float HeadshotMultiplier = 2.5f;

	/** Rounds per minute */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float FireRate = 600.f;

	/** Max effective range in cm */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float MaxRange = 5000.f;

	/** Bullet spread (degrees) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float Spread = 1.f;

	/** Magazine capacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	int32 MagazineSize = 30;

	/** Total reserve ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	int32 MaxAmmo = 120;

	/** Time to reload in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float ReloadTime = 2.f;
};

/** Player loadout */
USTRUCT(BlueprintType)
struct FPlayerLoadout
{
	GENERATED_BODY()

	/** Primary weapon class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	TSubclassOf<class AWeaponBase> PrimaryWeapon;

	/** Secondary weapon class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	TSubclassOf<class AWeaponBase> SecondaryWeapon;

	/** Throwable/equipment class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	TSubclassOf<class AWeaponBase> Equipment;
};

/** Match result for a single player */
USTRUCT(BlueprintType)
struct FMatchResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Match")
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly, Category = "Match")
	int32 Kills = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Match")
	int32 Deaths = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Match")
	int32 Assists = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Match")
	int32 Score = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Match")
	int32 KillStreak = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Match")
	ETeam Team = ETeam::NoTeam;
};
