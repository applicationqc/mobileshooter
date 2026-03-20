// Copyright 2024 MobileShooter. All Rights Reserved.

#include "PlayerController/MobileShooterPlayerController.h"
#include "Character/MobileShooterCharacter.h"
#include "UI/MobileShooterHUD.h"

AMobileShooterPlayerController::AMobileShooterPlayerController()
{
}

void AMobileShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowGameHUD();
}

void AMobileShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ControlledCharacter = Cast<AMobileShooterCharacter>(InPawn);
}

void AMobileShooterPlayerController::ShowGameHUD()
{
	if (AMobileShooterHUD* HUD = GetHUD<AMobileShooterHUD>())
	{
		HUD->ShowGameHUD();
	}
}

void AMobileShooterPlayerController::ShowRespawnScreen(float RespawnDelay)
{
	if (AMobileShooterHUD* HUD = GetHUD<AMobileShooterHUD>())
	{
		HUD->ShowRespawnScreen(RespawnDelay);
	}
}

void AMobileShooterPlayerController::ShowScoreboard()
{
	if (AMobileShooterHUD* HUD = GetHUD<AMobileShooterHUD>())
	{
		HUD->ShowScoreboard();
	}
}

void AMobileShooterPlayerController::HideAllHUD()
{
	if (AMobileShooterHUD* HUD = GetHUD<AMobileShooterHUD>())
	{
		HUD->HideAllHUD();
	}
}

// ─── Input handlers ──────────────────────────────────────────────────────────

void AMobileShooterPlayerController::OnFireButtonPressed()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->StartFire();
	}
}

void AMobileShooterPlayerController::OnFireButtonReleased()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->StopFire();
	}
}

void AMobileShooterPlayerController::OnReloadButtonPressed()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->Reload();
	}
}

void AMobileShooterPlayerController::OnSprintButtonPressed()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->StartSprint();
	}
}

void AMobileShooterPlayerController::OnSprintButtonReleased()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->StopSprint();
	}
}

void AMobileShooterPlayerController::OnJumpButtonPressed()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void AMobileShooterPlayerController::OnMoveInput(FVector2D MoveVector)
{
	if (!ControlledCharacter || MoveVector.IsNearlyZero())
	{
		return;
	}

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	ControlledCharacter->AddMovementInput(ForwardDir, MoveVector.Y);
	ControlledCharacter->AddMovementInput(RightDir, MoveVector.X);
}

void AMobileShooterPlayerController::OnLookInput(FVector2D LookVector)
{
	if (LookVector.IsNearlyZero())
	{
		return;
	}

	AddYawInput(LookVector.X);
	AddPitchInput(-LookVector.Y);
}
