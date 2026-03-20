// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Types/MSTypes.h"
#include "MobileShooterCharacter.generated.h"

class UAbilitySystemComponent;
class UMSAttributeSet;
class AWeaponBase;
class USpringArmComponent;
class UCameraComponent;
class UMSHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDeath,
	AMobileShooterCharacter*, DeadCharacter,
	AController*, KillerController);

/**
 * Main playable character for MobileShooter.
 * Supports Gameplay Ability System, weapon swapping, mobile touch controls,
 * health/damage, and smooth third-person movement.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API AMobileShooterCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMobileShooterCharacter();

	// ─── Components ───────────────────────────────────────────────────────

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UMSHealthComponent> HealthComponent;

	// ─── Weapon ───────────────────────────────────────────────────────────

	/** Current equipped weapon */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AWeaponBase> CurrentWeapon;

	/** Weapon stored in slot 2 (secondary) */
	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AWeaponBase> SecondaryWeapon;

	/** Default loadout applied on spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FPlayerLoadout DefaultLoadout;

	/** Socket name on the character mesh where the weapon attaches */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponAttachSocketName = TEXT("weapon_r");

	// ─── Movement ─────────────────────────────────────────────────────────

	/** Current movement state (replicated for animation) */
	UPROPERTY(ReplicatedUsing = OnRep_MovementState, BlueprintReadOnly, Category = "Movement")
	EMovementState MovementState = EMovementState::Idle;

	/** Walk speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 400.f;

	/** Sprint speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 700.f;

	/** Crouch speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed = 200.f;

	// ─── ADS ──────────────────────────────────────────────────────────────

	/** Field-of-view multiplier when aiming down sights (< 1 zooms in) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ADS")
	float ADSFOVMultiplier = 0.65f;

	/** Interpolation speed for ADS camera zoom transition */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ADS")
	float ADSInterpSpeed = 10.f;

	/** Walk speed while aiming down sights */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ADS")
	float ADSWalkSpeed = 250.f;

	// ─── Interface ────────────────────────────────────────────────────────

	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Equip a weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(AWeaponBase* WeaponToEquip);

	/** Swap between primary and secondary weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwapWeapon();

	/** Start firing */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartFire();

	/** Stop firing */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopFire();

	/** Reload current weapon */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Reload();

	/** Begin sprinting */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartSprint();

	/** Stop sprinting */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopSprint();

	/** Begin crouching */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartCrouch();

	/** Stop crouching */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopCrouch();

	/** Enter Aim Down Sights mode */
	UFUNCTION(BlueprintCallable, Category = "ADS")
	void StartADS();

	/** Exit Aim Down Sights mode */
	UFUNCTION(BlueprintCallable, Category = "ADS")
	void StopADS();

	/** True when the character is aiming down sights */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ADS")
	bool IsAiming() const { return bIsAiming; }

	/** Apply damage to this character */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;

	/** True when the character is dead */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	bool IsDead() const;

	// ─── Events ───────────────────────────────────────────────────────────

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnCharacterDeath OnCharacterDeath;

	// ─── Overrides ────────────────────────────────────────────────────────

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/** Called when the character has no health remaining */
	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void OnDeath(AController* KillerController);
	virtual void OnDeath_Implementation(AController* KillerController);

	UFUNCTION()
	void OnRep_CurrentWeapon();

	UFUNCTION()
	void OnRep_SecondaryWeapon();

	UFUNCTION()
	void OnRep_MovementState();

private:
	/** Spawn and attach the default loadout weapons */
	void SpawnDefaultLoadout();

	/** Hide a weapon by detaching it and disabling its visibility */
	void HideWeapon(AWeaponBase* Weapon);

	/** Show a weapon by re-attaching it */
	void ShowWeapon(AWeaponBase* Weapon);

	/** Last controller that dealt damage (for kill credit) */
	TWeakObjectPtr<AController> LastDamageInstigator;

	/** Map of controllers that dealt damage this life (for assist awards) */
	TMap<TWeakObjectPtr<AController>, float> DamageContributors;

	bool bIsDead = false;

	/** True while the character is aiming down sights */
	bool bIsAiming = false;

	/** Default (non-ADS) camera field of view, cached on BeginPlay */
	float DefaultCameraFOV = 90.f;
};
