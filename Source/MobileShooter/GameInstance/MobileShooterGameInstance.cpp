// Copyright 2024 MobileShooter. All Rights Reserved.

#include "GameInstance/MobileShooterGameInstance.h"
#include "GameInstance/MSProgressSave.h"
#include "Weapon/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

const FString UMobileShooterGameInstance::SaveSlotName = TEXT("PlayerProfile");

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
	UMSProgressSave* SaveObject = Cast<UMSProgressSave>(
		UGameplayStatics::CreateSaveGameObject(UMSProgressSave::StaticClass()));

	if (!SaveObject)
	{
		return;
	}

	SaveObject->PlayerName    = LocalPlayerName;
	SaveObject->PlayerLevel   = PlayerLevel;
	SaveObject->CurrentXP     = CurrentXP;
	SaveObject->TotalKills    = TotalKills;
	SaveObject->TotalMatches  = TotalMatches;

	// Serialize weapon class references as FSoftClassPath (relocate-safe)
	if (SavedLoadout.PrimaryWeapon)
	{
		SaveObject->PrimaryWeaponClass = FSoftClassPath(SavedLoadout.PrimaryWeapon.Get());
	}
	if (SavedLoadout.SecondaryWeapon)
	{
		SaveObject->SecondaryWeaponClass = FSoftClassPath(SavedLoadout.SecondaryWeapon.Get());
	}

	UGameplayStatics::AsyncSaveGameToSlot(SaveObject, SaveSlotName, 0);
}

void UMobileShooterGameInstance::LoadPlayerData()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		return;
	}

	UMSProgressSave* SaveObject = Cast<UMSProgressSave>(
		UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	if (!SaveObject)
	{
		return;
	}

	LocalPlayerName = SaveObject->PlayerName;
	PlayerLevel     = FMath::Max(1, SaveObject->PlayerLevel);
	CurrentXP       = FMath::Max(0, SaveObject->CurrentXP);
	TotalKills      = FMath::Max(0, SaveObject->TotalKills);
	TotalMatches    = FMath::Max(0, SaveObject->TotalMatches);

	// Restore weapon classes from saved soft paths
	if (SaveObject->PrimaryWeaponClass.IsValid())
	{
		UClass* PrimaryClass = SaveObject->PrimaryWeaponClass.TryLoadClass<AWeaponBase>();
		if (PrimaryClass)
		{
			SavedLoadout.PrimaryWeapon = PrimaryClass;
		}
	}
	if (SaveObject->SecondaryWeaponClass.IsValid())
	{
		UClass* SecondaryClass = SaveObject->SecondaryWeaponClass.TryLoadClass<AWeaponBase>();
		if (SecondaryClass)
		{
			SavedLoadout.SecondaryWeapon = SecondaryClass;
		}
	}
}
