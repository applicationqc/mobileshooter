// Copyright 2024 MobileShooter. All Rights Reserved.

using UnrealBuildTool;

public class MobileShooter : ModuleRules
{
	public MobileShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"UMG",
			"Slate",
			"SlateCore",
			"NetCore",
			"ReplicationGraph",
			"PhysicsCore",
			"NavigationSystem",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"RenderCore",
			"RHI",
			"AudioMixer",
			"AudioModulation",
			"Json",
			"JsonUtilities",
			"HTTP"
		});

		// Mobile-specific settings
		if (Target.Platform == UnrealTargetPlatform.Android ||
		    Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicDependencyModuleNames.Add("MobileGameplay");
		}
	}
}
