// Copyright 2024 MobileShooter. All Rights Reserved.

#include "PlayerState/MobileShooterPlayerState.h"
#include "Net/UnrealNetwork.h"

AMobileShooterPlayerState::AMobileShooterPlayerState()
{
}

void AMobileShooterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMobileShooterPlayerState, Kills);
	DOREPLIFETIME(AMobileShooterPlayerState, Deaths);
	DOREPLIFETIME(AMobileShooterPlayerState, Assists);
	DOREPLIFETIME(AMobileShooterPlayerState, Team);
}

void AMobileShooterPlayerState::AddKill()
{
	if (HasAuthority())
	{
		++Kills;
		AddScore(100.f);
	}
}

void AMobileShooterPlayerState::AddDeath()
{
	if (HasAuthority())
	{
		++Deaths;
	}
}

void AMobileShooterPlayerState::AddAssist()
{
	if (HasAuthority())
	{
		++Assists;
		AddScore(25.f);
	}
}

void AMobileShooterPlayerState::SetTeam(ETeam NewTeam)
{
	if (HasAuthority())
	{
		Team = NewTeam;
	}
}

float AMobileShooterPlayerState::GetKDRatio() const
{
	return Deaths == 0 ? static_cast<float>(Kills) : static_cast<float>(Kills) / static_cast<float>(Deaths);
}

void AMobileShooterPlayerState::OnRep_Kills()   {}
void AMobileShooterPlayerState::OnRep_Deaths()  {}
void AMobileShooterPlayerState::OnRep_Assists() {}
void AMobileShooterPlayerState::OnRep_Team()    {}
