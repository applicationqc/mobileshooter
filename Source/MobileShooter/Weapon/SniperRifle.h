// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "SniperRifle.generated.h"

/**
 * Bolt-action sniper rifle for long-range engagements.
 * High damage, slow fire rate, and tight spread for precision shots.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API ASniperRifle : public AWeaponBase
{
	GENERATED_BODY()

public:
	ASniperRifle();

	/** Zoom field-of-view when scoped in (degrees) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Sniper")
	float ScopedFOV = 20.f;

	/** Whether the player is currently scoped */
	UPROPERTY(ReplicatedUsing = OnRep_bIsScoped, BlueprintReadOnly, Category = "Weapon|Sniper")
	bool bIsScoped = false;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Sniper")
	void ToggleScope();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_bIsScoped();
};
