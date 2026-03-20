// Copyright 2024 MobileShooter. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MobileShooterEditorTarget : TargetRules
{
	public MobileShooterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5Latest;
		ExtraModuleNames.Add("MobileShooter");
	}
}
