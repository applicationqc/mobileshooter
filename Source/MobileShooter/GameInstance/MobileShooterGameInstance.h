// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Types/MSTypes.h"
#include "MobileShooterGameInstance.generated.h"

/**
 * Game Instance for MobileShooter.
 * Persists across level loads and holds:
 *  - Player preferences (name, loadout)
 *  - Online session management helpers
 *  - Unlocked weapons / progression data
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API UMobileShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMobileShooterGameInstance();

	// ─── Player Preferences ───────────────────────────────────────────────

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FString LocalPlayerName = TEXT("Soldier");

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FPlayerLoadout SavedLoadout;

	// ─── Progression ──────────────────────────────────────────────────────

	/** Total kills across all sessions */
	UPROPERTY(BlueprintReadWrite, Category = "Progression")
	int32 TotalKills = 0;

	/** Total matches played */
	UPROPERTY(BlueprintReadWrite, Category = "Progression")
	int32 TotalMatches = 0;

	/** Player level (increases with XP) */
	UPROPERTY(BlueprintReadWrite, Category = "Progression")
	int32 PlayerLevel = 1;

	/** Current XP towards next level */
	UPROPERTY(BlueprintReadWrite, Category = "Progression")
	int32 CurrentXP = 0;

	/** Unlocked weapon classes */
	UPROPERTY(BlueprintReadWrite, Category = "Progression")
	TArray<TSubclassOf<class AWeaponBase>> UnlockedWeapons;

	// ─── Session Settings ─────────────────────────────────────────────────

	/** Game mode selected in lobby */
	UPROPERTY(BlueprintReadWrite, Category = "Session")
	EGameModeType SelectedGameMode = EGameModeType::TeamDeathmatch;

	/** Map name selected in lobby */
	UPROPERTY(BlueprintReadWrite, Category = "Session")
	FString SelectedMap = TEXT("Map_Desert");

	// ─── Interface ────────────────────────────────────────────────────────

	/** Add XP and level up if threshold is reached */
	UFUNCTION(BlueprintCallable, Category = "Progression")
	void AddXP(int32 XPAmount);

	/** Save player data to a local save game */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SavePlayerData();

	/** Load player data from a local save game */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadPlayerData();

	// ─── Overrides ────────────────────────────────────────────────────────

	virtual void Init() override;

private:
	/** XP required per level (linear scale) */
	static constexpr int32 XPPerLevel = 1000;
};
