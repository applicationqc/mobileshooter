// Copyright 2024 MobileShooter. All Rights Reserved.

#include "UI/MobileShooterHUD.h"
#include "Blueprint/UserWidget.h"

AMobileShooterHUD::AMobileShooterHUD()
{
}

void AMobileShooterHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMobileShooterHUD::ShowGameHUD()
{
	HideAllHUD();

	if (!GameHUDWidget && GameHUDWidgetClass)
	{
		GameHUDWidget = CreateAndAddWidget(GameHUDWidgetClass);
	}
	else if (GameHUDWidget)
	{
		GameHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMobileShooterHUD::ShowRespawnScreen(float RespawnDelay)
{
	HideAllHUD();

	if (!RespawnScreenWidget && RespawnScreenWidgetClass)
	{
		RespawnScreenWidget = CreateAndAddWidget(RespawnScreenWidgetClass);
	}
	else if (RespawnScreenWidget)
	{
		RespawnScreenWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMobileShooterHUD::ShowScoreboard()
{
	if (!ScoreboardWidget && ScoreboardWidgetClass)
	{
		ScoreboardWidget = CreateAndAddWidget(ScoreboardWidgetClass);
	}
	else if (ScoreboardWidget)
	{
		ScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMobileShooterHUD::HideAllHUD()
{
	auto HideWidget = [](UUserWidget* Widget)
	{
		if (Widget)
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	};

	HideWidget(GameHUDWidget);
	HideWidget(RespawnScreenWidget);
	HideWidget(ScoreboardWidget);
}

UUserWidget* AMobileShooterHUD::CreateAndAddWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (!WidgetClass)
	{
		return nullptr;
	}

	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return nullptr;
	}

	UUserWidget* Widget = CreateWidget<UUserWidget>(PC, WidgetClass);
	if (Widget)
	{
		Widget->AddToViewport();
	}
	return Widget;
}
