// Copyright 2024 MobileShooter. All Rights Reserved.

#include "Components/MSHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

UMSHealthComponent::UMSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	CurrentHealth = MaxHealth;
	CurrentShield = MaxShield;
}

void UMSHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	CurrentShield = MaxShield;
}

void UMSHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMSHealthComponent, CurrentHealth);
	DOREPLIFETIME(UMSHealthComponent, CurrentShield);
}

void UMSHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bAutoRegen || CurrentHealth <= 0.f)
	{
		return;
	}

	if (bRegenActive)
	{
		TimeSinceLastDamage += DeltaTime;

		// Health regen
		if (TimeSinceLastDamage >= RegenDelay && CurrentHealth < MaxHealth)
		{
			Heal(RegenRate * DeltaTime);
		}

		// Shield regen (uses a longer delay so it kicks in after health)
		if (bShieldRegen && TimeSinceLastDamage >= ShieldRegenDelay && CurrentShield < MaxShield)
		{
			RestoreShield(ShieldRegenRate * DeltaTime);
		}
	}
}

void UMSHealthComponent::ApplyDamage(float DamageAmount)
{
	if (DamageAmount <= 0.f || CurrentHealth <= 0.f)
	{
		return;
	}

	// Deplete shield first
	if (CurrentShield > 0.f)
	{
		const float ShieldAbsorb = FMath::Min(CurrentShield, DamageAmount);
		CurrentShield -= ShieldAbsorb;
		DamageAmount  -= ShieldAbsorb;
		OnRep_CurrentShield();
	}

	if (DamageAmount > 0.f)
	{
		CurrentHealth = FMath::Max(0.f, CurrentHealth - DamageAmount);
		OnRep_CurrentHealth();
	}

	// Reset regen timer
	bRegenActive = true;
	TimeSinceLastDamage = 0.f;

	if (CurrentHealth <= 0.f)
	{
		OnDead.Broadcast();
	}
}

void UMSHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.f || CurrentHealth <= 0.f)
	{
		return;
	}
	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + HealAmount);
	OnRep_CurrentHealth();
}

void UMSHealthComponent::RestoreShield(float ShieldAmount)
{
	if (ShieldAmount <= 0.f)
	{
		return;
	}
	CurrentShield = FMath::Min(MaxShield, CurrentShield + ShieldAmount);
	OnRep_CurrentShield();
}

float UMSHealthComponent::GetHealthPercent() const
{
	return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f;
}

float UMSHealthComponent::GetShieldPercent() const
{
	return MaxShield > 0.f ? CurrentShield / MaxShield : 0.f;
}

void UMSHealthComponent::OnRep_CurrentHealth()
{
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UMSHealthComponent::OnRep_CurrentShield()
{
	OnShieldChanged.Broadcast(CurrentShield, MaxShield);
}
