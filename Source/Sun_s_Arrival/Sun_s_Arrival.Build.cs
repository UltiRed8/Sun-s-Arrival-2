// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sun_s_Arrival : ModuleRules
{
	public Sun_s_Arrival(ReadOnlyTargetRules Target) : base(Target)
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
            "OnlineSubsystemEOS",
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });

		//PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemSteam" });

		PublicIncludePaths.AddRange(new string[] {
			"Sun_s_Arrival"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
