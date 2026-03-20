// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types/MSTypes.h"
#include "MobileShooterGameMode.generated.h"

class AMobileShooterPlayerState;
class AMobileShooterCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, FName, NewMatchState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchEnded);

/**
 * Base game mode for all MobileShooter game variants.
 * Handles match lifecycle, scoring, team assignment, and respawn logic.
 */
UCLASS(Abstract, Blueprintable)
class MOBILESHOOTER_API AMobileShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMobileShooterGameMode();

	// ─── Match Settings ───────────────────────────────────────────────────

	/** Duration of a match in seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match|Settings")
	float MatchDuration = 300.f;

	/** Score limit to end the match early (0 = disabled) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match|Settings")
	int32 ScoreLimit = 50;

	/** Respawn delay in seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match|Settings")
	float RespawnDelay = 3.f;

	/** Maximum number of players allowed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match|Settings")
	int32 MaxPlayers = 10;

	/** Game mode type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match|Settings")
	EGameModeType GameModeType = EGameModeType::TeamDeathmatch;

	// ─── Events ───────────────────────────────────────────────────────────

	UPROPERTY(BlueprintAssignable, Category = "Match|Events")
	FOnMatchStateChanged OnMatchStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Match|Events")
	FOnMatchEnded OnMatchEnded;

	// ─── Interface ────────────────────────────────────────────────────────

	/** Called when a player is eliminated */
	UFUNCTION(BlueprintCallable, Category = "Match")
	virtual void OnPlayerEliminated(AMobileShooterCharacter* EliminatedCharacter,
	                                AController* VictimController,
	                                AController* KillerController);

	/** Request a respawn for the given controller */
	UFUNCTION(BlueprintCallable, Category = "Match")
	virtual void RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController);

	/** Add score to a player */
	UFUNCTION(BlueprintCallable, Category = "Match")
	virtual void AddScore(AController* ScoringController, int32 Points);

	/** Get remaining match time in seconds */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Match")
	float GetRemainingMatchTime() const;

	/** Return sorted match results */
	UFUNCTION(BlueprintCallable, Category = "Match")
	TArray<FMatchResult> GetMatchResults() const;

	// ─── Overrides ────────────────────────────────────────────────────────

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	/** Elapsed match time */
	float ElapsedMatchTime = 0.f;

	/** Whether the match is currently active */
	bool bMatchInProgress = false;

	/** Start the match timer and enable gameplay */
	UFUNCTION(BlueprintCallable, Category = "Match")
	void StartMatch();

	/** End the match and broadcast results */
	UFUNCTION(BlueprintCallable, Category = "Match")
	void EndMatch();

private:
	/** Timer handle for respawn */
	FTimerHandle RespawnTimerHandle;
};
