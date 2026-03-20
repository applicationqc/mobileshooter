// Copyright 2024 MobileShooter. All Rights Reserved.

#include "Weapon/Pistol.h"

APistol::APistol()
{
	WeaponType = EWeaponType::Pistol;
	FireMode   = EFireMode::Single;

	WeaponStats.BaseDamage        = 30.f;    // Higher damage per shot than SMG
	WeaponStats.HeadshotMultiplier = 3.0f;   // High headshot reward
	WeaponStats.FireRate          = 400.f;   // Semi-auto — limited by animation
	WeaponStats.MaxRange          = 4000.f;
	WeaponStats.Spread            = 0.8f;    // Fairly accurate
	WeaponStats.MagazineSize      = 15;
	WeaponStats.MaxAmmo           = 60;
	WeaponStats.ReloadTime        = 1.5f;
}
