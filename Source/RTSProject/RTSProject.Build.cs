// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RTSProject : ModuleRules
{
	public RTSProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"NavigationSystem", 
			"AIModule", 
			"Niagara", 
			"UMG", 
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"EnhancedInput"
		});
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "ImageWrapper", "RenderCore" });
    }
}
