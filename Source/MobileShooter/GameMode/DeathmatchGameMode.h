// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/MobileShooterGameMode.h"
#include "DeathmatchGameMode.generated.h"

/**
 * Free-for-All Deathmatch game mode.
 * No teams — every player fights for themselves.
 * First player to reach the kill limit wins.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API ADeathmatchGameMode : public AMobileShooterGameMode
{
	GENERATED_BODY()

public:
	ADeathmatchGameMode();

	virtual void OnPlayerEliminated(AMobileShooterCharacter* EliminatedCharacter,
	                                AController* VictimController,
	                                AController* KillerController) override;
};
