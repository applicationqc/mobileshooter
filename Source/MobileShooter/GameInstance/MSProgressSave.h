// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UObject/SoftObjectPath.h"
#include "MSProgressSave.generated.h"

/**
 * Serialises per-player progression data to disk.
 * Used by UMobileShooterGameInstance::SavePlayerData / LoadPlayerData.
 */
UCLASS()
class MOBILESHOOTER_API UMSProgressSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 PlayerLevel = 1;

	UPROPERTY()
	int32 CurrentXP = 0;

	UPROPERTY()
	int32 TotalKills = 0;

	UPROPERTY()
	int32 TotalMatches = 0;

	/** Saved loadout: weapon class soft references (relocate-safe) */
	UPROPERTY()
	FSoftClassPath PrimaryWeaponClass;

	UPROPERTY()
	FSoftClassPath SecondaryWeaponClass;
};
