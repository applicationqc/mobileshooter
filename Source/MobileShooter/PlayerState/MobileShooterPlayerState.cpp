// Copyright 2024 MobileShooter. All Rights Reserved.

#include "PlayerState/MobileShooterPlayerState.h"
#include "Net/UnrealNetwork.h"

// ─── Kill streak milestone constants ─────────────────────────────────────────
static constexpr int32 KillStreak_Tier1      = 3;
static constexpr int32 KillStreak_Tier2      = 5;
static constexpr int32 KillStreak_Tier3      = 7;
static constexpr int32 KillStreak_Tier4      = 10;

static constexpr float KillStreakBonus_Tier1 = 50.f;
static constexpr float KillStreakBonus_Tier2 = 100.f;
static constexpr float KillStreakBonus_Tier3 = 150.f;
static constexpr float KillStreakBonus_Tier4 = 200.f;

static constexpr float BaseKillScore        = 100.f;
// ─────────────────────────────────────────────────────────────────────────────

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
	DOREPLIFETIME(AMobileShooterPlayerState, KillStreak);
}

void AMobileShooterPlayerState::AddKill()
{
	if (HasAuthority())
	{
		++Kills;
		++KillStreak;
		if (KillStreak > BestKillStreak)
		{
			BestKillStreak = KillStreak;
		}

		// Bonus score for kill streaks at milestone thresholds
		float Bonus = 0.f;
		if      (KillStreak >= KillStreak_Tier4) { Bonus = KillStreakBonus_Tier4; }
		else if (KillStreak == KillStreak_Tier3) { Bonus = KillStreakBonus_Tier3; }
		else if (KillStreak == KillStreak_Tier2) { Bonus = KillStreakBonus_Tier2; }
		else if (KillStreak == KillStreak_Tier1) { Bonus = KillStreakBonus_Tier1; }

		AddScore(BaseKillScore + Bonus);
	}
}

void AMobileShooterPlayerState::AddDeath()
{
	if (HasAuthority())
	{
		++Deaths;
		KillStreak = 0;
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
void AMobileShooterPlayerState::OnRep_KillStreak() {}
