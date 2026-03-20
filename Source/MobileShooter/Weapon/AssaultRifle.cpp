// Copyright 2024 MobileShooter. All Rights Reserved.

#include "Weapon/AssaultRifle.h"

AAssaultRifle::AAssaultRifle()
{
	WeaponType = EWeaponType::AssaultRifle;
	FireMode   = EFireMode::Automatic;

	WeaponStats.BaseDamage         = 25.f;
	WeaponStats.HeadshotMultiplier = 2.5f;
	WeaponStats.FireRate           = 700.f;   // rounds per minute
	WeaponStats.MaxRange           = 6000.f;  // cm
	WeaponStats.Spread             = 1.5f;    // degrees
	WeaponStats.MagazineSize       = 30;
	WeaponStats.MaxAmmo            = 120;
	WeaponStats.ReloadTime         = 2.0f;    // seconds
}
