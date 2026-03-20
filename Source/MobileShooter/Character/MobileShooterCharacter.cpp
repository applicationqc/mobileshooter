// Copyright 2024 MobileShooter. All Rights Reserved.

#include "Character/MobileShooterCharacter.h"
#include "Weapon/WeaponBase.h"
#include "Components/MSHealthComponent.h"
#include "GameMode/MobileShooterGameMode.h"
#include "PlayerState/MobileShooterPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AMobileShooterCharacter::AMobileShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Spring arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Health component
	HealthComponent = CreateDefaultSubobject<UMSHealthComponent>(TEXT("HealthComponent"));

	// Movement defaults
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bUseControllerRotationYaw = false;
}

void AMobileShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Cache the camera's default FOV for ADS interpolation
	if (FollowCamera)
	{
		DefaultCameraFOV = FollowCamera->FieldOfView;
	}

	if (HasAuthority())
	{
		SpawnDefaultLoadout();
	}
}

void AMobileShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smoothly interpolate camera FOV for ADS
	if (FollowCamera)
	{
		const float TargetFOV = bIsAiming ? DefaultCameraFOV * ADSFOVMultiplier : DefaultCameraFOV;
		FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV,
		                                              DeltaTime, ADSInterpSpeed);
	}
}

void AMobileShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMobileShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMobileShooterCharacter, CurrentWeapon);
	DOREPLIFETIME(AMobileShooterCharacter, SecondaryWeapon);
	DOREPLIFETIME(AMobileShooterCharacter, MovementState);
}

UAbilitySystemComponent* AMobileShooterCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMobileShooterCharacter::EquipWeapon(AWeaponBase* WeaponToEquip)
{
	if (!HasAuthority() || !WeaponToEquip)
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}

	CurrentWeapon = WeaponToEquip;
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->AttachToComponent(GetMesh(),
	                                  FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                  WeaponAttachSocketName);
}

void AMobileShooterCharacter::SwapWeapon()
{
	if (!HasAuthority() || bIsDead || !SecondaryWeapon)
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		HideWeapon(CurrentWeapon);
	}

	AWeaponBase* OldPrimary = CurrentWeapon;
	CurrentWeapon   = SecondaryWeapon;
	SecondaryWeapon = OldPrimary;

	if (CurrentWeapon)
	{
		ShowWeapon(CurrentWeapon);
	}
}

void AMobileShooterCharacter::StartFire()
{
	if (CurrentWeapon && !bIsDead)
	{
		CurrentWeapon->StartFire();
	}
}

void AMobileShooterCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AMobileShooterCharacter::Reload()
{
	if (CurrentWeapon && !bIsDead)
	{
		CurrentWeapon->Reload();
	}
}

void AMobileShooterCharacter::StartSprint()
{
	if (bIsAiming)
	{
		StopADS();
	}
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	MovementState = EMovementState::Running;
}

void AMobileShooterCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? ADSWalkSpeed : WalkSpeed;
	MovementState = EMovementState::Walking;
}

void AMobileShooterCharacter::StartCrouch()
{
	if (!bIsDead)
	{
		Crouch();
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		MovementState = EMovementState::Crouching;
	}
}

void AMobileShooterCharacter::StopCrouch()
{
	UnCrouch();
	GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? ADSWalkSpeed : WalkSpeed;
	MovementState = EMovementState::Walking;
}

void AMobileShooterCharacter::StartADS()
{
	if (bIsDead)
	{
		return;
	}

	// Sprinting and ADS are mutually exclusive — cancel sprint first
	if (MovementState == EMovementState::Running)
	{
		StopSprint();
	}

	bIsAiming = true;
	GetCharacterMovement()->MaxWalkSpeed = ADSWalkSpeed;
}

void AMobileShooterCharacter::StopADS()
{
	bIsAiming = false;
	GetCharacterMovement()->MaxWalkSpeed =
		(MovementState == EMovementState::Crouching) ? CrouchSpeed : WalkSpeed;
}

float AMobileShooterCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
                                           AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bIsDead || Damage <= 0.f)
	{
		return 0.f;
	}

	if (EventInstigator)
	{
		LastDamageInstigator = EventInstigator;

		// Accumulate damage dealt by each contributor for assist detection
		if (HasAuthority())
		{
			TWeakObjectPtr<AController> InstigatorPtr(EventInstigator);
			float& Accumulated = DamageContributors.FindOrAdd(InstigatorPtr);
			Accumulated += Damage;
		}
	}

	HealthComponent->ApplyDamage(Damage);

	if (HealthComponent->GetCurrentHealth() <= 0.f)
	{
		OnDeath(LastDamageInstigator.Get());
	}

	return Damage;
}

bool AMobileShooterCharacter::IsDead() const
{
	return bIsDead;
}

void AMobileShooterCharacter::OnDeath_Implementation(AController* KillerController)
{
	bIsDead = true;

	if (bIsAiming)
	{
		StopADS();
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}

	// Award assists to any contributor who dealt damage but is not the killer
	if (HasAuthority())
	{
		for (auto& Pair : DamageContributors)
		{
			AController* Contributor = Pair.Key.Get();
			if (Contributor && Contributor != KillerController)
			{
				AMobileShooterPlayerState* PS = Contributor->GetPlayerState<AMobileShooterPlayerState>();
				if (PS)
				{
					PS->AddAssist();
				}
			}
		}
		DamageContributors.Empty();
	}

	// Notify game mode
	AMobileShooterGameMode* GM = GetWorld()->GetAuthGameMode<AMobileShooterGameMode>();
	if (GM)
	{
		GM->OnPlayerEliminated(this, GetController(), KillerController);
	}

	OnCharacterDeath.Broadcast(this, KillerController);
}

void AMobileShooterCharacter::OnRep_CurrentWeapon()
{
	// Re-attach on clients after replication
	if (CurrentWeapon)
	{
		ShowWeapon(CurrentWeapon);
	}
}

void AMobileShooterCharacter::OnRep_SecondaryWeapon()
{
	// Keep secondary weapon hidden on clients
	if (SecondaryWeapon)
	{
		HideWeapon(SecondaryWeapon);
	}
}

void AMobileShooterCharacter::OnRep_MovementState()
{
	// Update client-side movement parameters to match server state
	switch (MovementState)
	{
	case EMovementState::Running:
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		break;
	case EMovementState::Crouching:
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	}
}

void AMobileShooterCharacter::SpawnDefaultLoadout()
{
	FActorSpawnParameters Params;
	Params.Owner     = this;
	Params.Instigator = this;

	if (DefaultLoadout.PrimaryWeapon)
	{
		AWeaponBase* SpawnedPrimary = GetWorld()->SpawnActor<AWeaponBase>(
			DefaultLoadout.PrimaryWeapon, FVector::ZeroVector, FRotator::ZeroRotator, Params);

		if (SpawnedPrimary)
		{
			CurrentWeapon = SpawnedPrimary;
			CurrentWeapon->SetOwner(this);
			ShowWeapon(CurrentWeapon);
		}
	}

	if (DefaultLoadout.SecondaryWeapon)
	{
		AWeaponBase* SpawnedSecondary = GetWorld()->SpawnActor<AWeaponBase>(
			DefaultLoadout.SecondaryWeapon, FVector::ZeroVector, FRotator::ZeroRotator, Params);

		if (SpawnedSecondary)
		{
			SecondaryWeapon = SpawnedSecondary;
			SecondaryWeapon->SetOwner(this);
			HideWeapon(SecondaryWeapon);
		}
	}
}

void AMobileShooterCharacter::HideWeapon(AWeaponBase* Weapon)
{
	if (!Weapon)
	{
		return;
	}
	Weapon->SetActorHiddenInGame(true);
	Weapon->SetActorEnableCollision(false);
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AMobileShooterCharacter::ShowWeapon(AWeaponBase* Weapon)
{
	if (!Weapon)
	{
		return;
	}
	Weapon->SetActorHiddenInGame(false);
	Weapon->SetActorEnableCollision(false); // Weapons don't need world collision when held
	Weapon->AttachToComponent(GetMesh(),
	                           FAttachmentTransformRules::SnapToTargetIncludingScale,
	                           WeaponAttachSocketName);
}
