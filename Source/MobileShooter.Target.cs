// Copyright 2024 MobileShooter. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MobileShooterTarget : TargetRules
{
	public MobileShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5Latest;
		ExtraModuleNames.Add("MobileShooter");
	}
}
