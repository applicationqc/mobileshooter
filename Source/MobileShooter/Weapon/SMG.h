// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "SMG.generated.h"

/**
 * Submachine Gun — fast fire rate, low damage per bullet, large magazine.
 * Ideal for close-quarters engagements.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API ASMG : public AWeaponBase
{
	GENERATED_BODY()

public:
	ASMG();
};
