// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MSHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChanged, float, NewShield, float, MaxShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

/**
 * Manages health, shields, and regeneration for any actor that owns it.
 */
UCLASS(ClassGroup=(MobileShooter), meta=(BlueprintSpawnableComponent))
class MOBILESHOOTER_API UMSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMSHealthComponent();

	// ─── Configuration ────────────────────────────────────────────────────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxShield = 50.f;

	/** Seconds after taking damage before health regeneration starts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health|Regen")
	float RegenDelay = 5.f;

	/** Health regenerated per second */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health|Regen")
	float RegenRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health|Regen")
	bool bAutoRegen = true;

	/** Whether the shield also regenerates automatically */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health|Regen")
	bool bShieldRegen = true;

	/** Extra seconds after taking damage before shield regeneration starts (stacked on top of RegenDelay) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health|Regen")
	float ShieldRegenDelay = 8.f;

	/** Shield regenerated per second */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health|Regen")
	float ShieldRegenRate = 15.f;

	// ─── State ────────────────────────────────────────────────────────────

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly, Category = "Health")
	float CurrentHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentShield, BlueprintReadOnly, Category = "Health")
	float CurrentShield = 50.f;

	// ─── Events ───────────────────────────────────────────────────────────

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnShieldChanged OnShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnDead OnDead;

	// ─── Interface ────────────────────────────────────────────────────────

	/** Apply damage, depleting shield first then health */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyDamage(float DamageAmount);

	/** Restore health up to MaxHealth */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	/** Restore shield up to MaxShield */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void RestoreShield(float ShieldAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetCurrentShield() const { return CurrentShield; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetShieldPercent() const;

	// ─── Overrides ────────────────────────────────────────────────────────

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_CurrentShield();

	FTimerHandle RegenTimerHandle;
	float TimeSinceLastDamage = 0.f;
	bool bRegenActive = false;
};
