// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SunArrival2 : ModuleRules
{
	public SunArrival2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
            //"OnlineSubsystemEOS",
            //"OnlineSubsystem",
            //"OnlineSubsystemUtils",
			"RenderCore",
			"RHI"
        });

		//PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemSteam" });

		PublicIncludePaths.AddRange(new string[] {
			"SunArrival2"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
