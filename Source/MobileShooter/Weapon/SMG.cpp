// Copyright 2024 MobileShooter. All Rights Reserved.

#include "Weapon/SMG.h"

ASMG::ASMG()
{
	WeaponType = EWeaponType::SMG;
	FireMode   = EFireMode::Automatic;

	WeaponStats.BaseDamage        = 18.f;    // Lower damage than AR
	WeaponStats.HeadshotMultiplier = 2.0f;
	WeaponStats.FireRate          = 900.f;   // 900 RPM — very fast
	WeaponStats.MaxRange          = 3000.f;  // Shorter effective range
	WeaponStats.Spread            = 2.5f;    // More spread than AR
	WeaponStats.MagazineSize      = 40;
	WeaponStats.MaxAmmo           = 200;
	WeaponStats.ReloadTime        = 1.8f;
}
