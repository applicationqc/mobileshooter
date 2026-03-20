// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/MSTypes.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class USoundBase;
class UCameraShakeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, CurrentAmmo, int32, MaxAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadComplete);

/**
 * Abstract base class for all weapons in MobileShooter.
 * Handles firing, reloading, ammo management, and hit detection.
 * Subclass in Blueprint or C++ to create specific weapons.
 */
UCLASS(Abstract, Blueprintable)
class MOBILESHOOTER_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	// ─── Components ───────────────────────────────────────────────────────

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	// ─── Configuration ────────────────────────────────────────────────────

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Config")
	EWeaponType WeaponType = EWeaponType::AssaultRifle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Config")
	EFireMode FireMode = EFireMode::Automatic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Config")
	FWeaponStats WeaponStats;

	/** Muzzle socket name on the weapon mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Config")
	FName MuzzleSocketName = TEXT("MuzzleSocket");

	// ─── Effects ──────────────────────────────────────────────────────────

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<USoundBase> ReloadSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	// ─── Ammo State (replicated) ──────────────────────────────────────────

	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmo, BlueprintReadOnly, Category = "Weapon|Ammo")
	int32 CurrentAmmo;

	UPROPERTY(ReplicatedUsing = OnRep_ReserveAmmo, BlueprintReadOnly, Category = "Weapon|Ammo")
	int32 ReserveAmmo;

	// ─── Events ───────────────────────────────────────────────────────────

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnAmmoChanged OnAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnReloadComplete OnReloadComplete;

	// ─── Interface ────────────────────────────────────────────────────────

	/** Begin continuous or single firing */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StartFire();

	/** Stop firing */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StopFire();

	/** Trigger a reload */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	bool CanFire() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	bool IsReloading() const { return bIsReloading; }

	// ─── Overrides ────────────────────────────────────────────────────────

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/** Execute a single shot: line-trace and apply damage */
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void Fire();
	virtual void Fire_Implementation();

	/** Spawn muzzle flash and play fire sound */
	void SpawnFireEffects();

	/** Spawn hit effect at impact location */
	void SpawnImpactEffect(const FHitResult& Hit);

	bool bIsFiring   = false;
	bool bIsReloading = false;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;

private:
	UFUNCTION()
	void OnRep_CurrentAmmo();

	UFUNCTION()
	void OnRep_ReserveAmmo();

	/** Server-side fire execution */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();
	bool Server_Fire_Validate();
	void Server_Fire_Implementation();

	/** Multicast fire effects to all clients */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpawnFireEffects();
	void Multicast_SpawnFireEffects_Implementation();
};
