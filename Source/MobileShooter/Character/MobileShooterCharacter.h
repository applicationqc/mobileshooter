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

	// ─── Interface ────────────────────────────────────────────────────────

	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Equip a weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(AWeaponBase* WeaponToEquip);

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
	void OnRep_MovementState();

private:
	/** Spawn and attach the default loadout weapons */
	void SpawnDefaultLoadout();

	/** Last controller that dealt damage (for kill credit) */
	TWeakObjectPtr<AController> LastDamageInstigator;

	bool bIsDead = false;
};
