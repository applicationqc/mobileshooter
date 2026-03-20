// Copyright 2024 MobileShooter. All Rights Reserved.

#include "GameInstance/MobileShooterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"

UMobileShooterGameInstance::UMobileShooterGameInstance()
{
}

void UMobileShooterGameInstance::Init()
{
	Super::Init();
	LoadPlayerData();
}

void UMobileShooterGameInstance::AddXP(int32 XPAmount)
{
	if (XPAmount <= 0)
	{
		return;
	}

	CurrentXP += XPAmount;

	while (CurrentXP >= XPPerLevel)
	{
		CurrentXP -= XPPerLevel;
		++PlayerLevel;
	}
}

void UMobileShooterGameInstance::SavePlayerData()
{
	// TODO: Serialize the following fields to a USaveGame-derived class (e.g. UMSSaveGame)
	// and write it via UGameplayStatics::AsyncSaveGameToSlot with slot name "PlayerProfile".
	// Fields to persist:
	//   - LocalPlayerName    (FString)
	//   - TotalKills         (int32)
	//   - TotalMatches       (int32)
	//   - PlayerLevel        (int32)
	//   - CurrentXP          (int32)
	//   - UnlockedWeapons    (TArray<FSoftClassPath> serialized from TSubclassOf pointers)
	//   - SavedLoadout       (FPlayerLoadout – primary/secondary/equipment soft-class references)
	// Migration: bump a save-version integer in the save object so future changes can
	// apply defaults for newly added fields when loading an older save file.
}

void UMobileShooterGameInstance::LoadPlayerData()
{
	// TODO: Call UGameplayStatics::LoadGameFromSlot("PlayerProfile", 0) and cast to UMSSaveGame.
	// Restore all fields listed above.  On first-run (slot absent) keep the defaults defined
	// in the constructor.  On version mismatch, apply field-level defaults for missing data
	// rather than discarding the whole save.
}
