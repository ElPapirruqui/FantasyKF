// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FantasyKF : ModuleRules
{
	public FantasyKF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "NavigationSystem", "GameplayTasks", "UMG", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam" });
	}
}
