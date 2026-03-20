// Copyright 2024 MobileShooter. All Rights Reserved.

#include "GameMode/TeamDeathmatchGameMode.h"
#include "PlayerState/MobileShooterPlayerState.h"

ATeamDeathmatchGameMode::ATeamDeathmatchGameMode()
{
	GameModeType = EGameModeType::TeamDeathmatch;
	ScoreLimit   = 0; // Disabled — team score drives the match end
}

void ATeamDeathmatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AssignTeam(NewPlayer);
}

void ATeamDeathmatchGameMode::AssignTeam(APlayerController* NewPlayer)
{
	if (!NewPlayer)
	{
		return;
	}

	AMobileShooterPlayerState* PS = NewPlayer->GetPlayerState<AMobileShooterPlayerState>();
	if (!PS)
	{
		return;
	}

	// Balance teams — assign player to the smaller side
	if (AlphaCount <= BravoCount)
	{
		PS->SetTeam(ETeam::TeamAlpha);
		++AlphaCount;
	}
	else
	{
		PS->SetTeam(ETeam::TeamBravo);
		++BravoCount;
	}
}

void ATeamDeathmatchGameMode::OnPlayerEliminated(AMobileShooterCharacter* EliminatedCharacter,
                                                   AController* VictimController,
                                                   AController* KillerController)
{
	// Award kill to the killer's team
	if (KillerController)
	{
		AMobileShooterPlayerState* KillerPS = KillerController->GetPlayerState<AMobileShooterPlayerState>();
		if (KillerPS)
		{
			KillerPS->AddKill();

			if (KillerPS->Team == ETeam::TeamAlpha)
			{
				++TeamAlphaScore;
			}
			else if (KillerPS->Team == ETeam::TeamBravo)
			{
				++TeamBravoScore;
			}
		}
	}

	// Record death for victim
	if (VictimController)
	{
		AMobileShooterPlayerState* VictimPS = VictimController->GetPlayerState<AMobileShooterPlayerState>();
		if (VictimPS)
		{
			VictimPS->AddDeath();
		}
	}

	// Check win condition
	if (TeamAlphaScore >= TeamScoreLimit || TeamBravoScore >= TeamScoreLimit)
	{
		EndMatch();
		return;
	}

	RequestRespawn(EliminatedCharacter, VictimController);
}
