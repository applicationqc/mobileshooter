// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MobileShooterPlayerController.generated.h"

class AMobileShooterHUD;
class AMobileShooterCharacter;

/**
 * Player controller for MobileShooter.
 * Owns the mobile touch input mapping and bridges UI with gameplay.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API AMobileShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMobileShooterPlayerController();

	// ─── HUD ─────────────────────────────────────────────────────────────

	/** Show the in-game HUD overlay */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameHUD();

	/** Show the death/respawn screen */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowRespawnScreen(float RespawnDelay);

	/** Show the post-match scoreboard */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowScoreboard();

	/** Hide all HUD widgets */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideAllHUD();

	// ─── Input ────────────────────────────────────────────────────────────

	/** Fire button pressed (mobile touch) */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnFireButtonPressed();

	/** Fire button released */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnFireButtonReleased();

	/** Reload button pressed */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnReloadButtonPressed();

	/** Sprint button pressed */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnSprintButtonPressed();

	/** Sprint button released */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnSprintButtonReleased();

	/** Jump button pressed */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnJumpButtonPressed();

	/** Joystick move input (virtual joystick on mobile) */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveInput(FVector2D MoveVector);

	/** Joystick look input (virtual joystick on mobile) */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnLookInput(FVector2D LookVector);

	// ─── Overrides ────────────────────────────────────────────────────────

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

protected:
	/** Character currently controlled (convenience cast) */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<AMobileShooterCharacter> ControlledCharacter;
};
