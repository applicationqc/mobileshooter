// Copyright 2024 MobileShooter. All Rights Reserved.

#include "GameMode/DeathmatchGameMode.h"
#include "PlayerState/MobileShooterPlayerState.h"

ADeathmatchGameMode::ADeathmatchGameMode()
{
	GameModeType = EGameModeType::Deathmatch;
	ScoreLimit   = 30;
	MaxPlayers   = 12;
}

void ADeathmatchGameMode::OnPlayerEliminated(AMobileShooterCharacter* EliminatedCharacter,
                                              AController* VictimController,
                                              AController* KillerController)
{
	if (KillerController)
	{
		AMobileShooterPlayerState* KillerPS = KillerController->GetPlayerState<AMobileShooterPlayerState>();
		if (KillerPS)
		{
			KillerPS->AddKill();

			// Check individual kill limit
			if (KillerPS->GetKills() >= ScoreLimit)
			{
				EndMatch();
				return;
			}
		}
	}

	if (VictimController)
	{
		AMobileShooterPlayerState* VictimPS = VictimController->GetPlayerState<AMobileShooterPlayerState>();
		if (VictimPS)
		{
			VictimPS->AddDeath();
		}
	}

	RequestRespawn(EliminatedCharacter, VictimController);
}
