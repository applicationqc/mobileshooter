// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MobileShooterHUD.generated.h"

class UUserWidget;

/**
 * HUD class that manages all UMG widget layers for MobileShooter.
 * Creates and shows/hides the Game HUD, Respawn Screen, and Scoreboard.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API AMobileShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMobileShooterHUD();

	// ─── Widget Classes (set in Blueprint subclass) ───────────────────────

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Widgets")
	TSubclassOf<UUserWidget> GameHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Widgets")
	TSubclassOf<UUserWidget> RespawnScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Widgets")
	TSubclassOf<UUserWidget> ScoreboardWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Widgets")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	// ─── Interface ────────────────────────────────────────────────────────

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowRespawnScreen(float RespawnDelay);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowScoreboard();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideAllHUD();

	// ─── Overrides ────────────────────────────────────────────────────────

	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "HUD|Widgets")
	TObjectPtr<UUserWidget> GameHUDWidget;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Widgets")
	TObjectPtr<UUserWidget> RespawnScreenWidget;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Widgets")
	TObjectPtr<UUserWidget> ScoreboardWidget;

private:
	/** Helper to create and add a widget to the viewport */
	UUserWidget* CreateAndAddWidget(TSubclassOf<UUserWidget> WidgetClass);
};
