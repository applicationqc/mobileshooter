// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Pistol.generated.h"

/**
 * Semi-automatic pistol — reliable sidearm for finishing off enemies
 * or when the primary weapon runs dry.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API APistol : public AWeaponBase
{
	GENERATED_BODY()

public:
	APistol();
};
