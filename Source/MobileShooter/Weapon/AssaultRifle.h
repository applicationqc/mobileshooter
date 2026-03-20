// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "AssaultRifle.generated.h"

/**
 * Full-auto assault rifle — the default primary weapon.
 * Balanced damage, range, and fire rate suitable for most combat situations.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API AAssaultRifle : public AWeaponBase
{
	GENERATED_BODY()

public:
	AAssaultRifle();
};
