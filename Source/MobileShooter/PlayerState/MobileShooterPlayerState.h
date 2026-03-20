// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Types/MSTypes.h"
#include "MobileShooterPlayerState.generated.h"

/**
 * Stores per-player statistics that persist across respawns:
 * kills, deaths, assists, score, and team assignment.
 * Fully replicated so the scoreboard is always up to date on all clients.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API AMobileShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMobileShooterPlayerState();

	// ─── Stats ────────────────────────────────────────────────────────────

	UPROPERTY(ReplicatedUsing = OnRep_Kills, BlueprintReadOnly, Category = "Stats")
	int32 Kills = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Deaths, BlueprintReadOnly, Category = "Stats")
	int32 Deaths = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Assists, BlueprintReadOnly, Category = "Stats")
	int32 Assists = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Team, BlueprintReadOnly, Category = "Stats")
	ETeam Team = ETeam::NoTeam;

	/** Current unbroken kill streak (resets on death) */
	UPROPERTY(ReplicatedUsing = OnRep_KillStreak, BlueprintReadOnly, Category = "Stats")
	int32 KillStreak = 0;

	/** Longest kill streak achieved this match */
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 BestKillStreak = 0;

	// ─── Interface ────────────────────────────────────────────────────────

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddKill();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddDeath();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddAssist();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetTeam(ETeam NewTeam);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	int32 GetKills() const { return Kills; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	int32 GetDeaths() const { return Deaths; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	int32 GetAssists() const { return Assists; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	int32 GetKillStreak() const { return KillStreak; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	int32 GetBestKillStreak() const { return BestKillStreak; }

	/** K/D ratio (returns 0 when deaths == 0) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetKDRatio() const;

	// ─── Overrides ────────────────────────────────────────────────────────

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_Kills();

	UFUNCTION()
	void OnRep_Deaths();

	UFUNCTION()
	void OnRep_Assists();

	UFUNCTION()
	void OnRep_Team();

	UFUNCTION()
	void OnRep_KillStreak();
};
