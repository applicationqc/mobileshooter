// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/MobileShooterGameMode.h"
#include "TeamDeathmatchGameMode.generated.h"

/**
 * Team Deathmatch game mode.
 * Players are split into two teams. The team that reaches the kill limit
 * (or has the most kills when time runs out) wins.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API ATeamDeathmatchGameMode : public AMobileShooterGameMode
{
	GENERATED_BODY()

public:
	ATeamDeathmatchGameMode();

	/** Kills required for a team to win */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TDM")
	int32 TeamScoreLimit = 75;

	/** Score for Team Alpha (replicated via GameState) */
	UPROPERTY(BlueprintReadOnly, Category = "TDM")
	int32 TeamAlphaScore = 0;

	/** Score for Team Bravo */
	UPROPERTY(BlueprintReadOnly, Category = "TDM")
	int32 TeamBravoScore = 0;

	virtual void OnPlayerEliminated(AMobileShooterCharacter* EliminatedCharacter,
	                                AController* VictimController,
	                                AController* KillerController) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	/** Assign an incoming player to the smaller team */
	void AssignTeam(APlayerController* NewPlayer);

	int32 AlphaCount = 0;
	int32 BravoCount = 0;
};
