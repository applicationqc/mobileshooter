// Copyright 2024 MobileShooter. All Rights Reserved.

#include "GameMode/MobileShooterGameMode.h"
#include "Character/MobileShooterCharacter.h"
#include "PlayerState/MobileShooterPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AMobileShooterGameMode::AMobileShooterGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMobileShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartMatch();
}

void AMobileShooterGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bMatchInProgress)
	{
		ElapsedMatchTime += DeltaSeconds;
		if (ElapsedMatchTime >= MatchDuration)
		{
			EndMatch();
		}
	}
}

void AMobileShooterGameMode::StartMatch()
{
	bMatchInProgress = true;
	ElapsedMatchTime = 0.f;
}

void AMobileShooterGameMode::EndMatch()
{
	bMatchInProgress = false;
	OnMatchEnded.Broadcast();
}

float AMobileShooterGameMode::GetRemainingMatchTime() const
{
	return FMath::Max(0.f, MatchDuration - ElapsedMatchTime);
}

void AMobileShooterGameMode::OnPlayerEliminated(AMobileShooterCharacter* EliminatedCharacter,
                                                 AController* VictimController,
                                                 AController* KillerController)
{
	if (KillerController)
	{
		AddScore(KillerController, 100);
	}

	RequestRespawn(EliminatedCharacter, VictimController);
}

void AMobileShooterGameMode::RequestRespawn(ACharacter* EliminatedCharacter,
                                             AController* EliminatedController)
{
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Reset();
		EliminatedCharacter->Destroy();
	}

	if (EliminatedController)
	{
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindLambda([this, EliminatedController]()
		{
			if (IsValid(EliminatedController))
			{
				AActor* StartSpot = ChoosePlayerStart_Implementation(EliminatedController);
				RestartPlayerAtPlayerStart(EliminatedController, StartSpot);
			}
		});

		GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
	}
}

void AMobileShooterGameMode::AddScore(AController* ScoringController, int32 Points)
{
	if (!ScoringController)
	{
		return;
	}

	AMobileShooterPlayerState* PS = ScoringController->GetPlayerState<AMobileShooterPlayerState>();
	if (PS)
	{
		PS->AddScore(static_cast<float>(Points));

		if (ScoreLimit > 0 && PS->GetScore() >= ScoreLimit)
		{
			EndMatch();
		}
	}
}

TArray<FMatchResult> AMobileShooterGameMode::GetMatchResults() const
{
	TArray<FMatchResult> Results;

	for (TObjectIterator<AMobileShooterPlayerState> It; It; ++It)
	{
		AMobileShooterPlayerState* PS = *It;
		if (PS && PS->GetWorld() == GetWorld())
		{
			FMatchResult Result;
			Result.PlayerName = PS->GetPlayerName();
			Result.Kills      = PS->GetKills();
			Result.Deaths     = PS->GetDeaths();
			Result.Assists    = PS->GetAssists();
			Result.Score      = static_cast<int32>(PS->GetScore());
			Result.KillStreak = PS->GetBestKillStreak();
			Results.Add(Result);
		}
	}

	Results.Sort([](const FMatchResult& A, const FMatchResult& B)
	{
		return A.Score > B.Score;
	});

	return Results;
}

AActor* AMobileShooterGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AMobileShooterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
