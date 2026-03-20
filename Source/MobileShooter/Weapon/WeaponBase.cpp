// Copyright 2024 MobileShooter. All Rights Reserved.

#include "Weapon/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Character/MobileShooterCharacter.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo  = WeaponStats.MagazineSize;
	ReserveAmmo  = WeaponStats.MaxAmmo;
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase, CurrentAmmo);
	DOREPLIFETIME(AWeaponBase, ReserveAmmo);
}

bool AWeaponBase::CanFire() const
{
	return !bIsReloading && CurrentAmmo > 0;
}

void AWeaponBase::StartFire()
{
	if (!CanFire())
	{
		if (CurrentAmmo == 0)
		{
			Reload();
		}
		return;
	}

	bIsFiring = true;

	const float FireInterval = 60.f / FMath::Max(1.f, WeaponStats.FireRate);

	if (FireMode == EFireMode::Automatic)
	{
		GetWorldTimerManager().SetTimer(FireTimerHandle,
		                                this, &AWeaponBase::Server_Fire,
		                                FireInterval, true, 0.f);
	}
	else
	{
		Server_Fire();
	}
}

void AWeaponBase::StopFire()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AWeaponBase::Reload()
{
	if (bIsReloading || ReserveAmmo <= 0 || CurrentAmmo >= WeaponStats.MagazineSize)
	{
		return;
	}

	bIsReloading = true;
	StopFire();

	if (ReloadSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
	}

	FTimerDelegate ReloadDelegate;
	ReloadDelegate.BindLambda([this]()
	{
		const int32 Needed  = WeaponStats.MagazineSize - CurrentAmmo;
		const int32 Refill  = FMath::Min(Needed, ReserveAmmo);
		CurrentAmmo  += Refill;
		ReserveAmmo  -= Refill;
		bIsReloading  = false;
		OnReloadComplete.Broadcast();
		OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);
	});

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, ReloadDelegate,
	                                WeaponStats.ReloadTime, false);
}

void AWeaponBase::Fire_Implementation()
{
	if (!CanFire())
	{
		return;
	}

	--CurrentAmmo;
	OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);

	// Line-trace hit detection
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	FVector  TraceStart;
	FRotator TraceRotation;
	OwnerActor->GetActorEyesViewPoint(TraceStart, TraceRotation);

	// Apply spread
	const float HalfRad = FMath::DegreesToRadians(WeaponStats.Spread * 0.5f);
	const FVector ShotDirection = FMath::VRandCone(TraceRotation.Vector(), HalfRad);
	const FVector TraceEnd = TraceStart + ShotDirection * WeaponStats.MaxRange;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerActor);
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd,
	                                                  ECC_Visibility, Params);
	if (bHit)
	{
		SpawnImpactEffect(Hit);

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// Headshot detection via bone name
			const bool bHeadshot = Hit.BoneName.ToString().Contains(TEXT("head"),
			                                                          ESearchCase::IgnoreCase);
			const float Damage = WeaponStats.BaseDamage *
			                     (bHeadshot ? WeaponStats.HeadshotMultiplier : 1.f);

			FDamageEvent DamageEvent;
			HitActor->TakeDamage(Damage, DamageEvent,
			                     OwnerActor->GetInstigatorController(), OwnerActor);
		}
	}

	Multicast_SpawnFireEffects();

	if (CurrentAmmo == 0 && ReserveAmmo > 0)
	{
		Reload();
	}
}

void AWeaponBase::SpawnFireEffects()
{
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponMesh, MuzzleSocketName);
	}
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// Camera shake for local player
	if (FireCameraShake)
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (OwnerPawn && OwnerPawn->IsLocallyControlled())
		{
			APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
			if (PC)
			{
				PC->ClientStartCameraShake(FireCameraShake);
			}
		}
	}
}

void AWeaponBase::SpawnImpactEffect(const FHitResult& Hit)
{
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect,
		                                          Hit.ImpactPoint,
		                                          Hit.ImpactNormal.Rotation());
	}
}

void AWeaponBase::OnRep_CurrentAmmo()
{
	OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);
}

void AWeaponBase::OnRep_ReserveAmmo()
{
	OnAmmoChanged.Broadcast(CurrentAmmo, ReserveAmmo);
}

bool AWeaponBase::Server_Fire_Validate()
{
	return true;
}

void AWeaponBase::Server_Fire_Implementation()
{
	Fire_Implementation();
}

void AWeaponBase::Multicast_SpawnFireEffects_Implementation()
{
	SpawnFireEffects();
}
