// Copyright 2024 MobileShooter. All Rights Reserved.

#include "Character/MobileShooterCharacter.h"
#include "Weapon/WeaponBase.h"
#include "Components/MSHealthComponent.h"
#include "GameMode/MobileShooterGameMode.h"
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

	if (HasAuthority())
	{
		SpawnDefaultLoadout();
	}
}

void AMobileShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobileShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMobileShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMobileShooterCharacter, CurrentWeapon);
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
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	MovementState = EMovementState::Running;
}

void AMobileShooterCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	MovementState = EMovementState::Walking;
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

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
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
		CurrentWeapon->AttachToComponent(GetMesh(),
		                                  FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                  WeaponAttachSocketName);
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
	if (!DefaultLoadout.PrimaryWeapon)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;

	AWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(
		DefaultLoadout.PrimaryWeapon, FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (SpawnedWeapon)
	{
		EquipWeapon(SpawnedWeapon);
	}
}
