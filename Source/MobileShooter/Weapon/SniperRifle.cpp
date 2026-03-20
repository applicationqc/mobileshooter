// Copyright 2024 MobileShooter. All Rights Reserved.

#include "Weapon/SniperRifle.h"
#include "Net/UnrealNetwork.h"
#include "Character/MobileShooterCharacter.h"
#include "Camera/CameraComponent.h"

ASniperRifle::ASniperRifle()
{
	WeaponType = EWeaponType::SniperRifle;
	FireMode   = EFireMode::Single;

	WeaponStats.BaseDamage         = 95.f;
	WeaponStats.HeadshotMultiplier = 3.0f;
	WeaponStats.FireRate           = 40.f;    // rounds per minute
	WeaponStats.MaxRange           = 20000.f; // cm
	WeaponStats.Spread             = 0.1f;    // very tight spread
	WeaponStats.MagazineSize       = 5;
	WeaponStats.MaxAmmo            = 25;
	WeaponStats.ReloadTime         = 3.5f;
}

void ASniperRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASniperRifle, bIsScoped);
}

void ASniperRifle::ToggleScope()
{
	bIsScoped = !bIsScoped;
	OnRep_bIsScoped();
}

void ASniperRifle::OnRep_bIsScoped()
{
	// Notify owning character to adjust camera FOV
	AMobileShooterCharacter* OwnerChar = Cast<AMobileShooterCharacter>(GetOwner());
	if (OwnerChar && OwnerChar->FollowCamera)
	{
		OwnerChar->FollowCamera->SetFieldOfView(bIsScoped ? ScopedFOV : 90.f);
	}
}
