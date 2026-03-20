// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Shotgun.generated.h"

/**
 * Pump-action shotgun for close-range combat.
 * Fires multiple pellets per shot.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API AShotgun : public AWeaponBase
{
	GENERATED_BODY()

public:
	AShotgun();

	/** Number of pellets fired per shot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Shotgun")
	int32 PelletCount = 8;

protected:
	virtual void Fire_Implementation() override;
};
