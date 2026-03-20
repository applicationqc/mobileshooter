// Copyright 2024 MobileShooter. All Rights Reserved.

#include "Weapon/Shotgun.h"
#include "Character/MobileShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

AShotgun::AShotgun()
{
	WeaponType = EWeaponType::Shotgun;
	FireMode   = EFireMode::Single;

	WeaponStats.BaseDamage         = 15.f;  // per pellet
	WeaponStats.HeadshotMultiplier = 2.0f;
	WeaponStats.FireRate           = 60.f;  // rounds per minute
	WeaponStats.MaxRange           = 1500.f;
	WeaponStats.Spread             = 8.f;   // wide spread
	WeaponStats.MagazineSize       = 6;
	WeaponStats.MaxAmmo            = 36;
	WeaponStats.ReloadTime         = 2.5f;
}

void AShotgun::Fire_Implementation()
{
	if (!CanFire())
	{
		return;
	}

	--CurrentAmmo;
	OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	FVector  TraceStart;
	FRotator TraceRotation;
	OwnerActor->GetActorEyesViewPoint(TraceStart, TraceRotation);

	const float HalfRad = FMath::DegreesToRadians(WeaponStats.Spread * 0.5f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerActor);
	Params.bTraceComplex = true;

	// Fire PelletCount independent line-traces, each with its own random spread direction.
	// Each pellet performs its own hit detection and damage application so that
	// partially-obscured targets (e.g. limbs behind cover) only receive a fraction of
	// the total pellets.  Full test coverage of this loop should verify:
	//   1. Exactly PelletCount traces are issued per shot.
	//   2. Each pellet applies BaseDamage independently (with headshot multiplier when applicable).
	//   3. Ammo decrements by 1 (not by PelletCount) per trigger pull.
	for (int32 i = 0; i < PelletCount; ++i)
	{
		const FVector ShotDir = FMath::VRandCone(TraceRotation.Vector(), HalfRad);
		const FVector TraceEnd = TraceStart + ShotDir * WeaponStats.MaxRange;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				const bool bHeadshot = Hit.BoneName.ToString().Contains(TEXT("head"),
				                                                          ESearchCase::IgnoreCase);
				const float Damage = WeaponStats.BaseDamage *
				                     (bHeadshot ? WeaponStats.HeadshotMultiplier : 1.f);

				FDamageEvent DamageEvent;
				HitActor->TakeDamage(Damage, DamageEvent,
				                     OwnerActor->GetInstigatorController(), OwnerActor);
			}
		}
	}

	if (CurrentAmmo == 0 && ReserveAmmo > 0)
	{
		Reload();
	}
}
